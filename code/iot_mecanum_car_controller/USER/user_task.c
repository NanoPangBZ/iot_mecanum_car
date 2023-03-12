#include "user_task.h"

#include "bsp.h"
#include "pid/pid.h"
#include "dc_motor/dc_motor.h"
#include "vofa_protocol/vofa_protocol.h"

#include <math.h>

uint8_t test_motore_index = 0;
TaskHandle_t speed_pid_test_taskHandle = NULL;
float inc_angle = 0.01;
float zoom = 500;
float base = 1500;
/**
 * p  i  d z
 * 28 22 18 1
**/
void speed_pid_test_task( void* param )
{
    PID_Handle* pid = (PID_Handle*)param;
    TickType_t wake_time = xTaskGetTickCount();
    short speed = 0;
    float angle = 0.f;
    while(1)
    {
        angle += inc_angle;
        pid->Target = sin( angle ) * zoom + base ;
        speed = bsp_encoder_get_and_clear_value( test_motore_index );
        PID_IncOperation( pid , speed );
        dc_motor_output( test_motore_index , pid->Output );
        Vofa_Input( speed , 0 );
        Vofa_Input( pid->Target , 1 );
        Vofa_Input( pid->Output , 2 );
        Vofa_Send();
        xTaskDelayUntil( &wake_time , 20 / portTICK_PERIOD_MS );
    }
}

