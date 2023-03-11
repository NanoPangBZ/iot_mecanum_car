#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_

#include <stdint.h>

#define PIN_MASK( pin_num )             ( 0x01 << pin_num )
#define PIN_SET( gpio , pin_num )       ( gpio->ODR |= PIN_MASK(pin_num) )
#define PIN_RESET( gpio , pin_num )     ( gpio->ODR &= ~PIN_MASK(pin_num) )
#define PIN_READ( gpio , pin_num )      ( ( gpio->ODR & PIN_MASK(pin_num) ) ? 1 : 0 )
#define PIN_TOGGLE( gpio , pin_num )    PIN_READ( gpio , pin_num ) ? PIN_RESET( gpio , pin_num ) : PIN_SET( gpio , pin_num )

#define PIN_TICK_LED        0
#define PIN_MT1_DIR1        1
#define PIN_MT1_DIR2        2
#define PIN_MT2_DIR1        3
#define PIN_MT2_DIR2        4
#define PIN_MT3_DIR1        5
#define PIN_MT3_DIR2        6
#define PIN_MT4_DIR1        7
#define PIN_MT4_DIR2        8
#define PIN_ESP32C3_BOOT    9

void bsp_gpio_init();

void bsp_pin_set(  );
void bsp_pin_reset();

#endif  //_BSP_GPIO_H_

