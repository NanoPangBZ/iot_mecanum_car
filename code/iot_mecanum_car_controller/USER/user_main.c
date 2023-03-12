#include <stdio.h>
#include <math.h>
#include "stm32f1xx_hal.h"

#include "bsp.h"
#include "user_task.h"

#include "pid/pid.h"

#include "oled12864.h"

#include "vofa_protocol/vofa_protocol.h"

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

    OLED12864_Init();

    xTaskCreate(
        move_test_task,
        "test",
        128 , 
        NULL ,
        15 ,
        &test_taskHandle
    );

	vTaskStartScheduler();
	
    while(1);
}