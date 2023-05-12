#include "scp_trans.h"

#define PACK_HEAD_H 0xEA
#define PACK_HEAD_L 0xAF

#define PACK_HEAD_LEN       (2)
#define PACK_CONTROL_LEN    (2)
#define PACK_CMD_LEN        (1)
#define PACK_LEN_LEN        (2)
#define PACK_CRC_LEN        (2)
#define PACK_PAYLOAD_INDEX  ( PACK_HEAD_LEN + PACK_CONTROL_LEN + PACK_CMD_LEN + PACK_LEN_LEN )
#define PACK_FIXED_LEN      ( PACK_HEAD_LEN + PACK_CONTROL_LEN + PACK_CMD_LEN + PACK_LEN_LEN + PACK_CRC_LEN )

static void scp_tranc_decoder_error_handler( scp_trans_decoder_t* decoder , uint8_t cu_byte );

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
}

void scp_trans_decoder_reset( scp_trans_decoder_t* decoder )
{

}

void scp_tranc_decoder_error_handler( scp_trans_decoder_t* decoder , uint8_t cu_byte )
{
    //寻找下一个帧头

    //第二字节就出错，判断第二字节是不是帧头第一字节
    if( ( decoder->decode_state == 1) && ( cu_byte != PACK_HEAD_H ) )
    {
        scp_trans_decoder_reset( decoder );
        return;
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
            else scp_tranc_decoder_error_handler( decoder , byte );
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
            pack->payload_len <<= 8;
            decoder->decode_state++;     
            decoder->crc_cal += byte;
            break;
        case 6:
            pack->payload_len |= byte;
            decoder->decode_state++;
            decoder->crc_cal += byte;
            decoder->payload_count = 0;
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
            scp_decoder_reset( decoder );
        }else
        {
            scp_tranc_decoder_error_handler( decoder , byte );
        }
    }
}

void scp_trans_decoder_input( scp_trans_decoder_t* decoder , uint8_t* buf , uint16_t len )
{
    for( uint8_t temp = 0 ; temp < len ; temp++ )
        scp_decoder_input_byte( decoder , buf[temp] );
}
