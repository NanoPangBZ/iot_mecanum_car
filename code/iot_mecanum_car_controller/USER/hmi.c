#include "hmi.h"

#include "isr.h"
#include "hardware.h"

#include "motion_control.h"

#include "FreeRTOS.h"
#include "task.h"


static TaskHandle_t _keyboard_taskHandle = NULL;
static void keyboard_task( void* param )
{
    uint8_t last_keycode = 0;
    uint8_t keycode = 0;
    while(1)
    {
        keycode = get_keycode();
        if( keycode != last_keycode )
        {
            last_keycode = keycode;
            switch( keycode )
            {
                case 1:
                    motion_control_suspend();
                break;
                case 2: 
                    motion_control_resume();
                break;
                default:
                break;
            }
        }
        vTaskDelay( 20 / portTICK_PERIOD_MS );
    }
}

static TaskHandle_t _oled_taskHandle = NULL;
static void oled_task( void* param )
{
    while(1)
    {
        OLED12864_Clear_Page(0);
        OLED12864_Show_Num( 0 , 0 , motion_get_yaw() , 1 );
        OLED12864_Clear_Page(1);
        OLED12864_Show_Num( 1 , 0 , jy901s_yaw , 1 );
        vTaskDelay( 100 / portTICK_PERIOD_MS );
    }
}

void hmi_start( void )
{
    xTaskCreate( 
        keyboard_task,
        "key",
        64,
        NULL,
        configMAX_TASK_NAME_LEN - 2,
        &_keyboard_taskHandle
    );

    xTaskCreate( 
        oled_task,
        "oled",
        128,
        NULL,
        configMAX_TASK_NAME_LEN - 2,
        &_oled_taskHandle
    );
}


