#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <system_error>
#include "shost.h"
#include <mpsse.h>
// #include "libMPSSE_spi.h"
#include "SPI.h"

SPI::SPI(): Protocol("SPI") {

}

void SPI::_write(shost_xfer_t *xfer) {
    int ret = 0;
    // Open an MPSSE instance for SPI0 and store the context
    this->mpsse = MPSSE(modes::SPI0, xfer->clk, MSB);

    // Ensure the context is not NULL and and the mpsse channel is open
    if (this->mpsse != NULL && !this->mpsse->open) {
        // TODO what should be something like: "Failed to initialize MPSSE: %s\n", ErrorString(this->mpsse)
        throw std::system_error(EIO, std::generic_category(), ErrorString(this->mpsse));
    }

    ret = spi_write(xfer->tx_buff, xfer->rx_buff, xfer->len);

    Close(this->mpsse);

    switch (ret) {
        case 1:
            // TODO printing address whould be nice
            throw std::system_error(ENXIO, std::generic_category(), "No response from peripheral.");
            break;
        case 2:
            throw std::system_error(EIO, std::generic_category(), "Hardware failure.");
            break;
        default:
            // all is good, no need to do anything.
            break;
    }
    xfer->bytesTranferred = xfer->len;
}

void SPI::_read(shost_xfer_t *xfer) {
    int ret = 0;
    // Open an MPSSE instance for SPI0 and store the context
    this->mpsse = MPSSE(modes::SPI0, xfer->clk, MSB);

    // Ensure the context is not NULL and and the mpsse channel is open
    if (this->mpsse != NULL && !this->mpsse->open) {
        // TODO what should be something like: "Failed to initialize MPSSE: %s\n", ErrorString(this->mpsse)
        throw std::system_error(EIO, std::generic_category(), ErrorString(this->mpsse));
    }

    ret = spi_read(xfer->rx_buff, xfer->len);

    Close(this->mpsse);

    switch (ret) {
        case 1:
            // TODO printing address whould be nice
            throw std::system_error(ENXIO, std::generic_category(), "No response from peripheral.");
            break;
        case 2:
            throw std::system_error(EIO, std::generic_category(), "Hardware failure.");
            break;
        default:
            // all is good, no need to do anything.
            break;
    }
    xfer->bytesTranferred = xfer->len;

}

shost_ret_t SPI::spi_write(uint8_t *src_buffer, uint8_t *dest_buffer, size_t buffer_len) {
    shost_ret_t retVal = SHOST_RET_OK;

    // Send the start condition for SPI
    retVal = (shost_ret_t)Start(mpsse);

    if( SHOST_RET_OK == retVal ) {
        // Write our buffer and store the data read back
        retVal = (shost_ret_t)FastTransfer(mpsse, (char*)src_buffer, (char*)dest_buffer, buffer_len);
    }

    if( SHOST_RET_OK == retVal ) {
        // Send the stop condition for SPI
        retVal = (shost_ret_t)Stop(mpsse);
    }

    return retVal;
}

shost_ret_t SPI::spi_read(uint8_t *dest_buffer, size_t buffer_len) {
    shost_ret_t retVal = SHOST_RET_OK;

    // Send the start condition for SPI
    retVal = (shost_ret_t)Start(mpsse);

    if( SHOST_RET_OK == retVal ) {
        // Write our buffer
        retVal = (shost_ret_t)FastRead(mpsse, (char*)dest_buffer, buffer_len);
    }

    if( SHOST_RET_OK == retVal ) {
        // Send the stop condition for SPI
        retVal = (shost_ret_t)Stop(mpsse);
    }

    return retVal;
}
