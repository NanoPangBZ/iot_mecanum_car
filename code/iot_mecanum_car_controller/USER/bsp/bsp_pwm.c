/********************************************************
 * 板级支持包 电机pwm输出
 * 庞碧璋 2023/3/11
*******************************************************/

#include "bsp_pwm.h"
#include "tim.h"

__IO uint32_t* ccr_reg[4] = {
    &TIM5->CCR1,
    &TIM5->CCR2,
    &TIM5->CCR3,
    &TIM5->CCR4,
};

void bsp_pwm_init( void )
{
    for( uint8_t temp = 0 ; temp < 4; temp ++ )
    {
        *ccr_reg[ temp ] = 0;
    }
    HAL_TIM_PWM_Start( &htim5 ,  TIM_CHANNEL_1 );
    HAL_TIM_PWM_Start( &htim5 ,  TIM_CHANNEL_2 );
    HAL_TIM_PWM_Start( &htim5 ,  TIM_CHANNEL_3 );
    HAL_TIM_PWM_Start( &htim5 ,  TIM_CHANNEL_4 );
}

void bsp_pwm_out( uint8_t channel , uint16_t width )
{
    if( channel > 3 )
        return;
    *ccr_reg[ channel ] = width;
}

void bsp_pwm_all_out( uint16_t* width )
{
    for( uint8_t temp = 0 ; temp < 4; temp ++ )
    {
        *ccr_reg[ temp ] = *width;
        width++;
    }
}

uint16_t bsp_pwm_get_out( uint8_t channel )
{
    return channel < 4 ? *ccr_reg[ channel ] : 0;
}

void bsp_pwm_get_all_out( uint16_t* width_4buf )
{
    for( uint8_t temp = 0 ; temp < 4; temp ++ )
    {
        *width_4buf = *ccr_reg[ temp ];
        width_4buf++;
    }
}
