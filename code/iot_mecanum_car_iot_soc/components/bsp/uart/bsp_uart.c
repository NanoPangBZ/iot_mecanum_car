#include "bsp_uart.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#define RX_BUF_SIZE 1024

#define TXD_PIN (GPIO_NUM_6)
#define RXD_PIN (GPIO_NUM_5)

void bsp_uart_init( void )
{
        const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int bsp_uart_send(uint8_t* data , uint16_t len)
{
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    return txBytes;
}

int bsp_uart_recieve( uint8_t* data , uint16_t len , uint16_t timeout )
{
    return uart_read_bytes( UART_NUM_1, data , RX_BUF_SIZE, timeout / portTICK_PERIOD_MS);
}
