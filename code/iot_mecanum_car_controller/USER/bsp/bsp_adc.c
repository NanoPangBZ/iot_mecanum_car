#include "bsp_adc.h"
#include "adc.h"

void bsp_adc_init()
{
    HAL_ADC_Start_IT( &hadc3 );
}

uint16_t bsp_adc_meansure( ADC_HandleTypeDef* hadc )
{
    while( HAL_ADC_GetState( hadc ) == HAL_BUSY );
    HAL_ADC_Start( hadc );
    while( HAL_ADC_GetState( hadc ) == HAL_BUSY );
    return HAL_ADC_GetValue( hadc );
}

