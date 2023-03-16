#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stm32f1xx_hal.h"

#include "bsp.h"
#include "hardware.h"
#include "alg.h"
#include "protocol.h"

#include "user_task.h"

float yaw;

void user_main()
{
	bsp_init();

    OLED12864_Init();

    PIN_SET( PIN_ESP32_BOOT_INDEX );
    HAL_Delay(50);
    PIN_SET( PIN_ESP32_ENABLE_INDEX );

    while(1)
    {
        OLED12864_Clear_Page(0);
        OLED12864_Show_Num( 0 , 0 , yaw , 1 );
        HAL_Delay( 100 );
    }    
}