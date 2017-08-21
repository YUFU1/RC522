#include "adc.h"
#include "stm32f4xx.h"


void adc_init(void)
{
	//端口A时钟使能 PA5模拟
	RCC->AHB1ENR |=0x01<<0;
	GPIOA->MODER |=0x03<<10;
	//ADC模块使能
	RCC->APB2ENR |=0x01<<8;
	
	ADC1->CR1 &=~(0x03<<24);		//12位分辨率
	ADC1->CR1 &=~(0x01<<8);			//非扫描模式
	
	ADC1->CR2 &=~(0x01<<11); 		//右对齐
	ADC1->CR2 |=0x01<<10;				//每个规则转换结束EOC被置1
	ADC1->CR2 &=~(0x01<<1);			//单次转换模式
	
	ADC1->SMPR2 &=~(0x07<<15);
	ADC1->SMPR2 |=0x07<<15;			//480个采样周期
	
	ADC1->SQR1 &=~(0x0f<<20);		//规则通道序列长度为1，只有一个通道
	ADC1->SQR3 &=~(0x1f<<0);
	ADC1->SQR3 |=0x05<<0;				//通道编号为5
	
	ADC->CCR &=~(0x03<<16);
	ADC->CCR |=0x01<<16;				//ADC 4分频 82M/4=21M
	
	ADC1->CR2 |=0x01<<0;				//使能ADC
	
	
	
}


void adc_temp_init(void)
{
//	//端口A时钟使能 PA5模拟
//	RCC->AHB1ENR |=0x01<<0;
//	GPIOA->MODER |=0x03<<10;
	//ADC模块使能
	RCC->APB2ENR |=0x01<<8;
	
	ADC1->CR1 &=~(0x03<<24);		//12位分辨率
	ADC1->CR1 &=~(0x01<<8);			//非扫描模式
	
	ADC1->CR2 &=~(0x01<<11); 		//右对齐
	ADC1->CR2 |=0x01<<10;				//每个规则转换结束EOC被置1
	ADC1->CR2 &=~(0x01<<1);			//单次转换模式
	
	ADC1->SMPR1 &=~(0x07<<18);	//16通道的
	ADC1->SMPR1 |=0x07<<18;			//480个采样周期
	
	ADC1->SQR1 &=~(0x0f<<20);		//规则通道一次转换
	ADC1->SQR3 &=~(0x1f<<0);
	ADC1->SQR3 |=0x10<<0;				//通道编号16
	
	ADC->CCR &=~(0x03<<16);
	ADC->CCR |=0x01<<16;				//ADC 4分频 82M/4=21M
	
	ADC->CCR |=0x01<<23;				//使能温度传感器和Vrefint通道
	
	ADC1->CR2 |=0x01<<0;				//使能ADC
	
	
	
}



u16 get_adc1_ch5(void)
{
	u16 i,j,buf[100],a;

	for(i=0;i<10;i++)
	{
		//转换开始
		ADC1->CR2 |=0x01<<30;
		//等待转换结束
		while(!(ADC1->SR & 0x01<<1));
		buf[i]=ADC1->DR;
	}
	
	for(i=0;i<10;i++)
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				a=buf[i];
				buf[i]=buf[j];
				buf[j]=a;
			}
		}
		
		a=0;
		for(j=1;j<9;j++)
		{
			a+=buf[j];
		}
		
		return a/8.0;
	
}



u16 get_adc1_temp_ch16(void)
{
	u16 i,j,buf[100],a;
	//转换开始
	for(i=0;i<10;i++)
	{
		ADC1->CR2 |=0x01<<30;
		//等待转换结束
		while(!(ADC1->SR & 0x01<<1));
		buf[i]=ADC1->DR;
	}
	
	for(i=0;i<10;i++)
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				a=buf[i];
				buf[i]=buf[j];
				buf[j]=a;
			}
		}
		
		a=0;
		for(j=1;j<9;j++)
		{
			a+=buf[j];
		}
		
		return a/8.0;
	
}



