#include "scp_trans.h"
#include <string.h>

//PACK_HEAD_H 必需 不等于 PACK_HEAD_L 否则错误处理无法正常运行
#define PACK_HEAD_H 0xEA
#define PACK_HEAD_L 0xAF

#define PACK_HEAD_LEN       (2)
#define PACK_CONTROL_LEN    (2)
#define PACK_CMD_LEN        (1)
#define PACK_LEN_LEN        (2)
#define PACK_CRC_LEN        (2)
#define PACK_PAYLOAD_INDEX  ( PACK_HEAD_LEN + PACK_CONTROL_LEN + PACK_CMD_LEN + PACK_LEN_LEN )
#define PACK_FIXED_LEN      ( PACK_HEAD_LEN + PACK_CONTROL_LEN + PACK_CMD_LEN + PACK_LEN_LEN + PACK_CRC_LEN )

static void scp_trans_decoder_error_handler( scp_trans_decoder_t* decoder , uint8_t cu_byte );

scp_trans_decoder_t scp_trans_decoder_create( scp_pack_t* pack , scp_trans_decoder_cb cb )
{
    scp_trans_decoder_t decoder;
    decoder.cb = cb;
    decoder.pack = pack;
    decoder.decode_state = 0;
    return decoder;
}

scp_pack_t scp_trans_pack_create( uint8_t* buf , uint16_t buf_len )
{
    scp_pack_t pack;
    pack.cmd_word = 0;
    pack.control_word = 0;
    pack.payload_len = 0;
    pack.payload.buf = buf;
    pack.payload.buf_len = buf_len;
    return pack;
}

int scp_trans_send( scp_pack_t* pack , scp_trans_port port)
{
    if( pack->payload_len + PACK_FIXED_LEN > pack->payload.buf_len )
        return -1;
    uint8_t* buf_addr = pack->payload.buf;
    uint8_t* crc_addr = buf_addr + PACK_FIXED_LEN + pack->payload_len - PACK_CRC_LEN;
    uint16_t crc_value = 0;
    memcpy( buf_addr + PACK_PAYLOAD_INDEX , buf_addr , pack->payload_len );
    buf_addr[0] = PACK_HEAD_H;
    buf_addr[1] = PACK_HEAD_L;
    buf_addr[2] = pack->control_word << 8;
    buf_addr[3] = pack->control_word;
    buf_addr[4] = pack->cmd_word;
    // buf_addr[5] = ( pack->payload_len & 0x0ff0 ) >> 4;
    buf_addr[5] = pack->payload_len >> 4;
    buf_addr[6] = pack->payload_len & 0x000f;
    buf_addr[6] |= ( buf_addr[6]<<4 );
    //计算crc值
    while( buf_addr < crc_addr )
    {
        crc_value += *buf_addr;
        buf_addr++;
    }
    crc_value = ~crc_value;
    crc_addr[0] = crc_value>>8;
    crc_addr[1] = crc_value;

    return port( pack->payload.buf , PACK_FIXED_LEN + pack->payload_len );
}

void scp_trans_decoder_reset( scp_trans_decoder_t* decoder )
{
    decoder->decode_state = 0;
}

