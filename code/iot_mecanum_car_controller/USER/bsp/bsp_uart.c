/********************************************************
 * 基于CubeMX的串口收发
 * 庞碧璋 2023/1/22
 * ****************************************************/

#include "bsp_uart.h"
#include <string.h>

#define DEBUG_LOG(...)

uint8_t bsp_uart_rx_busy_check(UART_HandleTypeDef* huart)
{
    if( (HAL_UART_GetState( huart ) & HAL_UART_STATE_BUSY_RX)  == HAL_UART_STATE_BUSY_RX )
    {
        return 1;
    }
    return 0;
}

uint8_t bsp_uart_tx_busy_check(UART_HandleTypeDef* huart)
{
    if( (HAL_UART_GetState( huart ) & HAL_UART_STATE_BUSY_TX)  == HAL_UART_STATE_BUSY_TX )
    {
        return 1;
    }
    return 0;
}

int bsp_uart_send( UART_HandleTypeDef* huart , uint8_t* data , uint16_t len  )
{
    if( (HAL_UART_GetState( huart ) & HAL_UART_STATE_BUSY_TX)  == HAL_UART_STATE_BUSY_TX )
    {
        return -1;
    }
    if( huart->hdmatx != NULL )
    {
        HAL_UART_Transmit_DMA( huart , data , len );
    }else{
        HAL_UART_Transmit_IT( huart , data , len );
    }
	return 0;
}

int bsp_uart_receive( UART_HandleTypeDef* huart , uint8_t* data , uint16_t len )
{
    if( (HAL_UART_GetState( huart ) & HAL_UART_STATE_BUSY_RX ) == HAL_UART_STATE_BUSY_RX )
    {
        return -1;
    }
	if( huart->hdmarx != NULL )
	{
		HAL_UART_Receive_DMA( huart , data , len );
	}else{
		HAL_UART_Receive_IT( huart , data , len );
	}
    return 0;
}

int bsp_uart_send_block( UART_HandleTypeDef* huart , uint8_t* data , uint16_t len , uint16_t timeout )
{
    if( (HAL_UART_GetState( huart ) & HAL_UART_STATE_BUSY_TX)  == HAL_UART_STATE_BUSY_TX )
    {
        return -1;
    }
    HAL_UART_Transmit( huart , data , len , timeout );
    return 1;
}

int bsp_uart_receive_block( UART_HandleTypeDef* huart , uint8_t* data , uint16_t len , uint16_t timeout )
{
    if( (HAL_UART_GetState( huart ) & HAL_UART_STATE_BUSY_RX ) == HAL_UART_STATE_BUSY_RX )
    {
        return -1;
    }
    HAL_UART_Receive( huart , data , len , timeout );
    return 1;
}
