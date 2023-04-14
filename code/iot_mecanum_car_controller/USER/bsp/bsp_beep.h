#ifndef _BSP_BEEP_H_
#define _BSP_BEEP_H_

#include <stdint.h>

void bsp_beep_init( void );
void bsp_beep_on( uint16_t fre );
void bsp_beep_off( void );

#endif  //_BSP_BEEP_H_

