/*
 * @Date: 2022-11-20 21:43:50
 * @LastEditors: 没人要的野指针 https://github.com/CodingBugStd
 * @LastEditTime: 2022-11-27 15:53:19
 * @Description: 通过usb cdc、tcp、udp、uart等通道向vofa+上位机发送数据，实时返回速度、位置、姿态、电量等信息
 */
#ifndef _WAVEFROM_SERVICE_H_
#define _WAVEFROM_SERVICE_H_

#include <stdint.h>

//stm32不带wifi功能，这里的tcp、udp是通过iot_service向esp32c3发送串口数据实现的数据传输

//轮子pwm输出  4 float
//轮子实际转速 4 float
//轮子目标转速 4 float
//车子目标速度 3 float x、y方向速度和z方向角速度
//车子实际速度 3 float
//车子实际位置 3 float x、y坐标以及z方向
//车子目标位置 3 float
//电量        1 float
//其他变量监控 3 float

typedef enum{
    FB_CH_USB = 0,
    FB_CH_TCP = 1,
    FB_CH_UDP = 2,
    DB_CH_UART = 3
}wavefrom_channel_t;

typedef enum{
    FB_FLOAT = 0,
    FB_DOUBLE = 1,
    FB_INT = 2
}wavefrom_variable_type_t;

uint8_t wavefrom_service_start(void);
uint8_t wavefrom_service_stop(void);

void wavefrom_set_channel(wavefrom_channel_t channel);
void wavefrom_set_fre(int hz);

void wavefrom_set_udp_host(char* host);
void wavefrom_set_tcp_host(char* tcp);
void wavefrom_set_uart_index(uint8_t index);

/* 注册自定义的监控变量 */
// void wavefrom_register_variable_monitor(void* value_addr,wavefrom_variable_type_t type);

#endif //_WAVEFROM_SERVICE_H_
