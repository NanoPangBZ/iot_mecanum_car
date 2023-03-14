#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_

#include "gpio.h"

typedef struct{
    GPIO_TypeDef* gpio;
    uint16_t      pin_mask;
}pin_def;

extern pin_def bsp_pins[11];

#define PIN_TICK_LED_INDEX  0
#define PIN_ESP32_ENABLE_INDEX  9
#define PIN_ESP32_BOOT_INDEX    10

#define PIN_SET( pin_index )            ( bsp_pins[pin_index].gpio->ODR |= bsp_pins[pin_index].pin_mask )
#define PIN_RESET( pin_index )          ( bsp_pins[pin_index].gpio->ODR &= ~bsp_pins[pin_index].pin_mask )
#define PIN_READ( pin_index )           ( ( bsp_pins[pin_index].gpio->ODR & bsp_pins[pin_index].pin_mask ) ? 1 : 0 )
#define PIN_TOGGLE( pin_index )         PIN_READ( pin_index ) ? PIN_RESET( pin_index ) : PIN_SET( pin_index )

#define LED_ON()    PIN_RESET( PIN_TICK_LED_INDEX )
#define LED_OFF()   PIN_SET( PIN_TICK_LED_INDEX )

void bsp_gpio_init();
//设置电机方向
void bsp_pin_mt_dir_set( uint8_t motor_index , uint8_t dir );

#endif  //_BSP_GPIO_H_

