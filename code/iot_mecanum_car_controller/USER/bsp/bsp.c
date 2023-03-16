#include "bsp.h"

void bsp_init( void )
{
    bsp_uart_start_rx_buff_mode();
    bsp_gpio_init();
    bsp_pwm_init();
    bsp_encoder_init();
}

