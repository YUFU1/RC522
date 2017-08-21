#ifndef _TIME_H_
#define _TIME_H_
#include "stm32f4xx.h"

extern u8 interrupt_key;


void time6_key_scanf_init(void);
void time7_delay_init(void);
void time10_pwm_led_init(void);
void time12_pwm_LCD_init(void);
void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);

#endif

