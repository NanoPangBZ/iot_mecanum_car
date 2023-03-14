#include "stm32f1xx_hal.h"
#include "usart.h"
#include "tim.h"

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
