#ifndef _WIT_PROTOCOL_H_
#define _WIT_PROTOCOL_H_

//不支持多设备

#include <stdint.h>

void wit_protocol_machine_reset( void );
void wit_protocol_machine( uint8_t byte );

#endif  //_WIT_PROTOCOL_H_

