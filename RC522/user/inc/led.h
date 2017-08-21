#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"
#include "io_bit.h"

#define led1 PFout(6)
#define led2 PFout(9)
#define led3 PFout(10)
#define led4 PCout(0)


void led_init(void);

#endif

