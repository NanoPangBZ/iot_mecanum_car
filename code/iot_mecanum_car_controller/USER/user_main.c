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

    ESP32_ENABLE();

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