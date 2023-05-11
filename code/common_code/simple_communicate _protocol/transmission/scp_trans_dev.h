#ifndef _SCP_TRANS_DEV_H_
#define _SCP_TRANS_DEV_H_

typedef enum{
    MICRO_CONTROLLER = (0x01<<0),
    IOT_SLAVE = (0x01<<1),
    CLIENT = (0x01<<2),
    DEBUG_PORT = (0x01<<3),
    RESERVE0 = (0x01<<4),
    RESERVE1 = (0x01<<5),
    RESERVE2 = (0x01<<6),
    RESERVE3 = (0x01<<7),
}scp_device_bit_t;

#endif  //_SCP_TRANS_DEV_H_

