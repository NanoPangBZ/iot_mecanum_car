/**
 * @brief 简单通讯协议传输层
 * @anchor 庞
*/

#ifndef _SCP_TRANS_H_
#define _SCP_TRANS_H_

#include <stdint.h>
#include "scp_trans_dev.h"

#ifdef __cplusplis
extern "C"
{
#endif  //__cplusplis

typedef struct scp_buf_t;

typedef struct{
    scp_device_bit_t target_device;
    scp_device_bit_t sourse_device;
    uint8_t need_ack_bit : 1;
    uint8_t ack_code : 4;
    uint8_t cmd_word;
    uint8_t payload_len;
    scp_buf_t payload;
    uint8_t crc16[2];
}scp_pack_t;

typedef int (*scp_decoder_cb)( scp_pack_t pack );

typedef struct{
    scp_pack_t pack;
    uint16_t decode_use_len;
    scp_buf_t decode_buf;
    scp_decoder_cb cb;
}scp_decoder_t;

scp_decoder_t scp_decoder_create( scp_pack_t* pack , uint8_t* decoder_buf , uint16_t buf_len , scp_decoder_cb cb );
scp_pack_t scp_pack_create( uint8_t* buf_addr , uint16_t buf_len );

void scp_pack_send( scp_pack_t* pack );

#ifdef __cplusplis
}
#endif  //__cplusplis


#endif  //_SCP_TRANS_H_

