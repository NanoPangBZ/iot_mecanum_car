/********************************************************
 * 基于CubeMX的串口收发
 * 庞碧璋 2023/1/22
 * ****************************************************/

#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include "usart.h"

/* 异步调用 */
int bsp_uart_send( UART_HandleTypeDef* huart , uint8_t* data , uint16_t len  );
int bsp_uart_receive( UART_HandleTypeDef* huart , uint8_t* data , uint16_t len );

/* 同步调用 */
int bsp_uart_send_block( UART_HandleTypeDef* huart , uint8_t* data , uint16_t len , uint16_t timeout );
int bsp_uart_receive_block( UART_HandleTypeDef* huart , uint8_t* data , uint16_t len , uint16_t timeout );

#if 0
/* 缓存式收发 */
int bsp_uart_set_tx_sbuffer( UART_HandleTypeDef* huart , uint8_t* data , uint16_t len);
int bsp_uart_sbuffer_tx_start();
int bsp_uart_sbuffer_tx_stop();
int bsp_uart_set_rx_sbuffer( UART_HandleTypeDef* huart , uint8_t sbuff , uint16_t* len , uint16_t buf_len );
int bsp_uart_sbuffer_rx_start();
int bsp_uart_sbuffer_rx_stop();
#endif

#endif  //_BSP_UART_H_
