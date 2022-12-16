/*
 * @Date: 2022-11-27 16:00:42
 * @LastEditors: 没人要的野指针 https://github.com/CodingBugStd
 * @LastEditTime: 2022-11-27 16:01:07
 * @Description: 
 */
#ifndef _BATTERY_H_
#define _BATTERY_H_

#include <stdint.h>
#include <stdbool.h>

//获取电量百分比
uint8_t Battety_Grade_Get();
//获取电压值
float Battery_Voltage_Get();
//当前是否在充电
bool Battery_Charge_Get();

#endif  //_BATTERY_H_

