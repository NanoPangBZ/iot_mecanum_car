#include "user_task.h"

#include "user_def.h"
#include "bsp.h"
#include "hardware.h"
#include "alg.h"
#include "protocol.h"

#include <math.h>

TaskHandle_t test_taskHandle = NULL;

uint8_t test_motore_index = 0;
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

void uart3_con_uart5(void* param)
{
    while(1)
    {
        if( UART5->SR & (0x01<<5) )
        {
            USART3->DR = UART5->DR;
            UART5->SR &= ~(0x01<<5);
        }

        if( USART3->SR & (0x01<<5) )
        {
            UART5->DR = USART3->DR;
            USART3->SR &= ~(0x01<<5);
        }
    }
}

void show_encoder_to_oled(void* param)
{
    while(1)
    {
        OLED12864_Clear();
        OLED12864_Show_Num( 0 , 0 , bsp_encoder_get_value(0) , 1 );
        OLED12864_Show_Num( 1 , 0 , bsp_encoder_get_value(1) , 1 );
        OLED12864_Show_Num( 2 , 0 , bsp_encoder_get_value(2) , 1 );
        OLED12864_Show_Num( 3 , 0 , bsp_encoder_get_value(3) , 1 );
        OLED12864_Show_Num( 5 , 0 , HAL_GetTick()/1000 , 1 );
        HAL_Delay( 20 );
    }
}

void move_test_task( void* param )
{
    mecanum_constant_t model;
    TickType_t wake_time = xTaskGetTickCount();
    mecanum_input_t input;
    mecanum_output_t output;
    PID_Handle pid[4];
    float angle = 0.f;
    // float inc_angle = 0.1f;
    float base = 400;

    model.wheel_r = 1;
    model.x_len = 1;
    model.y_len = 1;

    for( uint8_t temp = 0 ; temp < 4 ; temp++ )
    {
        pid[temp].P = SPEED_PID_P;
        pid[temp].I = SPEED_PID_I;
        pid[temp].D = SPEED_PID_D;
        pid[temp].out_zoom = 1.f;
        pid[temp].OutputMax = 65535;
        pid[temp].OutputMin = -65535;
    }

    while(1)
    {
        OLED12864_Clear();
        output.cr_speed = 400;
        // output.x_speed = sin( angle ) * base ;
        output.y_speed = 500 ;
        mecanum_inverse_calculate( &model , &input , &output );
        for( uint8_t temp = 0 ; temp < 4 ; temp++ )
        {
            short speed = bsp_encoder_get_and_clear_value( temp );
            pid[temp].Target = input[temp];
            PID_IncOperation( &pid[temp] , speed );
            dc_motor_output( temp , pid[temp].Output );
            Vofa_Input( speed , temp );
            Vofa_Input( pid[temp].Output , temp+4 );
            OLED12864_Show_Num( temp , 0 , speed , 1 );
        }
        angle += inc_angle;
        OLED12864_Show_Num( 7 , 64 , xTaskGetTickCount()/1000 , 1 );
        Vofa_Send();
        xTaskDelayUntil( &wake_time , 20 / portTICK_PERIOD_MS );
    }
}

void sys_led_tick(void* param)
{
    // ESP32_ENABLE();
    while(1)
    {
        LED_OFF();
        vTaskDelay( 800 / portTICK_PERIOD_MS );
        LED_ON();
        OLED12864_Show_Num( 7 , 64 , xTaskGetTickCount()/1000 , 1 );
        vTaskDelay( 60 / portTICK_PERIOD_MS );
    }
}
