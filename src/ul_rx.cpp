/*! \file ul_rx.cpp
 *  \brief C++ file for the transmitter class.
 *  This class is a simple PN-sequence transmitter.
 *
 */

#include "ul_rx.h"
#include <iostream>

namespace win {

    /*!
     *  This constructor shows exactly what parameters need to be set for the transmitter
     */
    ul_rx::ul_rx() 
    {
    }

    /*!
     *  Transmits a single frame, blocking until the frame is sent.
     */
    void ul_rx::recv_data(int num_samples, std::vector<std::complex<double> > & buffer)
    {
        m_rx_streamer->recv(&buffer[0], num_samples, rx_md);
    }

    /*!
     *  Get current time.
     */
    uhd::time_spec_t ul_rx::get_usrp_time()
    {
        return(m_usrp->get_time_now());
    }

    /*!
     * \Initialize the usrp
     */
    void ul_rx::init_usrp()
    {
        // Instantiate the multi_usrp
        m_usrp = uhd::usrp::multi_usrp::make(uhd::device_addr_t(device_addr));
        m_device = m_usrp->get_device();

        // Set the center frequency
        m_usrp->set_rx_freq(uhd::tune_request_t(cen_freq));

        // Set the sample rate
        m_usrp->set_rx_rate(samp_rate);

        // Set the gains
        m_usrp->set_rx_gain(rx_gain);
        m_usrp->set_rx_gain(rx_gain);

        // Set the RX antenna
        //m_usrp->set_rx_antenna("RX2");

        // Get the TX and RX stream handles
        m_rx_streamer = m_usrp->get_rx_stream(uhd::stream_args_t("fc64"));
        m_rx_streamer = m_usrp->get_rx_stream(uhd::stream_args_t("fc64"));
    }

    /*!
     *  Set central frequency.
     */
    void ul_rx::set_cen_fre(double freq)
    {
        cen_freq = freq;
    }

    /*!
     *  Set sampling rate.
     */
    void ul_rx::set_samp_rate(double rate)
    {
        samp_rate = rate;
    }

    /*!
     *  Set transmit gain.
     */
    void ul_rx::set_rx_gain(double gain)
    {
        rx_gain = gain;
    }

    /*!
     *  Set device address.
     */
    void ul_rx::set_dev_addr(std::string addr)
    {
        device_addr = addr;
    }

    void ul_rx::reset_usrp_time()
    {
        m_usrp->set_time_now(uhd::time_spec_t(0.0));
    }

    void ul_rx::reset_usrp_time_with_pps()
    {
        m_usrp->set_time_unknown_pps(uhd::time_spec_t(0.0));
    }

    void ul_rx::use_external_clock()
    {
        m_usrp->set_clock_source("external");
    }

}
