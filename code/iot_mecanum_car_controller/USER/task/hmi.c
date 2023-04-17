#include "hmi.h"

#include "isr.h"
#include "bsp.h"
#include "hardware.h"

#include "motion_control.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

QueueHandle_t beep_queue = NULL;

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

static TaskHandle_t _led_taskHandle = NULL;
void sys_led_tick(void* param)
{
    while(1)
    {
        LED_OFF();
        vTaskDelay( 800 / portTICK_PERIOD_MS );
        LED_ON();
        vTaskDelay( 120 / portTICK_PERIOD_MS );
    }
}

static TaskHandle_t _beep_taskHandle = NULL;
static void beep_task( void* param )
{
    beep_notice_t notic;
    while(1)
    {
        xQueueReceive( beep_queue , &notic , -1 );
        switch( notic )
        {
            case BEEP_KEY_INPUT:
                bsp_beep_on( KEY_INPUT_BEEP_FRE );
                vTaskDelay( KEY_INPUT_BEEP_TIME / portTICK_PERIOD_MS );
                bsp_beep_off();
            break;
            case BEEP_SYS_INIT_FINNISH:
                for( uint8_t temp = 0 ; temp < SYS_INIT_BEEP_COUNT ; temp++ )
                {
                    bsp_beep_on( SYS_INIT_BEEP_FRE );
                    vTaskDelay( SYS_INIT_BEEP_TIME / portTICK_PERIOD_MS );
                    bsp_beep_off();
                    vTaskDelay( SYS_INIT_BEEP_OFF_TIME / portTICK_PERIOD_MS );
                }
            break;
            default:
            break;
        }
    }
}

void beep_notice( beep_notice_t notice )
{
    xQueueSend( beep_queue , &notice , 100 / portTICK_PERIOD_MS );
}

void hmi_start( void )
{
    beep_queue = xQueueCreate( 3 , sizeof(beep_notice_t) );

    xTaskCreate( 
        sys_led_tick ,
        "sys_led",
        32,
        NULL,
        1,
        &_led_taskHandle
    );

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

    xTaskCreate( 
        beep_task,
        "beep",
        32,
        NULL,
        3,
        &_beep_taskHandle
    );
}


