#ifndef _WIT_PROTOCOL_H_
#define _WIT_PROTOCOL_H_

#include <stdint.h>

#define WIT_TYPE_ACC
#define WIT_TYPE_ANG
#define WIT_TYPE_MEG

void wit_protocol_machine_input( uint8_t data );

#endif		//_WIT_PROTOCOL_H_

