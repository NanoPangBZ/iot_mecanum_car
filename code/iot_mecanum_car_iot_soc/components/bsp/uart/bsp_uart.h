#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif  //__cplusplus

void bsp_uart_init( void );
int bsp_uart_send(uint8_t* data , uint16_t len);
//返回非零表示接收到的数据长度 -1表示错误  这个函数会阻塞rots线程
int bsp_uart_recieve( uint8_t* data , uint16_t len , uint16_t timeout );

#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //_BSP_UART_H_

