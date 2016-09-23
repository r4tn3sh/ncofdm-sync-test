/*! \file transmitter.h
 *  \brief Header file for transmitter class.
 *
 *  The transmitter class is the public interface for the wl_sync_test transmit chain.
 *  This is the easiest way to start transmitting 802.11a OFDM frames out of the box.
 */


#ifndef UL_RX_H
#define UL_RX_H

#include <vector>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/convert.hpp>
#include <uhd/stream.hpp>
#include <uhd/types/time_spec.hpp>

namespace win {

    /*!
     * \brief The transmitter class is the public interface for the wl_sync_test transmit chain.
     *
     *  Usage: Create a transmitter object with the desired USRP parameters (center frequency,
     *  sample rate, transmitter gain, and amplitude). Then to send a packet simply call
     *  the transmitter::send_frame() function passing it the desired packet to be transmitted
     *  and the desired Physical Layer rate (PHY Rate) to transmit it.
     */
    class ul_rx
    {
    public:

        /*!
         * \brief Constructor for the transmitter with raw parameters
         * \param freq [Optional] Center frequency
         * \param samp_rate [Optional] Sample rate
         * \param rx_gain [Optional] Transmit Gain
         * \param rx_amp [Optional] Transmit Amplitude
         * \param device_addr [Optional] IP address of USRP device
         *
         *  Defaults to:
         *  - center_freq -> 5.72e9 (5.72 GHz)
         *  - sample_rate -> 5e6 (5 MHz)
         *  - rx_gain -> 20
         *  - device_addr -> "" (empty string will default to letting the UHD api
         *    automatically find an available USRP)
         *  - *Note:
         *     + rx_gain -> 20 even though it is irrelevant for the transmitter
         */
        ul_rx();

        /*!
         * \brief Send a single PHY frame at the given PHY Rate
         */
        void recv_data(std::vector<std::complex<double> > payload);

        /*!
         * \Initialize the usrp
         */
        void init_usrp();
       
        /*!
         *  Set transmit metadata.
         */
        void set_rxmetadata(bool sob, bool eob, bool hts, uhd::time_spec_t tspec);
       
        /*!
         *  Set central frequency.
         */
        void set_cen_fre(double freq);
       
        /*!
         *  Set sampling rate.
         */
        void set_samp_rate(double rate);
       
        /*!
         *  Set transmit gain.
         */
        void set_rx_gain(double gain);
       
        /*!
         *  Set device address.
         */
        void set_dev_addr(std::string addr); 

        /*!
         *  Set USRP time to 0.
         */
        void reset_usrp_time();

        void reset_usrp_time_with_pps();

        void use_external_clock();

        uhd::time_spec_t get_usrp_time();

    private:

        boost::shared_ptr<uhd::usrp::multi_usrp> m_usrp; //!< multi_usrp (main USRP handle)
        boost::shared_ptr<uhd::device> m_device;         //!< device (receives async messages)
        uhd::rx_streamer::sptr m_rx_streamer;            //!< TX (output) streamer
        uhd::rx_metadata_t rx_md;
        double cen_freq = 2e9;
        double samp_rate = 10e6;
        double rx_gain = 20;
        double rx_amp = 1.0;
        std::string device_addr = "addr=192.168.10.2";

    };

}



#endif // UL_RX_H
