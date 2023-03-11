#include "bsp_encoder.h"
#include "tim.h"

__IO uint32_t* cnt[4] = {
    &TIM2->CNT,
    &TIM3->CNT,
    &TIM4->CNT,
    &TIM8->CNT
};

void bsp_encoder_init( void )
{
    HAL_TIM_Encoder_Start( &htim2 , TIM_CHANNEL_ALL );
    HAL_TIM_Encoder_Start( &htim3 , TIM_CHANNEL_ALL );
    HAL_TIM_Encoder_Start( &htim4 , TIM_CHANNEL_ALL );
    HAL_TIM_Encoder_Start( &htim8 , TIM_CHANNEL_ALL );
}

short bsp_encoder_get_value( uint8_t channel )
{
    if( channel > 3 )
        return 0;
        
    short re = *cnt[channel];
    return re;
}

void bsp_encoder_clear( uint8_t channel )
{
    if( channel > 3 )
        return;
    *cnt[channel] = 0;
}

short bsp_encoder_get_and_clear_value( uint8_t channel )
{
    if( channel > 3 )
        return 0;

    short re = *cnt[channel];
    *cnt[channel] = 0;
    return re;
}

