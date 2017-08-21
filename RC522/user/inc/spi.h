#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f4xx.h" 
#include "io_bit.h"

#define MOSI 	PBout(5)
#define MISO 	PBin(4)
//#define CS  	PBout(14)
#define SCK 	PBout(3)

void SPI1_Init(void);
u8 SPI1_ReadWriteByte(u8 _data);
u8 SPI1_Read_WriteByte(u8 TxData);

#endif





