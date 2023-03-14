/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "main";

TaskHandle_t sys_led_task_handle = NULL;

void led_init()
{
    // gpio_set_pull_mode( GPIO_NUM_7 , GPIO_PULLUP_PULLDOWN );
    gpio_reset_pin( GPIO_NUM_7 );
    gpio_set_direction( GPIO_NUM_7 , GPIO_MODE_OUTPUT );
    gpio_set_level( GPIO_NUM_7 , 1 );
}

void sys_led_task(void*param)
{
    ESP_LOGI( TAG , "sys_led_task_start" );
    while(1)
    {
        gpio_set_level( GPIO_NUM_7 , 1 );
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
        gpio_set_level( GPIO_NUM_7 , 0 );
        vTaskDelay( 100 / portTICK_PERIOD_MS );
    }
}

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    led_init();

    xTaskCreatePinnedToCore(sys_led_task,
                            "sys_led",
                            512,
                            NULL,
                            1,
                            &sys_led_task_handle,
                            tskNO_AFFINITY);

}
