#include <stdio.h>
#include <math.h>
#include "stm32f1xx_hal.h"

#include "bsp.h"
#include "hardware.h"
#include "alg.h"
#include "protocol.h"

#include "user_task.h"

void user_main()
{
	bsp_init();

    OLED12864_Init();

    PIN_SET( PIN_ESP32_BOOT_INDEX );
    HAL_Delay(50);
    PIN_SET( PIN_ESP32_ENABLE_INDEX );

    xTaskCreate(
        sys_led_tick,
        "test",
        64 , 
        NULL ,
        1 ,
        &test_taskHandle
    );

	vTaskStartScheduler();

    while(1);
}