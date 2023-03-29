#include "SysLed.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SysLedGpio  GPIO_NUM_7

static TaskHandle_t _ledTaskHandle = NULL;
SysLed SysLed::sysLed;

void SysLed::ledTask( void* param )
{
    while(1)
    {
        gpio_set_level( SysLedGpio , 0 );
        vTaskDelay( SysLed::sysLed._onMs / portTICK_PERIOD_MS );
        gpio_set_level( SysLedGpio , 1 );
        vTaskDelay( SysLed::sysLed._offMs / portTICK_PERIOD_MS );
    }
}

void SysLed::start()
{
    gpio_reset_pin( SysLedGpio );
    gpio_set_direction( SysLedGpio , GPIO_MODE_OUTPUT );
    if( !_ledTaskHandle )
    {
        xTaskCreatePinnedToCore(
            ledTask,
            "sys_led",
            512,
            NULL,
            1,
            &_ledTaskHandle,
            tskNO_AFFINITY
        );
    }
}

void SysLed::stop()
{
    if( _ledTaskHandle )
    {
        vTaskDelete( _ledTaskHandle );
        _ledTaskHandle = NULL;
    }
}

void SysLed::setFre( float fre , uint8_t onMs )
{
    _onMs = onMs;
    _offMs = 1000 / fre - onMs;
}
