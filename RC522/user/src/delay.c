#include "delay.h"
#include "stm32f4xx.h"

void delay_us(u32 n)
{
	//设定重装载值
	if(n>1)
	TIM7->ARR =n-1;
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


void delay_ms(u32 n)
{
		//设定重装载值
	TIM7->ARR =n*10-1;
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


