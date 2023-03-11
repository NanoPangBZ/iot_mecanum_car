#include "bsp_gpio.h"

//GPIO列表
pin_def bsp_pins[10] = 
{
    { GPIOC , GPIO_PIN_8 },
    { GPIOC , GPIO_PIN_2 },
    { GPIOC , GPIO_PIN_3 },
    { GPIOA , GPIO_PIN_4 },
    { GPIOA , GPIO_PIN_5 },
    { GPIOA , GPIO_PIN_6 },
    { GPIOA , GPIO_PIN_7 },
    { GPIOC , GPIO_PIN_4 },
    { GPIOC , GPIO_PIN_5 }
};

//4个电机方向控制引脚在bsp_pin数组中的索引
//正: dir1:1 dir2:0
//反: dir1:0 dir2:1
static uint8_t motor_dir1_index[4] = { 6 , 7 , 3 , 2 };
static uint8_t motor_dir2_index[4] = { 5 , 8 , 4 , 1 };

void bsp_gpio_init()
{

}

void bsp_pin_mt_dir_set( uint8_t motor_index , uint8_t dir )
{
    if( dir == 0 )
    {
        PIN_SET( motor_dir1_index[motor_index] );
        PIN_RESET( motor_dir2_index[motor_index] );
    }else{
        PIN_SET( motor_dir2_index[motor_index] );
        PIN_RESET( motor_dir1_index[motor_index] );
    }
}

uint8_t bsp_pin_read( uint8_t pin_index )
{
    return 0;
}

