#include "vofa_protocol.h"
#include "bsp.h"

//宏定义通道数量
#define DataNum	10

typedef struct
{
	float Date[DataNum];
	unsigned char FramEnd[4];
}Frame;

static Frame Vofa_Sbuffer = {{0},{0x00,0x00,0x80,0x7f}};

void Vofa_Input(float data,unsigned char channel)
{
	if(channel < DataNum)
		Vofa_Sbuffer.Date[channel] = data;
}

void Vofa_Send(void)
{
    bsp_uart_send( &huart3 , (uint8_t*)&Vofa_Sbuffer,sizeof(Frame) );
}