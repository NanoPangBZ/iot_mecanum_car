#include <stdio.h>
#include <math.h>
#include "stm32f1xx_hal.h"

#include "bsp.h"
#include "hardware.h"
#include "alg.h"

#include "user_task.h"

void user_main()
{
	bsp_init();

    // xTaskCreate(
    //     move_test_task,
    //     "test",
    //     128 , 
    //     NULL ,
    //     15 ,
    //     &test_taskHandle
    // );

	vTaskStartScheduler();

    while(1);
}