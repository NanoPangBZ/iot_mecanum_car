#include "stm32f1xx_hal.h"
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
