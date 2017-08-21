#include "key.h"
#include "stm32f4xx.h"
#include "delay.h"


void key_init(void)
{
	RCC->AHB1ENR |=(0x01<<0) | (0x01<<4);
	//A 0
	GPIOA->MODER &=~(0x03<<0);
	GPIOA->PUPDR &=~(0x03<<0);
	//E 2 3 4
	GPIOE->MODER &=~((0x03<<4)|(0x03<<6)|(0x03<<8));
	GPIOE->PUPDR &=~((0x03<<4)|(0x03<<6)|(0x03<<8));
	
}


//u8 key_scanf(void)
//{
//	u8 key=key_no;
//	static u8 key_sta=1;
//	if(key1 && (key_sta==1))
//	{
//		delay1();
//		if(key1 && (key_sta==1))
//		{
//			key=key1_ok;
//			key_sta=0;
//			while(key1);
//		}
//	}
//	else if(key1==0)
//	{
//		key_sta=1;
//	}
//	return key;
//}


u8 key_scanf(void)
{
	u8 key=key_no;
	static u8 key_sta=1;
	static u8 keya,keyb,keyc,time=0;
	keyc=keyb;
	keyb=keya;
	time++;	
	if(((key1==1) || (key2==0) || (key3==0) || (key4==0)) && (key_sta==1))
	{
		
		if(key1==1)
		{
			keya=key1_ok;			
		}
		else if(key2==0)
		{
			keya=key2_ok;			
		}
		else if(key3==0)
		{
			keya=key3_ok;			
		}
		else if(key4==0)
		{
			keya=key4_ok;			
		}
		
		if(time==3)
		{
			time=0;
			if(keyb==keyc && keyc==keya)
			{
				key=keya;
				key_sta=0;
			}
	  }
	}
	else if(key1==0 && key2==1 && key3==1 && key4==1 )
	{
		time=0;
		key_sta=1;
	}
	return key;
}



//	 			key=key_scanf();
//			
//				if(key==key1_ok)
//				{
//					led1=!led1;
//				}
//				
//				
//				 else if(key==key2_ok)
//				{
//					led2=!led2;
//				}
//					
//				
//				else if(key==key3_ok)
//				{
//					led3=!led3;
//				}
//					
//				
//				else if(key==key4_ok)
//				{
//					led4=!led4;
//				}
//				
//				delay1();	



