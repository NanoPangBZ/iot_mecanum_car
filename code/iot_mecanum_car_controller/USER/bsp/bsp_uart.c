/********************************************************
 * 基于CubeMX的串口收发
 * 庞碧璋 2023/1/22
 * ****************************************************/

#include "bsp_uart.h"
#include <string.h>

#include "protocol.h"

static uint8_t u1_rx[64];
static uint8_t u1_rx_len = 0;
uint8_t u3_rx[64];
static uint8_t u3_rx_len = 0;
static uint8_t u5_rx[64];
static uint8_t u5_rx_len = 0;

//覆盖weak回调
//CubeMX生成的代码不管接收(阻塞式除外)方式如何
//最后都会通过isr调用到这个函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //todo
    if( huart == &huart3 )
    {
        u3_rx_len++;
        if( u3_rx_len == 63 )
        {
            bsp_uart_receive( huart , u3_rx + 63 , 1 );
        }else{
            bsp_uart_receive( huart , u3_rx + u3_rx_len , 1 );
        }
    }else if( huart == &huart5 )
    {
        wit_protocol_machine_input( u5_rx[0] );
        bsp_uart_receive( huart , u5_rx , 1 );
        // u5_rx_len++;
        // if( u5_rx_len == 63 )
        // {
        //     bsp_uart_receive( huart , u5_rx + 63 , 1 );
        // }else{
        //     bsp_uart_receive( huart , u5_rx + u5_rx_len , 1 );
        // }
    }else if( huart == &huart1 )
    {
        u1_rx_len++;
        if( u1_rx_len == 63 )
        {
            bsp_uart_receive( huart , u1_rx + 63 , 1 );
        }else{
            bsp_uart_receive( huart , u1_rx + u1_rx_len , 1 );
        }
    }
}

//覆盖weak回调
//CubeMX生成的代码不管发送(阻塞式除外)方式如何
//最后都会通过isr调用到这个函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    //todo
}

void bsp_uart_clear_rx_buff( UART_HandleTypeDef* huart )
{

}

void bsp_uart_start_rx_buff_mode( void )
{
    bsp_uart_receive( &huart1 , u1_rx , 1 );
    bsp_uart_receive( &huart3 , u3_rx , 1 );
    bsp_uart_receive( &huart5 , u5_rx , 1 );
}

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
