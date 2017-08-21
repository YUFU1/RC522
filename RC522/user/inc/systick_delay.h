#ifndef _SYSTICK_DELAY_H_
#define _SYSTICK_DELAY_H_

#include "stm32f4xx.h"
#include "io_bit.h"

//#define SYSTICK 168

void systick_delay_us(u32 nus);
void systick_delay_ms(u32 nms);
void systick_interrupt_init(u32 nms);

#endif


