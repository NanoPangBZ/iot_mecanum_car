#include "bsp.h"

void bsp_init( void )
{
    bsp_uart_init();
    bsp_adc_init();
    bsp_gpio_init();
    bsp_pwm_init();
    bsp_encoder_init();
}

