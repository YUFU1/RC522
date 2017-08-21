#include "stm32f4xx.h" 
#include "motor.h"
#include "delay.h"
#include "beep.h"
#include "uart.h"
#include "stdio.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 

//输出输入驱动代码	   
//由于还不知道电机的接口，所以暂时就法写	
//下面的io口是其他开发板上的
////////////////////////////////////////////////////////////////////////////////// 	   


void motor_Init(void)
{
//	 GPIO_InitTypeDef  GPIO_InitStructure; 	
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	 //使能PF端口时钟
//	 
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	 //IO-->PF1、PF2、PF3、PF4 端口配置	                     //推挽输出
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		                     //IO口速度为50MHz
//	 
//	GPIO_Init(GPIOF, &GPIO_InitStructure);					                     //根据设定参数初始化PF1、PF2、PF3、PF4
	
	
	//打开PF外设时钟
	RCC->AHB1ENR |=(1<<5);
	/**PF1、PF2、PF3、PF4配置为推挽输出**/
	//端口模式寄存器
	GPIOF->MODER &=~(0xFF<<2);  //清零
	GPIOF->MODER |=(0x55<<2);     //配置为通用输出模式
	//输出类型
	GPIOF->OTYPER &=~(0xF<<1);   //推挽输出
	//输出速率
	GPIOF->OSPEEDR &=~(0xFF<<2); //清零
	GPIOF->OSPEEDR |=(0xaa<<2); //输出速率50M
	//上下拉
	GPIOF->PUPDR &=~(0xFF<<2);  //清零
	GPIOF->PUPDR|=(0x55<<2);     //配置为上拉
}
/********************************************************************************
单极性步进电机驱动方式：
单四拍： A-B-C-D;
双四拍：AB-BC-CD-DA;
单双八拍：A-AB-B-BC-C-CD-D-DA
对于单双八拍的方式：每次转动角度为3.75°，对于双四拍每次转动的角度为7.5°
双极性步进电机驱动方式：
单四拍：A/-B-A-B/
双四拍：A/B-AB-AB/-A/B/
单双八拍：A/-A/B-B-AB-A-AB/-B/-A/B/
使用的单极性步进电机，驱动方式为单双八拍，驱动芯片是ULN2003
unsigned char  FFW[8]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9}; //正转
unsigned char  FFZ[8]={0xf9,0xf8,0xfc,0xf4,0xf6,0xf2,0xf3,0xf1}; //反转
**********************************************************************************/



/************************************
	电机正转1节拍的 角度约为0.7°
	电机正转90°
************************************/
void motorCW(void)
{
	int i;
	for(i=0;i<130;i++)
	{
		qmotor1=0;qmotor2=0;qmotor3=0;qmotor4=1;
		delay_ms(1);
		qmotor1=0;qmotor2=0;qmotor3=1;qmotor4=1;
		delay_ms(1);
		qmotor1=0;qmotor2=0;qmotor3=1;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=1;qmotor3=1;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=1;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=1;qmotor2=1;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=1;qmotor2=0;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=1;qmotor2=0;qmotor3=0;qmotor4=1;
		delay_ms(1);
			
	}
}

/************************************
             电机反转90°
************************************/
void motorCCW(void)
{
	int i;
	for(i=0;i<130;i++)
	{
		qmotor1=1;qmotor2=0;qmotor3=0;qmotor4=1;
		delay_ms(1);
		qmotor1=1;qmotor2=0;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=1;qmotor2=1;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=1;qmotor3=0;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=1;qmotor3=1;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=0;qmotor3=1;qmotor4=0;
		delay_ms(1);
		qmotor1=0;qmotor2=0;qmotor3=1;qmotor4=1;
		delay_ms(1);
		qmotor1=0;qmotor2=0;qmotor3=0;qmotor4=1;
		delay_ms(1);
			
	}
}





