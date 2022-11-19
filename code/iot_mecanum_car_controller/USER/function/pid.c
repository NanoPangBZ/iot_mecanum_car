/*
 * @Description: pid运算
 * @Author: 没人要的野指针
 * @Date: 2022-11-19 19:38:47
 * @LastEditors: CodingBugStd 3096470072@qq.com
 * @LastEditTime: 2022-11-19 19:51:39
 */

#include "pid.h"

/**
 * @brief 进行一次增量式PID运算
 * @param handle 要进行运算的目标PID句柄
 * @param Actual 当前被控制量实际值
 * @return 输出值
*/
float PID_IncOperation(PID_Handle*handle,float Actual)
{
    float Inc;
    handle->Err[2] = handle->Err[1];
    handle->Err[1] = handle->Err[0];
    handle->Err[0] = handle->Target - Actual;
    Inc = handle->P * (handle->Err[0] - handle->Err[1])
         + handle->I * (handle->Err[0])
         + handle->D * (handle->Err[0] - handle->Err[1]*2 + handle->Err[2]) ;
    handle->Output += Inc * handle->out_zoom;
    if(handle->Output > handle->OutputMax)
        handle->Output = handle->OutputMax;
    if(handle->Output < handle->OutputMin)
        handle->Output = handle->OutputMin;
    return handle->Output;
}

/**
 * @brief 进行一次位置式PID运算
 * @param handle 要进行运算的目标PID句柄
 * @param Actual 当前被控制量实际值
 * @return 输出值
*/
float PID_PosOperation(PID_Handle*handle,float Actual)
{
    handle->Err[2] = handle->Err[1];
    handle->Err[1] = handle->Target - Actual;
    handle->Err[0] += handle->Err[1];
    handle->Output = handle->P * handle->Err[1] + 
                        handle->I * handle->Err[0] + 
                        handle->D * handle->Err[2];
    handle->Output *= handle->out_zoom;
    if(handle->Output > handle->OutputMax)
        handle->Output = handle->OutputMax;
    if(handle->Output < handle->OutputMin)
        handle->Output = handle->OutputMin;
    return handle->Output;
}


