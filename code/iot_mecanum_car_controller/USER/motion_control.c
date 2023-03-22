#include "motion_control.h"

#include "user_def.h"
#include "isr.h"
#include "bsp.h"
#include "hardware.h"
#include "alg.h"
#include "protocol.h"

#include "FreeRTOS.h"
#include "task.h"

static float programe_yaw_ofs;      //程序参考坐标系相对陀螺仪的偏移
static float target_yaw = 0;        //目标航向角 -> 程序参考坐标系
static float target_position[2];    //目标位置 -> 程序参考坐标系
static uint8_t              motion_state = 0;   //0:待机 1:机动中
static mecanum_constant_t   model = { 1 , 1 , 1 };      //脉轮运动学模型常数
static mecanum_output_t     car_target_speed = { 0 , 0 , 0 };  //小车目标运动矢量
static mecanum_output_t     car_posi_speed = { 0 , 0 , 0 };     //运动学正解实际轮子速度获得的当前小车运动矢量

static TaskHandle_t _car_speed_taskHandle = NULL;
static void car_speed_control_task( void* param )
{
    mecanum_input_t wheel_speed;
    PID_Handle pid[4];
    TickType_t time = xTaskGetTickCount();
    float program_speed[2]; //程序
    short int encoder_speed;
    
    //PID句柄初始化
    for( uint8_t temp = 0 ; temp < 4 ; temp++ )
    {
        pid[temp].P = SPEED_PID_P;
        pid[temp].I = SPEED_PID_I;
        pid[temp].D = SPEED_PID_D;
        pid[temp].out_zoom = 1;
        pid[temp].OutputMax = 65535;
        pid[temp].OutputMin = -65535;
    }

    while(1)
    {
        //通过小车目标速度逆解各轮子的目标速度
        mecanum_inverse_calculate( &model , &wheel_speed , &car_target_speed );
        for( uint8_t temp = 0; temp < 4; temp++ )
        {
            encoder_speed = bsp_encoder_get_and_clear_value(temp);
            pid[temp].Target = wheel_speed[temp];
            PID_IncOperation( &pid[temp] , encoder_speed );
            dc_motor_output( temp , pid[temp].Output );
            wheel_speed[temp] = encoder_speed;  //记录实际速度 用于开环速度积分
        }

        //通过实际轮子速度计算小车速度 对其积分 计算位移
        mecanum_positive_calculate( &model , &wheel_speed , &car_posi_speed );
        //将小车的速度矢量转化到程序参考坐标系
        //todo

        vTaskDelayUntil( &time , 20 / portTICK_PERIOD_MS );
    }
}

static TaskHandle_t _yaw_control_taskHanle = NULL;
static void yaw_control_task( void* param )
{
    TickType_t time = xTaskGetTickCount();
    PID_Handle yaw_pid = { 7.2 , 0 , 0.02 , 1.0 , 0 , 0 , { 0 , 0 , 0  } , 1000 , -1000 };
    float threshold;
    float err_yaw ;
    yaw_pid.Target = 0;
    while(1)
    {
        //将目标航向角转话为相对当前小车的角度
        err_yaw = target_yaw - ( jy901s_yaw + programe_yaw_ofs );
        if( err_yaw > 360 ) err_yaw -= 360;
        if( err_yaw < -360) err_yaw += 360;

        //机动过程中偏差大于0.5°时才开始修正航向角
        //待机状态下偏差大于2°时才开始修正航向角
        threshold = motion_state ? 0.5 : 2;
        if( err_yaw < motion_state && err_yaw > -motion_state ) err_yaw = 0;
        else    PID_IncOperation( &yaw_pid , err_yaw );

        car_target_speed.cr_speed = -yaw_pid.Output;
        vTaskDelayUntil( &time , 50 / portTICK_PERIOD_MS );
    }
}

void motion_control_start()
{
    if( _car_speed_taskHandle || _yaw_control_taskHanle ) return;

    programe_yaw_ofs = jy901s_yaw;

    xTaskCreate(
        car_speed_control_task,
        "car speed",
        128,
        NULL,
        14,
        &_car_speed_taskHandle
    );

    xTaskCreate(
        yaw_control_task,
        "yaw control",
        72,
        NULL,
        13,
        &_yaw_control_taskHanle
    );
}

float motion_get_yaw()
{
    float re = jy901s_yaw - programe_yaw_ofs;
    if( re >= 180 ) re -= 360;
    if( re <= -180 ) re += 360;
    return jy901s_yaw + programe_yaw_ofs;
}

void motion_reset_yaw()
{
    programe_yaw_ofs = jy901s_yaw;
}

void motion_control_suspend( void )
{
    if( _car_speed_taskHandle ) vTaskSuspend( _car_speed_taskHandle );
    if( _yaw_control_taskHanle ) vTaskSuspend( _yaw_control_taskHanle );
}

void motion_control_resume( void )
{
    if( _car_speed_taskHandle )
    {
        vTaskResume( _car_speed_taskHandle );
    }

    if( _yaw_control_taskHanle )
    {
        vTaskResume( _yaw_control_taskHanle );
    }
}
