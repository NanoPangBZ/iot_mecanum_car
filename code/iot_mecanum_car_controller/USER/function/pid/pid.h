/*
 * @Description: pid运算
 * @Author: 没人要的野指针
 * @Date: 2022-11-19 19:38:47
 * @LastEditors: CodingBugStd 3096470072@qq.com
 * @LastEditTime: 2022-11-19 19:49:27
 */
#ifndef _PID_H_
#define _PID_H_

/*************************************************
 * 增量式PID
 * Github: https://github.com/CodingBugStd
 * csdn:   https://blog.csdn.net/RampagePBZ
 * Encoding: utf-8
 * date:    2021/8/27
*************************************************/

// #ifndef USE_DOUBLE
//     #define USE_DOUBLE  0   //使用单精度浮点型
// #endif  //USE_DOUBLE

// #if USE_DOUBLE == 1
//     typedef double float;
// #else
//     typedef float  float;
// #endif

typedef struct
{
    float P;
    float I;
    float D;
    float out_zoom;    //输出缩放
    float Output;  //输出
    float Target;  //目标值
    float Err[3];  //增量式为近三次误差 位置式为 [0]误差积分 [1]本次误差 [2]上次误差
    float OutputMax;
    float OutputMin;
}PID_Handle;

float PID_IncOperation(PID_Handle*handle,float Actual);
float PID_PosOperation(PID_Handle*handle,float Actual);

#endif  //_PID_H_

