#include "motion_control.h"

#include "user_def.h"
#include "bsp.h"
#include "hardware.h"
#include "alg.h"

#include "FreeRTOS.h"
#include "task.h"

extern float yaw;
static mecanum_constant_t   model = { 1 , 1 , 1 };
static mecanum_output_t     car_speed = { 0 , 0 , 0 };

static TaskHandle_t _car_speed_taskHandle = NULL;
static void car_speed_control_task( void* param )
{
    mecanum_input_t wheel_speed;
    PID_Handle pid[4];
    TickType_t time = xTaskGetTickCount();

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
        mecanum_inverse_calculate( &model , &wheel_speed , &car_speed );
        for( uint8_t temp = 0; temp < 4; temp++ )
        {
            pid[temp].Target = wheel_speed[temp];
            PID_IncOperation( &pid[temp] , bsp_encoder_get_and_clear_value(temp) );
            dc_motor_output( temp , pid[temp].Output );
        }
        vTaskDelayUntil( &time , 20 / portTICK_PERIOD_MS );
    }
}

void motion_control_start()
{
    
}

