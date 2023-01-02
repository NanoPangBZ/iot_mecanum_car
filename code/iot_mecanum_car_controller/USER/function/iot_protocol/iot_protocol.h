/*
 * @Date: 2022-11-20 21:37:06
 * @LastEditors: 没人要的野指针 https://github.com/CodingBugStd
 * @LastEditTime: 2022-12-19 21:55:18
 * @Description: 自定义物联网协议
 */
#ifndef _IOT_PROTOCOL_H_
#define _IOT_PROTOCOL_H_

#include <stdbool.h>

bool iot_send_msg(char *msg);

#endif  //_IOT_PROTOCOL_H_
