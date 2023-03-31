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

#include "bsp.h"
#include "tcp_client.h"

#define TAG "main"

extern "C" void app_main(void)
{
    ESP_LOGI( TAG , "App run." );
    bsp_init();
    tcp_client_connect( inet_addr("192.168.1.113") , htons(12341) );
    while(1)
    {
        tcp_client_send( (uint8_t*)"HelloWorld!" , strlen("HelloWorld!") + 1  );
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}
