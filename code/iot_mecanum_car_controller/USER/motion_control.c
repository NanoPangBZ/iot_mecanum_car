#include "motion_control.h"

#include "user_def.h"
#include "bsp.h"
#include "hardware.h"
#include "alg.h"
#include "protocol.h"

#include "FreeRTOS.h"
#include "task.h"

extern float yaw;                   //当前陀螺仪航向角
static float abs_yaw_offset;        //程序参考坐标系相对方位偏移
static float target_yaw = 0;        //目标航向角 -> 程序参考坐标系下的值
static int   target_position[2];    //目标位置 -> 程序参考坐标系
static mecanum_constant_t   model = { 1 , 1 , 1 };      //脉轮运动学模型常数
static mecanum_output_t     car_target_speed = { 0 , 0 , 0 };  //小车目标运动矢量
static mecanum_output_t     car_posi_speed = { 0 , 0 , 0 };     //运动学正解获得的当前小车运动矢量

static TaskHandle_t _car_speed_taskHandle = NULL;
static void car_speed_control_task( void* param )
{
    mecanum_input_t wheel_speed;
    PID_Handle pid[4];
    TickType_t time = xTaskGetTickCount();
    short int encoder_speed;

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
        mecanum_inverse_calculate( &model , &wheel_speed , &car_target_speed );
        for( uint8_t temp = 0; temp < 4; temp++ )
        {
            encoder_speed = bsp_encoder_get_and_clear_value(temp);
            pid[temp].Target = wheel_speed[temp];
            PID_IncOperation( &pid[temp] , encoder_speed );
            dc_motor_output( temp , pid[temp].Output );
        }
        vTaskDelayUntil( &time , 20 / portTICK_PERIOD_MS );
    }
}

static TaskHandle_t _yaw_control_taskHanle = NULL;
static void yaw_control_task( void* param )
{
    TickType_t time = xTaskGetTickCount();
    PID_Handle yaw_pid = { 7.2 , 0 , 0.02 , 1.0 , 0 , 0 , { 0 , 0 , 0  } , 1000 , -1000 };
    yaw_pid.Target = 180;
    while(1)
    {
        PID_IncOperation( &yaw_pid , yaw );
        car_target_speed.cr_speed = -yaw_pid.Output;
        vTaskDelayUntil( &time , 50 / portTICK_PERIOD_MS );
    }
}

void motion_control_start()
{
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

