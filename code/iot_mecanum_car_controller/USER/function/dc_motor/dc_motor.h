/*
 * @Date: 2022-11-20 23:27:29
 * @LastEditors: 没人要的野指针 https://github.com/CodingBugStd
 * @LastEditTime: 2022-11-20 23:28:09
 * @Description: 直流电机
 */
#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include <stdint.h>

void dc_motor_output(uint8_t motor_index,int out);
void dc_motors_output(int *buff_4int);

int dc_motor_get_output(uint8_t *motor_index);
void dc_motors_get_output(int *buff_4init);

void dc_motor_brake(uint8_t motor_index);
void dc_motors_brake(void);

#endif  //_DC_MOTOR_H_
