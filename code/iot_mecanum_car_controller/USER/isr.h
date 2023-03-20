#ifndef _ISR_H_
#define _ISR_H_

#include <stdint.h>

extern float jy901s_yaw;

//串口1接收缓存
extern uint8_t u1_rx[64];
extern uint8_t u1_rx_len;

//串口3接收缓存
extern uint8_t u3_rx[64];
extern uint8_t u3_rx_len;

//串口5接收缓存 - 同步处理JY901S
extern uint8_t u5_rx;

//ADC值
extern uint16_t adc2_value;
extern uint16_t adc3_value;

#endif  //_ISR_H_

