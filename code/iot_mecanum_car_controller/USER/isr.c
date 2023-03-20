#include "stm32f1xx_hal.h"

#include "adc.h"

#include "bsp.h"
#include "protocol.h"

//串口1接收缓存
uint8_t u1_rx[64];
uint8_t u1_rx_len = 0;

//串口3接收缓存
uint8_t u3_rx[64];
uint8_t u3_rx_len = 0;

//串口5接收缓存 - 同步处理JY901S
uint8_t u5_rx;

//JY901S解析出的yaw值
float jy901s_yaw;

//ADC值
uint16_t adc2_value;
uint16_t adc3_value;

//覆盖wit_protocol中的weak回调
void wit_protocol_callback( uint8_t type , uint16_t* t_4_uint16 )
{
    if( type == 0x53 )
    {
        jy901s_yaw = t_4_uint16[2] / 32768.0f * 180.0f;
    }
}

//覆盖weak回调
//CubeMX生成的代码不管接收(阻塞式除外)方式如何
//最后都会通过isr调用到这个函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if( huart == &huart3 )
    {
        u3_rx_len++;
        if( u3_rx_len > 63 )
        {
            bsp_uart_receive( huart , u3_rx + 63 , 1 );
        }else{
            bsp_uart_receive( huart , u3_rx + u3_rx_len , 1 );
        }
    }else if( huart == &huart5 )
    {
        //处理JY901S数据
        wit_protocol_machine_input( u5_rx );
        bsp_uart_receive( huart , &u5_rx , 1 );
    }else if( huart == &huart1 )
    {
        u1_rx_len++;
        if( u1_rx_len > 63 )
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

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if( hadc == &hadc2 )
    {
        adc2_value = HAL_ADC_GetValue( &hadc2 );
        HAL_ADC_Start_IT( &hadc2 );
    }else{
        adc3_value = HAL_ADC_GetValue( hadc );
        HAL_ADC_Start_IT( &hadc3 );
    }
}
