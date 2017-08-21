#ifndef _DMA_H_
#define _DMA_H_


#include "stm32f4xx.h"


void dma2_init(u32 par,u16 ndtr,u32 mar);
void dma2_M_to_M_init(u32 par,u16 ndtr,u32 mar);
void dma2_transf(u16 ndtr,u32 mar);
void UART_DMA_Config(u32 cpar,u32 cmar);
void UART_DMA_Enable(u16 ndtr);

#endif


