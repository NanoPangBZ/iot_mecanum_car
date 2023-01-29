#include "stm32f1xx_hal.h"

#include "oled12864.h"
#include "debug_log.h"
#include <stdio.h>

#include "bsp_uart.h"

#include "FreeRTOS.h"
#include "task.h"

void test_task(void*parm)
{
    while(1)
    {
        INFO_LOG( __FILE__ , "as" );
        vTaskDelay( 1000 / portTICK_PERIOD_MS  );
    }
}

TaskHandle_t _handle = NULL;

void user_main()
{
    xTaskCreate( 
        test_task,
        NULL,
        1024,
        "HelloWorld",
        15,
        &_handle
     );

     vTaskStartScheduler();
}