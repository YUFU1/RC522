#include "stm32f4xx.h"
#include "uart.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "time.h"
#include "dma.h"

void uart_init(u32 bond)
{
	//PA
	RCC->AHB1ENR |=0x01<<0;
	//PA 9 10
	GPIOA->MODER &=~(0x03<<18 | 0x03<<20);
	GPIOA->MODER |=(0x02<<18 | 0x02<<20);       //PA 9 10复用功能模式
	GPIOA->OTYPER &=~(0x01<<9);								//PA9推挽输出
	GPIOA->OSPEEDR &=~(0x03<<18);							//PA9低速
	GPIOA->PUPDR &=~(0x03<<18 |0x03<<20);               //PA9无上下拉
	
	GPIOA->AFR[1] &= 0xFFFFF00F;							//复用功能
	GPIOA->AFR[1] |= 0x00000770;
	//串口uart时钟使能
	RCC->APB2ENR |=0x01<<4;
	//波特率
	USART1->BRR =84000000/bond;
	//16倍采样
	USART1->CR1 &=~(0x01<<15);
	//字长
	USART1->CR1 &=~(0x01<<12);
	//开接收，开发送
	USART1->CR1 |=(0x01<<2 |0x01<<3);
	//uart使能
	USART1->CR1 |=(0x01<<13);
	
}


void uart1_interrupt_init(u32 bond)
{
	//PA
	RCC->AHB1ENR |=0x01<<0;
	//PA 9 10
	GPIOA->MODER &=~(0x03<<18 | 0x03<<20);
	GPIOA->MODER |=0x02<<18 | 0x02<<20;       //PA 9 10复用功能模式
	GPIOA->OTYPER &=~(0x01<<9);								//PA9推挽输出
	GPIOA->OSPEEDR &=~(0x03<<18);							//PA9低速
	GPIOA->PUPDR &=~(0x03<<18 |0x03<<20);               //PA9无上下拉
	
	GPIOA->AFR[1] &= 0xFFFFF00F;							//复用功能
	GPIOA->AFR[1] |= 0x00000770;
	//串口uart时钟使能
	RCC->APB2ENR |=0x01<<4;
	//波特率
	USART1->BRR =84000000/bond;
	//16倍采样
	USART1->CR1 &=~(0x01<<15);
	//字长
	USART1->CR1 &=~(0x01<<12);
	//开接收，开发送
	USART1->CR1 |=(0x01<<2 |0x01<<3);
	
	//开串口接收中断使能
	USART1->CR1 |=0x01<<5;
	//计算优先级，设置优先级
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,1,0));
	//使能串口中断
	NVIC_EnableIRQ(USART1_IRQn);
	//uart使能
	USART1->CR1 |=(0x01<<13);
}

u8 rec_buf[1000];
u16 rec_num=0;
u8 rec_sta=0;     //数据接收完成标志
void USART1_IRQHandler(void)
{
	u8 read;
	if((rec_sta==0) && (USART1->SR & (0x01<<5)))		//是否接受到数据
	{
		SysTick->CTRL &=~(0x01<<0);
		SysTick->VAL =0;
		SysTick->CTRL |=0x01<<0;
		rec_buf[rec_num++]=USART1->DR;	
	}
	else
	{
		read=USART1->DR;
	}
}


void uart_send_string(s8 *p)			//s8是char的
{
	while(*p!='\0')
	{
		while(!(USART1->SR & (0x01<<7)));
		USART1->DR=*(p++);
	}
}

//串口发送缓存区 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//发送缓冲,最大USART2_MAX_SEND_LEN字节
//串口接收缓存区 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.

u16 USART2_RX_STA=0;
//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
   	 
void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART2->SR&(1<<5))//接收到数据
	{	 
		res=USART2->DR; 			 
		if(USART2_RX_STA<USART2_MAX_RECV_LEN)		//还可以接收数据
		{
			TIM4->CNT=0;         									//计数器清空
			if(USART2_RX_STA==0) TIM4_Set(1);	 		//使能定时器4的中断 
			USART2_RX_BUF[USART2_RX_STA++]=res;		//记录接收到的值	 
		}else 
		{
			USART2_RX_BUF[USART2_RX_STA&0X3FFF]=0;
			USART2_RX_STA|=1<<15;					//强制标记接收完成
			
		} 
	}  					
}

//初始化IO 串口2
//bound:波特率	
void usart2_init(u32 bound)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
//	USART_InitTypeDef USART_InitStruct;
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE );	//GPIOA使能
	//PA9
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;				//PA3
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;			//复用
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			//输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;		//低速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;  //浮空
	GPIO_Init( GPIOA,  &GPIO_InitStruct );
	//PA10
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2;				//PA2
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//浮空
	GPIO_Init( GPIOA,  &GPIO_InitStruct );
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);


	
	RCC->APB1ENR |=0x01<<17;								//UART2使能	
	USART2->BRR =84000000/bound;							//波特率	
	USART2->CR1 &=~(0x01<<15);							//16倍采样	
	USART2->CR1 &=~(0x01<<12);							//字长	
	USART2->CR1 |=(0x01<<13);								//uart2使能		
	USART2->CR1 |=(0x01<<2 |0x01<<3);				//开接收，开发送
	USART2->CR3 |=0x01<<7;									//DMA发送使能	
	UART_DMA_Config((u32)&USART2->DR,(u32)USART2_TX_BUF);
#ifdef USART2_RX_EN		  									//如果使能了接收
	USART2->CR1|=1<<8;    									//PE中断使能	
	USART2->CR1 |=0x01<<5;									//开串口接收中断使能
	NVIC_SetPriority(USART2_IRQn,NVIC_EncodePriority(7-2,1,0));//使能串口中断	
	NVIC_EnableIRQ(USART2_IRQn);
	TIM4_Init(999,7199);	//100ms中断
	USART2_RX_STA=0;		//清零
	TIM4_Set(1);			//关闭定时器4	
#endif	
}

//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	while(DMA1_Stream6->CR & 0x01<<0);	//等待数据流结束	
	UART_DMA_Enable(strlen((const char*)USART2_TX_BUF)); 	//通过dma发送出去
}



#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
		while((USART1->SR &(0X01<<7))==0 );
		USART1->DR=ch;
		return (ch);
}
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}


