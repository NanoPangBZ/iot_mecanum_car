#include <string.h>
#include "wit_protocol.h"

#define PROTOCOL_HEAD       0x55
#define PROTOCOL_TYPE_ACC   0x51    //加速度
#define PROTOCOL_TYPE_PAL   0x52    //角速度
#define PROTOCOL_TYPE_ANG   0x53    //角度
#define PROTOCOL_TYPE_MAG   0x54    //磁场

static uint8_t machine_state = 0;

static uint8_t decoder_buf[8];
static uint8_t buf_len = 0;
static uint8_t crc_value = 0;

__attribute__((weak)) void wit_protocol_callback( uint8_t type_byte , const uint8_t* data_buf )
{

}

static void wit_protocol_error_handler( void )
{
    //寻找下一个HEAD
    uint8_t header = 0;
    for(  header = 0 ; ( header < 8 ) && ( decoder_buf[ header ] == PROTOCOL_HEAD ) ; header++  );
    if( decoder_buf[header] == PROTOCOL_HEAD )
        //重新解析
        for( uint8_t temp = header ; temp < 8 ; temp++ )
            wit_protocol_machine( decoder_buf[temp] );
    return;
}

void wit_protocol_machine( uint8_t byte )
{
    switch( machine_state )
    {
        //头捕获
        case 0:
            if( byte == PROTOCOL_HEAD )
            {
                crc_value = byte;
                machine_state++;
            }
        break;

        //等待完整数据 类型 + 数据
        case 1:
            decoder_buf[ buf_len ] = byte;
            crc_value += byte;
            buf_len++;
            machine_state++;
        break;
            
        //校验
        case 2:
            if( crc_value != byte )
            {
                wit_protocol_error_handler( );
            }else{
                wit_protocol_callback( decoder_buf[0] , decoder_buf + 1 );
            }
            wit_protocol_machine_reset();
        break;
    }
}

void wit_protocol_machine_reset( void )
{
    crc_value = 0;
    buf_len = 0;
}
