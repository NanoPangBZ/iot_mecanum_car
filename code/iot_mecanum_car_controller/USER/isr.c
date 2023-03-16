#include "stm32f1xx_hal.h"

#include "bsp.h"
#include "protocol.h"

extern float yaw;
void wit_protocol_respond( uint8_t type , uint16_t* t_4_uint16 )
{
    if( type == 0x53 )
    {
        yaw = t_4_uint16[2] / 32768.0f * 180.0f;
    }
}

// //覆盖weak回调
// //CubeMX生成的代码不管接收(阻塞式除外)方式如何
// //最后都会通过isr调用到这个函数
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     //jy901s中断处理
//     if( huart == &huart5 )
//     {

//     }
// }

// //覆盖weak回调
// //CubeMX生成的代码不管发送(阻塞式除外)方式如何
// //最后都会通过isr调用到这个函数
// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
// {
//     //todo
// }
