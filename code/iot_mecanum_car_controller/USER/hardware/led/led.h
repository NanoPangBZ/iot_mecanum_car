/*
 * @Date: 2022-11-20 21:19:06
 * @LastEditors: 没人要的野指针
 * @LastEditTime: 2022-11-20 21:20:07
 * @Description: led灯
 */
#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>

typedef enum
{
	TICK_LED = 0,
	DEBUG_LED = 1
}led_t;

void led(led_t led,uint8_t onoff);

#endif  //_LED_H_

