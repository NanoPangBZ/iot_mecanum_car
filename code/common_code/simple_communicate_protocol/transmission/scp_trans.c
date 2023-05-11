#include "scp_trans.h"

#define PACK_HEAD_H 0xEA
#define PACK_HEAD_L 0xAF

extern int scp_send_port( uint8_t* buf , uint16_t len );

scp_decoder_t scp_decoder_create( scp_pack_t* pack , scp_decoder_cb cb )
{
    scp_decoder_t decoder;
    decoder.cb = cb;
    decoder.pack = pack;
    decoder.decode_state = 0;
    return decoder;
}

scp_pack_t scp_pack_create( uint8_t* buf , uint16_t buf_len )
{
    scp_pack_t pack;
    pack.cmd_word = 0;
    pack.control_word = 0;
    pack.payload_len = 0;
    pack.payload.buf = buf;
    pack.payload.buf_len = buf_len;
    return pack;
}

void scp_pack_send( scp_pack_t* pack )
{

}

void scp_decoder_reset( scp_decoder_t* decoder )
{

}

void scp_decoder_input_byte( scp_decoder_t* decoder , uint8_t byte )
{
    scp_pack_t* pack = decoder->pack;
    switch( decoder->decode_state )
    {
        //寻找帧头
        case 0:
            if( byte == PACK_HEAD_H )
            {
                decoder->crc_cal = PACK_HEAD_H;
                decoder->decode_state ++;
            }
            break;
        case 1:
            if( byte == PACK_HEAD_L ) decoder->decode_state++;
            else scp_decoder_reset( decoder );
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
        if( ( decoder->crc_cal == ~decoder->crc_rec ) && decoder->cb != NULL )
        {
            decoder->cb( pack );
        }
        scp_decoder_reset( decoder );
    }
}

void scp_decoder_input( scp_decoder_t* decoder , uint8_t* buf , uint16_t len )
{
    for( uint8_t temp = 0 ; temp < len ; temp++ )
        scp_decoder_input_byte( decoder , buf[temp] );
}