void scp_trans_decoder_error_handler( scp_trans_decoder_t* decoder , uint8_t cu_byte )
{
    //只有状态1、7、10会产生错误
    if( decoder->decode_state == 1 )
    {
        if( cu_byte != PACK_HEAD_H )
            scp_trans_decoder_reset( decoder );
    }else if( decoder->decode_state == 7 )
    {
        //重置解码器 使用编码器在接收缓存中寻找下一个帧头
        scp_trans_decoder_reset( decoder );
        //若payload发生了截断，则无法还原接收缓存，直接返回
        if( decoder->decode_state == 10 && decoder->pack->payload_len > decoder->pack->payload.buf_len )
        { 
            return;  
        }

        //还原接收字节到缓存
        uint8_t buf[7];
        buf[0] = decoder->pack->control_word >> 8;
        buf[1] = decoder->pack->control_word;
        buf[2] = decoder->pack->cmd_word;
        buf[3] = decoder->pack->payload_len >> 4;
        if( decoder->decode_state == 7 )
            buf[4] = cu_byte;
        else
        {
            buf[4] = decoder->pack->payload_len & 0x000f ;
            buf[4] |= ( buf[4] << 4 );
            buf[5] = decoder->crc_rec << 8;
            buf[6] = cu_byte;
        }

        //重新解码 -> 这个阶段不可能对pack payload_buf进行操作！
        scp_trans_decoder_input( decoder , buf , 5 );

        if( decoder->decode_state == 10 )
        {
            scp_trans_decoder_input( decoder , decoder->pack->payload.buf , decoder->pack->payload_len );
            scp_trans_decoder_input( decoder , buf + 5 , 2 );
        }
    }
}

void scp_trans_decoder_input_byte( scp_trans_decoder_t* decoder , uint8_t byte )
{
    scp_pack_t* pack = decoder->pack;

    switch( decoder->decode_state )
    {
        //等待帧头
        case 0:
            if( byte == PACK_HEAD_H )
            {
                decoder->crc_cal = PACK_HEAD_H;
                decoder->decode_state ++;
            }
            break;
        case 1:
            if( byte == PACK_HEAD_L ) decoder->decode_state++;
            else scp_trans_decoder_error_handler( decoder , byte );
            decoder->crc_cal += byte;
            break;

        //控制字
        case 2:
            pack->control_word = byte;
            pack->control_word <<= 8;
            decoder->decode_state++;
            decoder->crc_cal += byte;
            break;
        case 3:
            pack->control_word |= byte;
            decoder->decode_state++;
            decoder->crc_cal += byte;
            break;

        //命令字
        case 4:
            pack->cmd_word = byte;
            decoder->decode_state++;
            decoder->crc_cal += byte;
            break;

        //负载长度字
        case 5:
            pack->payload_len =  byte;
            pack->payload_len <<= 4;
            decoder->decode_state++;     
            decoder->crc_cal += byte;
            break;
        case 6:
            if( ( byte>>4 ) != ( byte & 0x0f) )
            {
                scp_trans_decoder_error_handler( decoder , byte );
            }else
            {
                pack->payload_len |= ( byte & 0x0f );
                decoder->decode_state++;
                decoder->crc_cal += byte;
                decoder->payload_count = 0;
            }
            break;

        //接收负载
        case 7:
            //注意break的位置,有可能payload负载为0!!
            //状态机需要直接进入crc校验接收
            if( decoder->payload_count == pack->payload_len )
            {
                decoder->decode_state++;
            }else
            {
                //判断pack的payload缓冲区是否能装下
                if( decoder->payload_count < pack->payload.buf_len )
                    pack->payload.buf[ decoder->payload_count ] = byte;
                decoder->payload_count++;
                if( decoder->payload_count == pack->payload_len )
                    decoder->decode_state++;
                decoder->crc_cal += byte;
                break;
            }
        
        //接收crc值
        case 8:
            decoder->crc_rec = byte;
            decoder->crc_rec <<= byte;
            decoder->decode_state++;
            break;
        case 9:
            decoder->crc_rec |= byte;
            decoder->decode_state++;
            break;
    }

    if( decoder->decode_state == 10 )
    {
        if( decoder->crc_cal == ~decoder->crc_rec )
        {
            if( decoder->cb != NULL )
                decoder->cb( pack );
            scp_trans_decoder_reset( decoder );
        }else
        {
            scp_trans_decoder_error_handler( decoder , byte );
        }
    }
}

void scp_trans_decoder_input( scp_trans_decoder_t* decoder , uint8_t* buf , uint16_t len )
{
    for( uint8_t temp = 0 ; temp < len ; temp++ )
        scp_trans_decoder_input_byte( decoder , buf[temp] );
}
