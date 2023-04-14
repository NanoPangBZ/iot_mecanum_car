#include "bsp_beep.h"
#include "tim.h"

void bsp_beep_init()
{
    BEEP_TIM.Instance->CCR1 = 0 ;
    HAL_TIM_PWM_Start( &BEEP_TIM , TIM_CHANNEL_1 );
}

void bsp_beep_on(uint16_t fre)
{
    BEEP_TIM.Instance->CCR1 = 72000000 / fre ;
}

void bsp_beep_off()
{
    BEEP_TIM.Instance->CCR1 = 0;
}
