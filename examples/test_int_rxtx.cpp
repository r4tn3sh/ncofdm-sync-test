
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "ul_rxtx.h"

#define DATALEN 2000
#define PKTLEN 800
#define ULSEQLEN 160
#define COEFFTHRESH 0.5

bool get_data();
int seq_detector(std::vector<std::complex<double> > samples);
int correlate_ulseq(std::vector<std::complex<double> > samples);

using namespace win;

double freq = 2e9;
double samp_rate = 10e6;
double rx_gain = 20;
//double rx_gain = 30;
double amp = 2.0;
std::string device_addr = "addr=192.168.10.2";

char pnseq[DATALEN];
double pnseq_d[ULSEQLEN];
std::vector<std::complex<double> > data(DATALEN);
unsigned int fileindex;
unsigned int pnsearch = 0;

double tx_time = 0.0;
double rx_time = 0.0;

int main(int argc, char * argv[]){

    if (argc<2)
    {
        std::cout << "Not enough arguments." << std::endl;
        return(-1);
    }
    else
    {
        fileindex = strtol(argv[1], NULL, 10);
    }

    get_data();
    std::cout << "File read complete." << std::endl;

    ul_rxtx rxtx = ul_rxtx();

    std::vector<std::complex<double> > samples(PKTLEN);
    for (int i = 0; i<PKTLEN; i++)
    {
        samples[i] = amp*data[i];
    }

    rxtx.init_usrp();
    rxtx.use_external_clock();
    // rxtx.reset_usrp_time();

    const size_t num_of_samples = PKTLEN;
    std::vector<std::complex<double> > rx_samples(num_of_samples); //!< Vector to hold the raw samples received from the USRP and passed into the ul_receiver_chain

    int rx_count = 0;
    double rx_time = 0.0;
    // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    double curr_time = rxtx.get_usrp_time().get_real_secs();

    double samp_duration = 1.0/samp_rate;

    // ********** Detection ************
    std::cout << "PN sequence detection started at "<< curr_time << std::endl;
    auto begin_time = std::chrono::high_resolution_clock::now();
    uhd::rx_metadata_t rx_metadata;
    while(1)
    {
        rxtx.recv_data(num_of_samples, rx_samples);
        int pk_index = PKTLEN;
        if (pnsearch%73 == 0)
        {
            rx_metadata = rxtx.rx_md;
            pk_index = correlate_ulseq(rx_samples);
        }

        pnsearch++;

        if(pk_index < PKTLEN)
        {
            rx_time = rx_metadata.time_spec.get_full_secs() + rx_metadata.time_spec.get_frac_secs();
            rx_time += pk_index*samp_duration;
            std::cout<< "Signal found at " << pk_index << " at time " << rx_time << std::endl;
            std::cout<< "Detailed frame start time : " <<rx_metadata.time_spec.get_full_secs() << " + " <<rx_metadata.time_spec.get_frac_secs() << std::endl;
            tx_time = rx_time + samp_duration*(PKTLEN+160) +0.96; //FIXME: hack for 960 sample repetition
            std::cout<< "Transmission scheduled at "  << tx_time << std::endl;
            break;
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time-begin_time).count();
        if(duration > 30000000)
        {
            std::cout<< "Timeout occured" << std::endl;
            break;
        }
    }
    rxtx.stop_rx();
    // ********** Transmission ************
    double rep_time = samp_duration*(PKTLEN+160);
    std::cout << "Current time is " << rxtx.get_usrp_time().get_real_secs() << std::endl; 
    std::cout << "Transmisson started ..." << std::endl;

    while(1)
    {
        tx_time += rep_time;
        rxtx.set_txmetadata(true, true, true, uhd::time_spec_t(tx_time));
        rxtx.send_data(samples);
    }
}


int seq_detector(std::vector<std::complex<double> > samples)
{
    uhd::rx_metadata_t rx_md;
    return(1);
}

/*!
 * \Correlate and find signal and return the index of the peak
 */
int correlate_ulseq(std::vector<std::complex<double> > samples)
{

    std::complex<double> temp_mul;
    std::complex<double> temp_mean;
    double temp_norm_v;
    double corr_coeff = 0.0;
    double sqr_sum = 0.0;
    double numr;
    double denm;
    unsigned int N = ULSEQLEN;

    double pn_mean=0.0;
    for (int j=0; j<N; j++)
    {
        pn_mean += pnseq_d[j];
    }
    pn_mean/=N;
    double test_thresh = 0.5;
    // std::cout << "PN mean : " << pn_mean << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    int peak_location = PKTLEN;
    for (int i=0; i<PKTLEN; i++)
    {
        if(i%1>0)
            continue;
        temp_mul = (0.0, 0.0);
        temp_mean = (0.0, 0.0);
        sqr_sum = 0.0;
        for (int j=0; j<N; j++)
        {
            temp_mul += samples[i+j] * pnseq_d[j];
            sqr_sum += pow(abs(samples[i+j]),2);
            temp_mean += samples[i+j];
        }
        // std::cout << "Sample sum : " << temp_mean << std::endl;
        temp_mean/=N;
        temp_norm_v = 0.0;
        std::complex<double> scaled_temp_mean = N*pn_mean*temp_mean;
        numr = abs(temp_mul-scaled_temp_mean);
        denm = sqrt(sqr_sum-N*pow(abs(temp_mean),2))*sqrt(N);
        corr_coeff = numr/denm;

        if(corr_coeff<-test_thresh || corr_coeff>test_thresh)
            std::cout << "Correlation coefficient : " << corr_coeff << " " << i <<  std::endl;

        if(corr_coeff>COEFFTHRESH)
        {
            std::cout << "Correlation coefficient above threshold. " << corr_coeff  << std::endl;
            peak_location = i;
            return(i);
        }
    }
    return(peak_location);
}

/*!
 * \Read the PN sequence from the file generated by MATLAB
 */
bool get_data()
{
    std::ifstream datafile;
    float tempdata[2*DATALEN] = {0.0};
    if(fileindex == 0)
        datafile.open("../int_data1.dat", std::ios::in | std::ios::binary);
    else
        datafile.open("../int_data2.dat", std::ios::in | std::ios::binary);

    if(!datafile) 
    {
        std::cout << "Cannot open file.\n";
        return false;
    }
    else
    {
        std::cout << "Data file opened.\n";
    }
    datafile.read((char*)&tempdata, 8*DATALEN);

    for(int i=0; i<DATALEN; i++)
    {
        data[i].real(tempdata[2*i]);
        data[i].imag(0-tempdata[2*i+1]);
    }

    std::ifstream pnfile; 
    pnfile.open("../pnseq.dat", std::ios::in | std::ios::binary);
    if(!pnfile) 
    {
        std::cout << "Cannot open file.\n";
        return false;
    }
    else
    {
        std::cout << "PN-seq file opened.\n";
    }
    pnfile.read(pnseq, DATALEN);
    std::cout << "PN-seq file read complete.\n";


    for (int i=0; i<ULSEQLEN; i++)
    {
        if(fileindex == 0)
            pnseq_d[i] = 2*((double)(pnseq[200+i]))-1;
        else
            pnseq_d[i] = 2*((double)(pnseq[i]))-1;
    }
}

