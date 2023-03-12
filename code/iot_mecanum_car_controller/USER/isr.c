#include "stm32f1xx_hal.h"
#include "usart.h"
#include "tim.h"

// extern uint16_t encoder_count[4];

// //覆盖hal库中的weak函数
// void encoder_handler(TIM_HandleTypeDef *htim){
//     //编码器中断
//     if( htim == &htim2 ){
//         encoder_count[0] = htim->Instance->CNT / 4;
//     }else if( htim == &htim3 ){
//         encoder_count[1] = htim->Instance->CNT / 4;
//     }else if( htim == &htim4 ){
//         encoder_count[2] = htim->Instance->CNT / 4;
//     }else if( htim == &htim8 ){
//         encoder_count[3] = htim->Instance->CNT / 4;
//     }
//     //清除计数器
//     htim->Instance->CNT = 0;
// }


//覆盖weak回调
//CubeMX生成的代码不管接收(阻塞式除外)方式如何
//最后都会通过isr调用到这个函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //todo
}

//覆盖weak回调
//CubeMX生成的代码不管发送(阻塞式除外)方式如何
//最后都会通过isr调用到这个函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    //todo
}
