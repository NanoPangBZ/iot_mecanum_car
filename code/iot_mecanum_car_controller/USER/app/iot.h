#ifndef _IOT_H_
#define _IOT_H_

#include <stdint.h>
#include "protocol.h"

void iot_start(void);
void iot_port_input( uint8_t byte );
int iot_send( scp_pack_t* pack );

#endif  //_IOT_H_

