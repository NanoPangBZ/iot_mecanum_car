#include "motion_control.h"

#include "user_def.h"
#include "isr.h"
#include "bsp.h"
#include "hardware.h"
#include "alg.h"
#include "protocol.h"

#include "FreeRTOS.h"
#include "task.h"

#include <math.h>

#define WHELL_SPEED_MIN 100
#define WHELL_SPEED_MAX 3000
#define PI  3.1415926

static float programe_yaw_ofs;      //程序参考坐标系相对陀螺仪的偏移
static float target_yaw = 0;        //目标航向角 -> 程序参考坐标系
static float target_position[2];    //目标位置 -> 程序参考坐标系
static float car_position[2];       //当前位置 -> 程序参考坐标系
static uint8_t              motion_state = 0;   //0:待机 1:机动中
static mecanum_constant_t   model = { 1 , 1 , 1 };      //脉轮运动学模型常数
static mecanum_center_speed_t     car_target_speed = { 0 , 0 , 0 };  //小车目标运动矢量
static mecanum_center_speed_t     car_posi_speed = { 0 , 0 , 0 };     //运动学正解实际轮子速度获得的当前小车运动矢量
static motion_control_function_t  motion_control_function = 0;

//小车速度控制线程 -> 最终实现控制的线程
static TaskHandle_t _car_speed_control_taskHandle = NULL;
static void car_speed_control_task( void* param );

//小车航目标速度设置线程
static TaskHandle_t _target_speed_set_taskHanle = NULL;
static void _target_speed_set_task( void* param );

void car_speed_control_task( void* param )
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
            //获取当前速度
            encoder_speed = bsp_encoder_get_and_clear_value(temp);

            if( wheel_speed[temp] < WHELL_SPEED_MIN && wheel_speed[temp] > -WHELL_SPEED_MIN )
            {
                pid[temp].Target = 0;
                dc_motor_output( temp , 0 );
            }else
            {
                pid[temp].Target = wheel_speed[temp];
                PID_IncOperation( &pid[temp] , encoder_speed );
                dc_motor_output( temp , pid[temp].Output );
            }
            
            wheel_speed[temp] = encoder_speed;  //记录实际速度 用于开环速度积分
        }

        //通过实际轮子速度计算小车速度 对其积分 计算位移
        // mecanum_positive_calculate( &model , &wheel_speed , &car_posi_speed );
        //将小车的速度矢量转化到程序参考坐标系
        //todo

        vTaskDelayUntil( &time , 20 / portTICK_PERIOD_MS );
    }
}

void _target_speed_set_task( void* param )
{
    TickType_t time = xTaskGetTickCount();
    PID_Handle yaw_pid = { 7.2 , 0.01 , 0.02 , 1.0 , 0 , 0 , { 0 , 0 , 0  } , 1000 , -1000 };
    float threshold;
    float err_yaw ;
    yaw_pid.Target = 0; //目标值始终为0
    while(1)
    {
        err_yaw = target_yaw - motion_get_yaw();
        //机动过程中偏差大于0.5°时才开始修正航向角
        //待机状态下偏差大于2°时才开始修正航向角
        threshold = motion_state ? 2 : 0.5;
        if( err_yaw < motion_state && err_yaw > -motion_state ) err_yaw = 0;
        else    PID_IncOperation( &yaw_pid , err_yaw );

        car_target_speed.cr_speed = yaw_pid.Output;
        vTaskDelayUntil( &time , 50 / portTICK_PERIOD_MS );
    }
}

void motion_control_start()
{
    if( _car_speed_control_taskHandle ) return;

    motion_reset_yaw();
    motion_control_function_enable( ENABLE_ALL );

    xTaskCreate(
        car_speed_control_task,
        "car speed",
        128,
        NULL,
        14,
        &_car_speed_control_taskHandle
    );

    xTaskCreate(
        _target_speed_set_task,
        "yaw control",
        72,
        NULL,
        13,
        &_target_speed_set_taskHanle
    );
}

float motion_get_yaw()
{
    float re = jy901s_yaw + programe_yaw_ofs;
    if( re >= 180 ) re -= 360;
    if( re <= -180 ) re += 360;
    return re;
}

void motion_reset_yaw()
{
    programe_yaw_ofs = -jy901s_yaw;
    if( programe_yaw_ofs <= -180 )
        programe_yaw_ofs += 360;
}

void motion_control_suspend( void )
{
    if( _car_speed_control_taskHandle ) vTaskSuspend( _car_speed_control_taskHandle );
    if( _target_speed_set_taskHanle ) vTaskSuspend( _target_speed_set_taskHanle );
}

void motion_control_resume( void )
{
    if( _car_speed_control_taskHandle )
    {
        vTaskResume( _car_speed_control_taskHandle );
    }
}

void motion_set_target_position( float x , float y , position_reference_t ref )
{
    if( ref == SOFT_REF)
    {
        target_position[0] = x;
        target_position[1] = y;
    }else if( ref == CAR_REF )
    {
        target_position[0] = ( x - car_position[0] ) * cos( motion_get_yaw() * ( PI / 180 ) ) ;
        target_position[1] = ( x - car_position[1] ) * sin( motion_get_yaw() * ( PI / 180 ) ) ;
    }
}

void motion_reset_position( void )
{
    target_position[0] += car_position[0];
    target_position[1] += car_position[1];
    car_position[0] = car_position[1] = 0;
}

void motion_get_target_position( float* x , float*y , position_reference_t ref )
{
    if( ref == SOFT_REF )
    {
        *x = target_position[0];
        *y = target_position[1];
    }else if( ref == CAR_REF )
    {
        *x = ( target_position[0] - car_position[0] ) * cos( motion_get_yaw() * ( PI / 180 ) );
        *y = ( target_position[1] - car_position[1] ) * sin( motion_get_yaw() * ( PI / 180 ) );
    }
}

void motion_set_target_yaw( float yaw )
{
    target_yaw = yaw;
}

void motion_control_function_enable( motion_control_function_t function )
{
    motion_control_function |= function;
}

void motion_control_function_disable( motion_control_function_t function )
{
    motion_control_function &= ~function;
}

motion_control_function_t motion_get_control_function( void )
{
    return motion_control_function;
}

void motion_get_position( float* x , float* y , position_reference_t ref)
{
    *x = target_position[0];
    *y = target_position[1];
}
