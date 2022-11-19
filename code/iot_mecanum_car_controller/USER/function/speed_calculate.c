/*
 * @Description: 计算速度
 * @Author: 没人要的野指针 https://github.com/CodingBugStd
 * @Date: 2022-11-19 16:56:53
 * @LastEditTime: 2022-11-19 19:41:58
 */

#include "speed_calculate.h"
#include "tim.h"

static const float rio = (6000.f/ENCODER_CYCLE_MS) / (float)REDUCTION_RATIO / (float)ENCODER_LINE;      // k * ( 1 / ENCODER_LINE ) * (6000/ENCODER_CYCLE_MS) / REDUCTION_RATIO
uint16_t encoder_count[4] = {0};  //当前计数周期脉冲个数 -> 计算过倍频后的真实值

void speed_calculate(speed_t* speed){
    if( !speed ){
        return;
    }
    short buf = 0;
    for(uint8_t temp=0;temp<4;temp++){
        buf = *(short*)(&encoder_count[temp]);
        (*speed)[temp] =  (float)buf * rio;
    }
}



