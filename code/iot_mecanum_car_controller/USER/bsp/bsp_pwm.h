/********************************************************
 * 板级支持包 电机pwm输出
 * 庞碧璋 2023/3/11
*******************************************************/

#ifndef _BSP_PWM_H_
#define _BSP_PWM_H_

#include <stdint.h>

#define MT1_PWM_CHANNLE 0
#define MT2_PWM_CHANNLE 1
#define MT3_PWM_CHANNLE 2
#define MT4_PWM_CHANNLE 3

void bsp_pwm_init( void );
void bsp_pwm_out( uint8_t channel , uint16_t width );
void bsp_pwm_all_out( uint16_t* width_4buf );
uint16_t bsp_pwm_get_out( uint8_t channel );
void bsp_pwm_get_all_out( uint16_t* width_4buf );

#endif  //_BSP_PWM_H_

