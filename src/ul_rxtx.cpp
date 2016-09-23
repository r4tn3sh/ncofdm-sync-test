/*! \file ul_rxtx.cpp
 *  \brief C++ file for the transmitter class.
 *  This class is a simple PN-sequence transmitter.
 *
 */

#include "ul_rxtx.h"
#include <iostream>

namespace win {

    /*!
     *  This constructor shows exactly what parameters need to be set for the transmitter
     */
    ul_rxtx::ul_rxtx() 
    {
    }

    /*!
     *  Transmits a single frame, blocking until the frame is sent.
     */
    void ul_rxtx::send_data(std::vector<std::complex<double> > payload)
    {
        //std::cout << tx_md.has_time_spec << " " <<tx_md.time_spec.get_real_secs() << std::endl;
        size_t num_rx_data = m_tx_streamer->send(&payload[0], payload.size(), tx_md, 2.1);
        //std::cout << num_rx_data << std::endl;
    }


    /*!
     * Receive frame 
     */
    void ul_rxtx::recv_data(int num_samples, std::vector<std::complex<double> > & buffer)
    {
        m_rx_streamer->recv(&buffer[0], num_samples, rx_md, 3.0);
        if (rx_md.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT) {
            std::cout << boost::format("Timeout while streaming") << std::endl;
        }
    }

    /*!
     *  Set transmit metadata.
     */
    void ul_rxtx::set_txmetadata(bool stob, bool eob, bool hts, uhd::time_spec_t tspec)
    {
        tx_md.start_of_burst = stob;
        tx_md.end_of_burst = eob;
        tx_md.has_time_spec = hts;
        tx_md.time_spec = tspec;
    }

    /*!
     *  Get current time.
     */
    uhd::time_spec_t ul_rxtx::get_usrp_time()
    {
        return(m_usrp->get_time_now());
    }

    /*!
     * \Initialize the usrp
     */
    void ul_rxtx::init_usrp()
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
        m_usrp->set_rx_gain(rx_gain);

        // Set the RX antenna
        //m_usrp->set_rx_antenna("RX2");

        // Get the TX and RX stream handles
        m_tx_streamer = m_usrp->get_tx_stream(uhd::stream_args_t("fc64"));
        m_rx_streamer = m_usrp->get_rx_stream(uhd::stream_args_t("fc64"));

        // Start the RX stream
        uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
        stream_cmd.stream_now = true;
        //m_usrp->issue_stream_cmd(stream_cmd);
        m_rx_streamer->issue_stream_cmd(stream_cmd);
    }

    /*!
     *  Shut down rx.
     */
    void ul_rxtx::stop_rx()
    {
        // Shut down receiver
        //stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
        //m_rx_streamer->issue_stream_cmd(stream_cmd);
    }

    /*!
     *  Set central frequency.
     */
    void ul_rxtx::set_cen_fre(double freq)
    {
        cen_freq = freq;
    }

    /*!
     *  Set sampling rate.
     */
    void ul_rxtx::set_samp_rate(double rate)
    {
        samp_rate = rate;
    }

    /*!
     *  Set receive gain.
     */
    void ul_rxtx::set_rx_gain(double gain)
    {
        rx_gain = gain;
    }

    /*!
     *  Set transmit gain.
     */
    void ul_rxtx::set_tx_gain(double gain)
    {
        tx_gain = gain;
    }

    /*!
     *  Set device address.
     */
    void ul_rxtx::set_dev_addr(std::string addr)
    {
        device_addr = addr;
    }

    void ul_rxtx::reset_usrp_time()
    {
        m_usrp->set_time_now(uhd::time_spec_t(0.0));
    }

    void ul_rxtx::reset_usrp_time_with_pps()
    {
        m_usrp->set_time_unknown_pps(uhd::time_spec_t(0.0));
    }

    void ul_rxtx::use_external_clock()
    {
        m_usrp->set_clock_source("external");
    }

}
