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

__attribute__( __weak ) void wit_motion_callback(  )
{

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

            }else{

            }
            wit_motion_protocol_machine_reset();
        break;
    }
}

void wit_motion_protocol_machine_reset( void )
{
    crc_value = 0;
    buf_len = 0;
}
