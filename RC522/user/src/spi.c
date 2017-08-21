#include "stm32f4xx.h" 
#include "spi.h"

//使用NRF2401进行与刷卡模块RC522进行通信
//SPI控制器初始化
void SPI1_Init(void)
{
	u16 tempreg=0;
	//SPI1 MISO   MOSI  SCK
	RCC->AHB1ENR |=0X01<<1;
	GPIOB->MODER &=~(0X03<<6 |0X03<<8 |0X03<<10 );
	GPIOB->MODER |=0X02<<6 |0X02<<8 |0X02<<10 ;
	
	GPIOB->OTYPER &=~(0X01<<3 | 0X01<<5 );  //推挽输出
	GPIOB->OSPEEDR |=(0X02<<6 |0X02<<10 );  //输出速度50M
	GPIOB->PUPDR &=~(0X03<<6 |0X03<<8 | 0X03<<10 ); //无上下拉
	
	GPIOB->AFR[0] &=0xFF000FFF;
	GPIOB->AFR[0] |=0x00555000;
	
	//NRF RC522_RST(CE)--->PG6  CS---->PG7
	RCC->AHB1ENR |=0X01<<6;	
	
	GPIOG->MODER &=~(0x03<<12 | 0x03<<14);			//模式----通用输出模式（01）
	GPIOG->MODER |=(0X01<<12 | 0X01<<14 );

	GPIOG->OTYPER &= ~(0X01<<6 | 0X01<<7);		//输出类型----推挽

	GPIOG->OSPEEDR |=(0x02<<12 | 0x02<<14);		//输出速度----50M

//	GPIOG->PUPDR &=~(0x03<<12 | 0x03<<14); //下拉	
	GPIOG->PUPDR |=(0x01<<12 | 0x01<<14);			//上拉
	
	
//	//SPI模块使能
//	RCC->APB2ENR |=0x01<<12;
//	RCC->APB2RSTR |=0x01<<12;			//复位模块
//	RCC->APB2RSTR &=~(0x01<<12);	//停止复位模块
//	
//	SPI1->CR1 &=~(0x01<<15);   	//双线单向通信
//	SPI1->CR1 &=~(0x01<<11);		//8位数据帧格式
//	SPI1->CR1|=1<<1; 						//CPOL=0时空闲模式下SCK为1  CPOL=1
//	SPI1->CR1|=1<<0; 						//数据采样从第二个时间边沿开始,CPHA=1  
//	SPI1->CR1|=0<<7; 						//MSBfirst
//	SPI1->CR1 &=~(0x01<<10);   //全双工
//	SPI1->CR1 |=0x03<<8;			 //软件管理
//	SPI1->CR1 &=~(0x07<<3);		 
//	SPI1->CR1|=7<<3;						//波特率f pclk/256
//	SPI1->CR1 |=0x01<<2;				//主模式
//	SPI1->CR1 &=~(0x03<<0);			//模式0  串行同步时钟的空闲状态为低电平
//															//串行同步时钟的第一个跳变沿（上升或下降）数据被采样
//	SPI1->CR1 |=0x01<<6;	

	

		RCC->AHB1ENR|=1<<1;    	//使能PORTB时钟	   
		RCC->APB2ENR|=1<<12;   	//SPI1时钟使能 

//		GPIOB->MODER &=~(0X03<<6 |0X03<<8 |0X03<<10 );
//		GPIOB->MODER |=0X02<<6 |0X02<<8 |0X02<<10 ;
//		
//		GPIOB->OTYPER &=~(0X01<<3 | 0X01<<5 );  //推挽输出
//		GPIOB->OSPEEDR |=(0X02<<6 |0X02<<10 );  //输出速度50M
//		GPIOB->PUPDR &=~(0X03<<6 |0X03<<8 | 0X03<<10 ); //无上下拉
//		
//		GPIOB->AFR[0] &=0xFF000FFF;//清零
//		GPIOB->AFR[0] |=0x00555000;//复用为AF5

	//这里只针对SPI口初始化
	RCC->APB2RSTR|=1<<12;	//复位SPI1
	RCC->APB2RSTR&=~(1<<12);//停止复位SPI1
	tempreg|=0<<10;			//全双工模式	
	tempreg|=1<<9;			//软件nss管理
	tempreg|=1<<8;			 
	tempreg|=1<<2;			//SPI主机  
	tempreg|=0<<11;			//8位数据格式	
	tempreg|=1<<1;			//空闲模式下SCK为1 CPOL=1 
	tempreg|=1<<0;			//数据采样从第2个时间边沿开始,CPHA=1  
 	//对SPI1属于APB2的外设.时钟频率最大为84Mhz频率.
	tempreg|=7<<3;			//Fsck=Fpclk1/256
	tempreg|=0<<7;			//MSB First  
	tempreg|=1<<6;			//SPI启动 
	SPI1->CR1=tempreg; 		//设置CR1  
	SPI1->I2SCFGR&=~(1<<11);//选择SPI模式
	SPI1_ReadWriteByte(0xff);//启动传输		 
	
}

void spi_delay(void)
{
	u32 i=5;
	while(i--);
}

//参数：_data---待发送的数据
//返回值：读取到的一个字节数据
u8 SPI1_ReadWriteByte(u8 _data)
{
	while(!(SPI1->SR & 0x01<<1));		//等待发送缓冲器为空
	SPI1->DR=_data;		
	while(!(SPI1->SR & 0x01<<0));		//等待接收缓冲器为空
	return SPI1->DR;
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_Read_WriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}

