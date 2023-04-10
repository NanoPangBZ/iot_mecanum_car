#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bsp.h"

#include "esp_log.h"
#define TAG "main"

extern "C" void app_main(void)
{   
    SysLed::sysLed.setFre( 1 , 200 );
    SysLed::sysLed.start();

    bsp_wifi_init();
    bsp_wifi_ap( "iot_mecanum_car" , "123456789" );

    vTaskDelay( 30000 / portTICK_PERIOD_MS );
    ESP_LOGI( TAG , "wifi ap shutdown" );
    bsp_wifi_stop();

    vTaskDelay( 500 / portTICK_PERIOD_MS );
    ESP_LOGI( TAG , "wifi connecting." );
    bsp_wifi_connect( "Redmi Note 11T Pro" , "09296666" );

    ESP_LOGI( TAG , "app running!" );
    while(1)
    {

        vTaskDelay( 100 / portTICK_PERIOD_MS );
    }
}
