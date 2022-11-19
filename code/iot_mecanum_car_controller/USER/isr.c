#include "stm32f1xx_hal.h"
#include "tim.h"

extern short encoder_count[4];

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if( htim == &htim2 ){
        encoder_count[0] = htim->Instance->CNT;
    }else if( htim == &htim3 ){
        encoder_count[1] = htim->Instance->CNT;
    }
}
