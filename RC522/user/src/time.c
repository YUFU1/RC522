#include "stm32f4xx.h"
#include "key.h"
#include "led.h"
#include "time.h"
#include "uart.h"

//10ms进一次中断
void time6_key_scanf_init(void)
{
	//时钟使能
	RCC->APB1ENR |=0X01<<4;
	//设定配置寄存器
	TIM6->CR1 |=0X01<<7; //ARR有缓存
	TIM6->CR1 &=~(0x03<<1); //使能更新事件，设置UG位会产生更新事件	
	//设定重装载值
	TIM6->ARR =99;
	//设定预分频
	TIM6->PSC =8399;
	//清cnt
	TIM6->CNT =0;
	//设置UG位产生更新事件
	TIM6->EGR |=0x01;
	//清更新标志
	TIM6->SR &=~(0x01<<0);
	
	//使能中断
	TIM6->DIER |=0X01<<0;
	
	NVIC_SetPriority(TIM6_DAC_IRQn,NVIC_EncodePriority(7-2,3,3));  //设置优先级
	NVIC_EnableIRQ(TIM6_DAC_IRQn); //外部中断使能（系统中断没有这个使能）

	//开定时器	
	TIM6->CR1 |=0X01<<0; 
}


void time7_delay_init(void)
{
	
	//时钟使能
	RCC->APB1ENR |=0X01<<5;
	//设定配置寄存器
	TIM7->CR1 |=0X01<<7; //ARR有缓存
	TIM7->CR1 &=~(0x03<<1); //使能更新事件，设置UG位会产生更新事件
	
}



void time10_pwm_led_init(void)
{
	//端口时钟使能pf  6
	RCC->AHB1ENR |=0X01<<5;
	GPIOF->MODER &=~(0X03<<12);
	GPIOF->MODER |=0X02<<12;     //复用功能
	GPIOF->OTYPER &=~(0x01<<6);   //推挽输出
	GPIOF->OSPEEDR &=~(0X03<<12);  //输出最高速度2M
	GPIOF->PUPDR  &=~(0X03<<12);   //禁用上下拉
	
	GPIOF->AFR[0] &=0XF0FFFFFF;
	GPIOF->AFR[0] |=0X03000000;   //pf6为time10的ch1
	
	//时钟使能
	RCC->APB2ENR |=0X01<<17;
	
	TIM10->CR1 |=0X01<<7;
	TIM10->CR1 &=~(0X03<<1);
	
	TIM10->CCMR1 &=~(0X03);    //通道1设为输出
	TIM10->CCMR1 |=0x01<<3;    //CCR1有预装载
	
	TIM10->CCMR1 &=~(0x07<<4);
	TIM10->CCMR1 |=0X06<<4;    //pWM模式1
	
	TIM10->PSC =1679;    //CNT计数频率10k
	TIM10->ARR =999;     //计数10000次
	TIM10->CNT=0;        //计数初值
	TIM10->CCR1=0;
	
	TIM10->EGR |=0X01;  //设置UG位，更新相关寄存器
	TIM10->SR &=~(0X01);  //清更新标志
	
	TIM10->CCER |=0X01<<1;  //通道1低电平有效
	TIM10->CCER |=0X01<<0;  //开通道1
	
	TIM10->CR1 |=0X01<<0;  //开定时器
}

void time12_pwm_LCD_init(void)
{
	//端口时钟使能pb  15
	RCC->AHB1ENR |=0X01<<1;
	GPIOB->MODER &=~(0X03<<30);
	GPIOB->MODER |=0X02<<30;     //复用功能
	GPIOB->OTYPER &=~(0x01<<15);   //推挽输出
	GPIOB->OSPEEDR &=~(0X03<<30);  //输出最高速度2M
	GPIOB->PUPDR  &=~(0X03<<30);   //禁用上下拉
	
	GPIOB->AFR[1] &=0X0FFFFFFF;
	GPIOB->AFR[1] |=0X90000000;   //pb15为time12的ch2
	
	//时钟使能 time12
	RCC->APB1ENR |=0X01<<6;
	
	TIM12->CR1 |=0X01<<7;				//ARR寄存器进行缓冲
	TIM12->CR1 &=~(0X03<<1);		//更新
	
	TIM12->CCMR1 &=~(0X03<<8);    //通道2设为输出
	TIM12->CCMR1 |=0x01<<11;    		//CCR2有预装载
	
	TIM12->CCMR1 &=~(0x07<<12);
	TIM12->CCMR1 |=0X06<<12;    //pWM模式1
	
	TIM12->PSC =6719;    //CNT计数频率100k
	TIM12->ARR =249;     //计数1000次
	TIM12->CNT=0;        //计数初值
	TIM12->CCR2=10;
	
	TIM12->EGR |=0X01;  //设置UG位，更新相关寄存器
	TIM12->SR &=~(0X01);  //清更新标志
	
	TIM12->CCER &=~(0X01<<5);  //通道2  高电平有效
	TIM12->CCER |=0X01<<4;  //开通道2
	
	TIM12->CR1 |=0X01<<0;  //开定时器
}


u8 interrupt_key=key_no;

void  TIM6_DAC_IRQHandler (void)
{
	u8 key;
	TIM6->SR &=~(0x01<<0);
	key=key_scanf( );
	if(key!=key_no)
	{
		interrupt_key=key;
	}
}

//定时器4中断服务程序		    
void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X01)//是更新中断
	{	 			
    USART2_RX_BUF[USART2_RX_STA&0X3FFF]=0;		
		USART2_RX_STA|=1<<15;	//标记接收完成
		TIM4->SR&=~(1<<0);		//清除中断标志位		   
		TIM4_Set(0);			//关闭TIM4  
	}	    
}
//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
			TIM4->CR1|=1<<0;     //使能定时器4
    	TIM4->CNT=0;         //计数器清空
	}
	else TIM4->CR1&=~(1<<0);//关闭定时器4	   
}
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;	//TIM4时钟使能    
 	TIM4->ARR=arr;  	//设定计数器自动重装值   
	TIM4->PSC=psc;  	//预分频器
 	TIM4->DIER|=1<<0;   //允许更新中断				
 	TIM4->CR1|=0x01;  	//使能定时器4	
	NVIC_SetPriority(TIM4_IRQn,NVIC_EncodePriority(7-2,1,1));//使能串口中断	
	NVIC_EnableIRQ(TIM4_IRQn);
								 
}



