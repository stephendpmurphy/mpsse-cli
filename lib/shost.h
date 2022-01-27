#ifndef _XFER_H_
#define _XFER_H_

#include <stdint.h>
#include <stddef.h>
#include "ftdi.h"

typedef enum {
    SHOST_RET_OK = 0x00,
    SHOST_RET_LIB_ERR,
    SHOST_RET_HW_ERR,
    SHOST_RET_NAK,
    SHOST_RET__MAX__
} shost_ret_t;

typedef enum {
    XFER_INTF_NONE = 0x00,
    XFER_INTF_SPI,
    XFER_INTF_I2C,
    XFER_INTF__MAX__
} shost_xfer_intf_t;

typedef enum {
    XFER_TYPE_NONE = 0x00,
    XFER_TYPE_WRITE,
    XFER_TYPE_READ,
    XFER_TYPE_READ_WRITE,
    XFER_TYPE__MAX__
} shost_xfer_type_t;

typedef struct {
    int clk;
    int channel;
    int tx_len;
    int rx_len;
    int bytesTranferred;
    int address;
    int _register;
    shost_xfer_intf_t intf;
    shost_xfer_type_t xferType;
    uint8_t *tx_buff;
    uint8_t *rx_buff;
} shost_xfer_t;

int shost_getConnectedDevices(bool printDevInfo);
int shost_xfer_begin(shost_xfer_t *xfer);

#endif // _XFER_H_