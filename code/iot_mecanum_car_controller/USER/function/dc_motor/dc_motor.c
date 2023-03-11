/*
 * @Date: 2022-11-20 23:27:22
 * @LastEditors: 没人要的野指针 https://github.com/CodingBugStd
 * @LastEditTime: 2022-11-20 23:28:11
 * @Description: 直流电机
 */
#include "dc_motor.h"
#include "bsp_gpio.h"
#include "bsp_pwm.h"

void dc_motor_output(uint8_t motor_index,int out)
{
	bsp_pin_mt_dir_set( motor_index , out > 0 ? 0 : 1 );
	bsp_pwm_out( motor_index , out > 0 ? out : -out );
}

void dc_motors_output(int *buff_4int)
{
}

int dc_motor_get_output(uint8_t *motor_index)
{
	return 0;
}

void dc_motors_get_output(int *buff_4init)
{
}

void dc_motor_brake(uint8_t motor_index)
{
}

void dc_motors_brake(void)
{
}
