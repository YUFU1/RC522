#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"

#define USART2_MAX_RECV_LEN		1024				//最大接收缓存字节数
#define USART2_MAX_SEND_LEN		1024				//最大发送缓存字节数

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern u16 USART2_RX_STA;   											//接收数据状态
extern u8 rec_buf[1000];
extern u8 rec_sta; 
extern u16 rec_num;

void uart_init(u32 bond);
void uart_send_string(s8 *p);
void uart1_interrupt_init(u32 bond);
void usart2_init(u32 bound);
void u2_printf(char* fmt,...);

#endif
