#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include <stdint.h>

void dc_motor_out( uint8_t index , int value );
int dc_motor_get_out( uint8_t index );

#endif  //_DC_MOTOR_H_

