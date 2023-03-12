#include <stdio.h>
#include <math.h>
#include "stm32f1xx_hal.h"

#include "bsp.h"
#include "user_task.h"

#include "pid/pid.h"

#include "oled12864.h"

#include "vofa_protocol/vofa_protocol.h"

PID_Handle pid;


void show_encoder_to_oled()
{
    // chargeMode();
	bsp_init();

    OLED12864_Init();

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

void user_main()
{
    // show_encoder_to_oled();
	bsp_init();

    pid.P = 28;
    pid.I = 22;
    pid.D = 18;
    pid.out_zoom = 1.f;
    pid.OutputMax = 65535;
    pid.OutputMin = -65535;
    pid.Target = 1500;

    xTaskCreate(
        speed_pid_test_task,
        "test",
        128 , 
        &pid ,
        15 ,
        &speed_pid_test_taskHandle
    );

	vTaskStartScheduler();
	
    while(1);
}