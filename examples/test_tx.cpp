
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <iostream>
#include <fstream>
#include "ul_tx.h"

#define PNSEQLEN 2000
#define PKTLEN 640
#define ULSEQLEN 160

using namespace win;

double freq = 2e9;
double sample_rate = 10e6;
double tx_gain = 30;
//double rx_gain = 30;
double amp = 0.5;
std::string device_addr = "addr=192.168.10.2";

char pnseq[PNSEQLEN];
unsigned int pnoffset;

int main(int argc, char * argv[]){

    if (argc<2)
    {
        std::cout << "Not enough arguments." << std::endl;
        return(-1);
    }
    else
    {
        pnoffset = strtol(argv[1], NULL, 10);
    }
    set_realtime_priority();

	std::cout << "Start transmit chain..." << std::endl;
    srand(time(NULL)); //Initialize random seed

    get_pnsequence();
    ul_tx tx = ul_tx(freq, sample_rate, tx_gain, amp, device_addr);

    std::vector<unsigned char> ulseq(ULSEQLEN);
    std::vector<unsigned char> packets(PKTLEN,0);

    for(int i = 0; i < ULSEQLEN; i++)
    {
        // ulseq[i] = pnseq[pnoffset + i];
        packets[i] = pnseq[pnoffset + i];
    }

    tx.init_usrp();

    int tx_count = 0;
    while(1)
    {
        // test_tx(freq, sample_rate, tx_gain, amp, phy_rate);
        // // Transmit all the packets
        // for(int i = 0; i < 10000; i++)
        // {
            // tx.send_data(packets, phy_rate);
        // }
        // std::cout << "Transmission number : " << ++tx_count << std::endl;
        // sleep(1);
    }
}

