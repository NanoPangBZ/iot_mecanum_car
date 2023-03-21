#include "adc_keyboard.h"
#include "isr.h"

typedef struct{
    uint16_t adc_min;
    uint16_t adc_max;
    uint8_t keycode;
}keycode_threshold;

static keycode_threshold _table[5] = 
{
    { 3100 , 4000 , 0 },
    { 0 , 100 , 1 },
    { 2400 , 2800 , 2 },
    { 1700 , 2100 , 3 },
    { 1200 , 1500 , 4 }
};

uint8_t get_keycode(void)
{
    for( uint8_t temp = 0 ; temp < 5 ; temp++ )
    {
        if( adc3_value >= _table[temp].adc_min && adc3_value <= _table[temp].adc_max )
            return _table[temp].keycode;
    }
    return 0xff;
}

