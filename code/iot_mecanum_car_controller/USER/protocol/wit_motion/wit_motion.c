#include "wit_motion.h"

#define PROTOCOL_HEAD   0x55

static uint8_t machine_state = 0;

void wit_motion_protocol_machine( uint8_t byte )
{
    switch( machine_state )
    {
        //头捕获
        case 0:
            if( byte == PROTOCOL_HEAD )
                machine_state++;
        break;

        //数据类型字节捕获
        case 1:
            
        break;
    }
}

