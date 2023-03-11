#ifndef _BSP_ENCODER_H_
#define _BSP_ENCODER_H_

#include <stdint.h>

void bsp_encoder_init( void );
short bsp_encoder_get_value( uint8_t channel );
void bsp_encoder_clear( uint8_t channel );

short bsp_encoder_get_and_clear_value( uint8_t channel );

#endif  //_BSP_ENCODER_H_
