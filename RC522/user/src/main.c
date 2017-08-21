#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "systick_delay.h"
#include "spi.h"
#include "time.h"
#include "math.h"
#include "beep.h"
#include "malloc.h"
#include "rc522.h"
#include "motor.h"

//////////////////////////////////////////////////////////
//M1卡分為16個扇區，每個扇區由4塊（塊0、塊1、塊2、塊3）組成
//我們也將16個扇區的64個塊按絕對地址編號0~63
//第0扇區的塊0（即絕對地址0塊），他用於存放廠商代碼，已經固化，不可更改
//每個扇區的塊0、塊1、塊2為數據塊，可用於存放數據
//每個扇區的塊3為控制塊（絕對地址塊3、7、11....），包括了密碼A，存取控制、密碼B。

/*******************************
*连线说明：
*1--CS  <----->PG7
*2--SCK <----->PB3
*3--MOSI<----->PB5
*4--MISO<----->PB4
*5--悬空
*6--GND <----->GND
*7--RST(CE) <----->PG6
*8--VCC <----->VCC
************************************/

int main(void)	
{
	u8 temp=0;
	time7_delay_init();	
	led_init();//LED初始化
	beep_init();
	motor_Init();
	uart_init(9600);
	InitRc522(); 	//RC522
	led2=0;
  BEEP=0;			
	while(1)
	{ 
			RC522_Entrance_Guard();
			delay_ms(1000);
			printf("TEXT\r\n"); 
	}
}




