#include "stm32f4xx.h"
#include "dma.h"
#include "adc.h"
#include "delay.h"


//ADC到DMA
void dma2_init(u32 par,u16 ndtr,u32 mar)
{
	//DMA2模快时钟使能
	RCC->AHB1ENR |=0x01<<22;
	
	DMA2_Stream4->CR &=~(0x01<<0);		//禁止数据流
	
	DMA2_Stream4->CR &=~(0x07<<25);			
	DMA2_Stream4->CR |=0X00<<25;			//通道0
	
	DMA2_Stream4->CR &=~(0x03<<23);		//存储器单次传输
	DMA2_Stream4->CR &=~(0x03<<21);		//外设单次传输
	
	DMA2_Stream4->CR &=~(0x03<<16);		//优先级为低
	
	//DMA2_Stream4->CR &=~(0x03<<13);		
	//DMA2_Stream2->CR |=0x01<<13;			//存储器数据大小为16位
	DMA2_Stream4->CR &=~(0x03<<11);
	DMA2_Stream4->CR |=0x01<<11;				//外设数据据大小为16位
	
	DMA2_Stream4->CR |=0x01<<10;			//存储器地址增量模式
	DMA2_Stream4->CR &=~(0x01<<9);		//外设地址固定
	DMA2_Stream4->CR &=~(0x01<<8);		//非循环模式
	
	DMA2_Stream4->CR &=~(0x03<<6);		//外设到存储器
	
	DMA2_Stream4->PAR=par;						//外设地址
	DMA2_Stream4->NDTR =ndtr;
	DMA2_Stream4->M0AR =mar;
	
	DMA2_Stream4->FCR &=~(0x01<<2);		//使用直接模式，外设跟存储器数据宽度需要一致	
				
	DMA2_Stream4->CR |=0x01<<4;				//开完成中断
	
	
	
	
	NVIC_SetPriority(DMA2_Stream4_IRQn,NVIC_EncodePriority(7-2,1,0));
	NVIC_EnableIRQ(DMA2_Stream4_IRQn);
	DMA2_Stream4->CR |=0x01<<0;				//开启数据流	
	
	ADC1->CR2 &=~(0x03<<8);
	ADC1->CR2 |=0x03<<8;				//使能DMA
	ADC1->CR2 |=0X01<<30;    //软件触发开始转换	





}


void dma2_M_to_M_init(u32 par,u16 ndtr,u32 mar)
{
	//DMA2模快时钟使能
	RCC->AHB1ENR |=0x01<<22;
	
	DMA2_Stream0->CR &=~(0x01<<0);		//禁止数据流
	
	DMA2_Stream0->CR &=~(0x07<<25);
	DMA2_Stream0->CR |=0x00<<25;			//通道0
	
	DMA2_Stream0->CR &=~(0x03<<23);		//
	DMA2_Stream0->CR &=~(0x03<<21);
	
	DMA2_Stream0->CR &=~(0x03<<16);		//优先级为低
	
	//DMA2_Stream0->CR |=0x01<<13;			//存储器数据大小为16位
	DMA2_Stream0->CR &=~(0x03<<11);
	DMA2_Stream0->CR |=0x01<<11;				//外设数据据大小为16位
	
	DMA2_Stream0->CR |=0x01<<10;			//存储器地址增量模式
	DMA2_Stream0->CR |=0x01<<9;				//外设地址增量模式
	DMA2_Stream0->CR &=~(0x01<<8);		//非循环模式
	
	//DMA2_Stream0->FCR |=0x01<<2;			//禁止直接模式
	
	
	DMA2_Stream0->CR &=~(0x03<<6);	
	DMA2_Stream0->CR |=0x02<<6;				//存储器到存储器
	
				
	
	DMA2_Stream0->PAR=par;						//外设地址
	DMA2_Stream0->NDTR =ndtr;
	DMA2_Stream0->M0AR =mar;
	
//	DMA2_Stream0->FCR &=~(0X01<<2);  //直接传输模式,外设跟存储器数据宽度需要一致	

	

	//DMA2_Stream0->CR |=0x01;   //开启数据流	
	

}



//mar:存储器的地址
//ndtr：数据项数
void dma2_transf(u16 ndtr,u32 mar)
{
	DMA2->LIFCR |=0x01<<21;		//清完成传输标志
	DMA2_Stream2->CR &=~(0x01<<0); 	//关闭数据流
	while(DMA2_Stream2->CR & 0x01<<0);	//等待数据流结束
	DMA2_Stream2->NDTR =ndtr;
	DMA2_Stream2->M0AR =mar;
	DMA2_Stream2->CR |=0x01<<0;				//开启数据流
	
}

void DMA2_Stream4_IRQHandler(void)
{
	if(DMA2->HISR & (0x01<<5))
	{
		ADC1->CR2 &=~(0x01<<0);				//bu使能ADC
		DMA2->HIFCR |=0x01<<5;
		while(DMA2->HISR & (0x01<<5));
		DMA2_Stream0->CR |=0x01<<0;		//开启数据流0
	}
}

///////////////////////////////////////USART2 DMA发送配置部分//////////////////////////////////	   		    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址    
void UART_DMA_Config(u32 cpar,u32 cmar)
{
 	RCC->AHB1ENR|=1<<21;			//开启DMA1时钟
	delay_us(5);	
	DMA1_Stream6->PAR=cpar; 					//DMA1 外设地址 
	DMA1_Stream6->M0AR=cmar; 					//DMA1,存储器地址	
	DMA1_Stream6->CR &=~(0x01<<0);		//禁止数据流
	DMA1_Stream6->CR &=~(0x07<<25);			
	DMA1_Stream6->CR |=0X04<<25;			//通道4
	DMA1_Stream6->CR &=~(0x03<<23);		//存储器单次传输
	DMA1_Stream6->CR &=~(0x03<<21);		//外设单次传输
	DMA1_Stream6->CR &=~(0x03<<16);
	DMA1_Stream6->CR |=(0x01<<16);		//优先级为中
	DMA1_Stream6->CR &=~(0x03<<13);		//存储器数据位大小8位
	DMA1_Stream6->CR &=~(0x03<<11);		//外设数据据大小为8位				
	DMA1_Stream6->CR |=0x01<<10;			//存储器地址增量模式
	DMA1_Stream6->CR &=~(0x01<<9);		//外设地址非增量模式(固定）
	DMA1_Stream6->CR &=~(0x01<<8);		//非循环模式	
	DMA1_Stream6->CR &=~(0x03<<6);		
	DMA1_Stream6->CR |=(0x01<<6);		//存储器到外设 	
} 
//开启一次DMA传输
//ndtr：数据项数
void UART_DMA_Enable(u16 ndtr)
{
	DMA1->HIFCR |=0x01<<21;		//清完成传输标志
	DMA1_Stream6->CR &=~(0x01<<0); 	//关闭数据流
	while(DMA1_Stream6->CR & 0x01<<0);	//等待数据流结束
	DMA1_Stream6->NDTR =ndtr;
	DMA1_Stream6->CR |=0x01<<0;				//开启数据流
	
	
}	   



