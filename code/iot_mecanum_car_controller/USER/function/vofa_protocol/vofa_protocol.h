#ifndef _VOFA_PROTOCOL_H_
#define _VOFA_PROTOCOL_H_

#include <stdint.h>

typedef enum
{
	COM = 0,
	USB_CDC = 1,
	TCP = 2,
	UDP = 3
}vofa_way_t;

void vofa_buffer_init(uint8_t channel_num);
void vofa_channel_set(uint8_t channel , float value);
void vofa_updata(vofa_way_t way);

#endif  //_VOFA_PROTOCOL_H_

