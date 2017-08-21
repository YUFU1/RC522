#ifndef _KEY_H_
#define _KEY_H_


#include "stm32f4xx.h"
#include "io_bit.h"

#define key_no 0xff
#define key1_ok 1
#define key2_ok 2
#define key3_ok 3
#define key4_ok 4
#define key1 PAin(0)
#define key2 PEin(2)
#define key3 PEin(3)
#define key4 PEin(4)
extern u8 key;

void key_init(void);
u8 key_scanf(void);


#endif
