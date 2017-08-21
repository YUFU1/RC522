#include "systick_delay.h"
#include "stm32f4xx.h"
#include "led.h"
#include "uart.h"

//u8 key=key_no;

//void systick_delay_us(u32 nus)
//{
//	u32 sta;
//	//选择时钟源
//	SysTick->CTRL &=~(0x01<<2);
//	//设置load的值
//	SysTick->LOAD =nus*SYSTICK/8;
//	//对val写操作
//	SysTick->VAL =0;
//	//开启时钟
//	SysTick->CTRL |=0x01<<0;
//	//等待定时时间到（查询CTRL.16是否为1）
//	do
//	{
//		sta =SysTick->CTRL;
//	}while((sta & 0x01) && !(sta & (0x01<<16)));
//	//关时钟
//	SysTick->CTRL &=~(0x01<<0);
//}


//void systick_delay_ms(u32 nms)
//{
//	u32 sta;
//	//选择时钟源
//	SysTick->CTRL &=~(0x01<<2);
//	//设置load的值
//	SysTick->LOAD =nms*1000*SYSTICK/8;
//	//对val写操作
//	SysTick->VAL =0;
//	//开启时钟
//	SysTick->CTRL |=0x01<<0;
//	//等待定时时间到（查询CTRL.16是否为1）
//	do
//	{
//		sta =SysTick->CTRL;
//	}while((sta & 0x01) && !(sta & (0x01<<16)));
//	//关时钟
//	SysTick->CTRL &=~(0x01<<0);
//}


//void systick_interrupt_init(u32 nms)
//{
//	//选择时钟源
//	SysTick->CTRL &=~(0x01<<2);
//	//设置load的值
//	SysTick->LOAD =nms*1000*SYSTICK/8;
//	//对val写操作
//	SysTick->VAL =0;
//	//开启中断
//	SysTick->CTRL |=0x01<<1;
//	//设置优先级，计算优先级
//	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(7-2,1,0));
//	//开启systick
//	//SysTick->CTRL |=0x01<<0;
//}


//void SysTick_Handler(void)
//{
//	SysTick->CTRL &=~(0x01<<0);
//	rec_sta=1;
//	rec_buf[rec_num]='\0';
//	rec_num=0;
//	
//}


void systick_delay_us(u32 nus)
{
	//设定重装载值
	if(nus>1)
	TIM7->ARR =nus-1;
	else
	TIM7->ARR =1;
	//设定预分频
	TIM7->PSC =83;
	//清cnt
	TIM7->CNT =0;
	//设置UG位产生更新事件
	TIM7->EGR |=0x01;
	//清更新标志
	TIM7->SR &=~(0x01<<0);
	//开定时器	
	TIM7->CR1 |=0X01<<0; 
	while(!(TIM7->SR & 0x01));  //等待定时时间到
	TIM7->SR &=~(0x01<<0);
	TIM7->CR1 &=~(0X01);
}


void systick_delay_ms(u32 nms)
{
		//设定重装载值
	TIM7->ARR =nms*10-1;
	//设定预分频
	TIM7->PSC =8399;
	//清cnt
	TIM7->CNT =0;
	//设置UG位产生更新事件
	TIM7->EGR |=0x01;
	//清更新标志
	TIM7->SR &=~(0x01<<0);
	//开定时器	
	TIM7->CR1 |=0X01<<0; 
	while(!(TIM7->SR & 0x01));  //等待定时时间到
	TIM7->SR &=~(0x01<<0);
	TIM7->CR1 &=~(0X01);
}


