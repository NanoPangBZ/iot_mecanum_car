/********************************************************
 * 基于CubeMX的串口收发
 * 庞碧璋 2023/1/22
 * ****************************************************/

#include "bsp_uart.h"
#include <string.h>

#define DEBUG_LOG(...)

//覆盖weak回调
//CubeMX生成的代码不管接收(阻塞式除外)方式如何
//最后都会通过isr调用到这个函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //todo
}

//覆盖weak回调
//CubeMX生成的代码不管发送(阻塞式除外)方式如何
//最后都会通过isr调用到这个函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    //todo
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
