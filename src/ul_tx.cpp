/*! \file ul_tx.cpp
 *  \brief C++ file for the transmitter class.
 *  This class is a simple PN-sequence transmitter.
 *
 */

#include "ul_tx.h"
#include <iostream>

namespace win {

    /*!
     *  This constructor shows exactly what parameters need to be set for the transmitter
     */
    ul_tx::ul_tx(double freq, double samp_rate, double tx_gain, double tx_amp, std::string device_addr) 
    {
    }

    /*!
     *  Transmits a single frame, blocking until the frame is sent.
     */
    void ul_tx::send_data(std::vector<std::complex<double> > payload)
    {
        uhd::tx_metadata_t tx_metadata;
        tx_metadata.start_of_burst = true;
        tx_metadata.end_of_burst = true;
        tx_metadata.has_time_spec = false;
        m_tx_streamer->send(&payload[0], payload.size(), tx_metadata);
    }

    /*!
     *  Set transmit metadata.
     */
    void ul_tx::set_md(bool sob, bool eob, bool hts, uhd::time_spec_t tspec)
    {
        tx_md.start_of_burst = sob;
        tx_md.end_of_burst = eob;
        tx_md.has_time_spec = hts;
        tx_md.time_spec = tspec;
    }

    /*!
     * \Initialize the usrp
     */
    void init_usrp()
    {
        // Instantiate the multi_usrp
        m_usrp = uhd::usrp::multi_usrp::make(uhd::device_addr_t(device_addr));
        m_device = m_usrp->get_device();

        // Set the center frequency
        m_usrp->set_tx_freq(uhd::tune_request_t(cen_freq));
        m_usrp->set_rx_freq(uhd::tune_request_t(cen_freq));

        // Set the sample rate
        m_usrp->set_tx_rate(samp_rate);
        m_usrp->set_rx_rate(samp_rate);

        // Set the gains
        m_usrp->set_tx_gain(tx_gain);
        m_usrp->set_rx_gain(tx_gain);

        // Set the RX antenna
        //m_usrp->set_rx_antenna("RX2");

        // Get the TX and RX stream handles
        m_tx_streamer = m_usrp->get_tx_stream(uhd::stream_args_t("fc64"));
        m_rx_streamer = m_usrp->get_rx_stream(uhd::stream_args_t("fc64"));
    }

    /*!
     *  Set central frequency.
     */
    void ul_tx::set_cen_fre(double freq)
    {
        cen_freq = freq;
    }

    /*!
     *  Set sampling rate.
     */
    void ul_tx::set_samp_rate(double rate)
    {
        samp_rate = rate;
    }

    /*!
     *  Set transmit gain.
     */
    void ul_tx::set_tx_gain(double gain)
    {
        tx_gain = gain;
    }

    /*!
     *  Set device address.
     */
    void ul_tx::set_dev_addr(std::string addr)
    {
        device_addr = addr;
    }

}
