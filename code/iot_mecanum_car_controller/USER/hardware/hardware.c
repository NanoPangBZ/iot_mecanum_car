#include "hardware.h"
#include "bsp.h"

static void esp32c3_init( void )
{
    PIN_SET( PIN_ESP32_BOOT_INDEX );
    HAL_Delay(50);
    PIN_SET( PIN_ESP32_ENABLE_INDEX );
}

void hardware_init(void)
{
    esp32c3_init();
    OLED12864_Init();
}

