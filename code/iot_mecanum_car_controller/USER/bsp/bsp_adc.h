#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

#include "adc.h"

void bsp_adc_init( void );
uint16_t bsp_adc_meansure( ADC_HandleTypeDef* hadc );

#endif  //_BSP_ADC_H_

