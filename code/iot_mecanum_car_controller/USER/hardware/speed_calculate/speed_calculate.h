/*
 * @Description: 计算速度
 * @Author: 没人要的野指针 https://github.com/CodingBugStd
 * @Date: 2022-11-19 16:56:53
 * @LastEditTime: 2022-11-19 21:09:15
 */
#ifndef _SPEED_MEANSURE_H_
#define _SPEED_MEANSURE_H_

//单位 r/min
typedef float wheel_speed_t[4];

#define WHEEL_RADIUS_MM     48  //半径
#define REDUCTION_RATIO     30  //减速比
#define ENCODER_LINE        500 //编码器线数
#define ENCODER_CYCLE_MS    5   //编码器计数周期

void speed_calculate(wheel_speed_t *speed);

#endif  //_SPEED_MEANSURE_H_

