/**
 * @brief 简单通讯协议传输层
 * @anchor 庞
*/

#ifndef _SCP_TRANS_H_
#define _SCP_TRANS_H_

#include <stdint.h>
#include "scp_trans_dev.h"

#ifdef __cplusplus
extern "C"
{
#endif  //__cplusplus

typedef struct{
    uint8_t* buf;
    uint16_t buf_len;
}scp_buf_t;

//scp_pack_t不可同时用于收发(编码/解码)！！
//因为payload缓存接收、发送、编码、解码共用！！
typedef struct{
    uint16_t control_word;
    uint8_t cmd_word;
    uint16_t payload_len;
    scp_buf_t payload;      //与 解码/编码 缓存共用!!
}scp_pack_t;

typedef int (*scp_trans_port)( uint8_t* buf , uint16_t len );
typedef int (*scp_trans_decoder_cb)( scp_pack_t* pack );

typedef struct{
    scp_pack_t* pack;
    uint8_t decode_state;
    scp_trans_decoder_cb cb;
    uint16_t payload_count;
    uint16_t crc_cal;   //crc计算值
    uint16_t crc_rec;   //crc接收值
}scp_trans_decoder_t;

scp_trans_decoder_t scp_trans_decoder_create( scp_pack_t* pack , scp_trans_decoder_cb cb );
scp_pack_t scp_trans_pack_create( uint8_t* buf , uint16_t buf_len );

int scp_trans_send( scp_pack_t* pack , scp_trans_port port);

void scp_trans_decoder_reset( scp_trans_decoder_t* decoder );
void scp_trans_decoder_input_byte( scp_trans_decoder_t* decoder , uint8_t byte );
void scp_trans_decoder_input( scp_trans_decoder_t* decoder , uint8_t* buf , uint16_t len );

#ifdef __cplusplus
}
#endif  //__cplusplus


#endif  //_SCP_TRANS_H_

