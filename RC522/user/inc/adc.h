#ifndef _ADC_H_
#define _ADC_H_
#include "stm32f4xx.h"


void adc_init(void);
void adc_temp_init(void);
u16 get_adc1_ch5(void);
u16 get_adc1_temp_ch16(void);


#endif




