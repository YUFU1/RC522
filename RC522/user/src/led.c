#include "led.h"
#include "stm32f4xx.h"

void led_init(void)
{
	  //GPIOF的6、9、10和GPIOC的0端口时钟使能
		RCC->AHB1ENR |=(0x01<<5) | (0x01<<2);				//开启GPIOF、C的时钟	
  
		//GPIOF的6、9、10和GPIOC的0 为通用输出模式
		GPIOF->MODER &=~(0x03<<12);		//F端口6清零
		GPIOF->MODER |=0X01<<12;			//F端口6为通过输出模式
	
		GPIOF->MODER &=~(0x03<<18);		//F端口9清零
		GPIOF->MODER |=0X01<<18;
	
		GPIOF->MODER &=~(0x03<<20);		//F端10清零
		GPIOF->MODER |=0X01<<20;
	
		GPIOC->MODER &=~(0x03<<0);		//C端口6清零
		GPIOC->MODER |=0X01<<0;
	
		//GPIOF的6、9、10和GPIOC的0推挽输出
		GPIOF->OTYPER &=~(0X01<<6);		//F端口6为推挽输出
	  GPIOF->OTYPER &=~(0X01<<9);
		GPIOF->OTYPER &=~(0X01<<10);
		GPIOC->OTYPER &=~(0X01<<0);
	
		//GPIOF的6、9、10和GPIOC的0低速	
		GPIOF->OSPEEDR &=~(0x03<<12); //为低速
		GPIOF->OSPEEDR &=~(0x03<<18);
		GPIOF->OSPEEDR &=~(0x03<<20);
		GPIOC->OSPEEDR &=~(0x03<<0);
		
		//GPIOF的6、9、10和GPIOC的0无上拉或下拉
		GPIOF->PUPDR &=~(0x03<<12);		//无上拉或下拉
		GPIOF->PUPDR &=~(0x03<<18);
		GPIOF->PUPDR &=~(0x03<<20);
		GPIOC->PUPDR &=~(0x03<<0);
		
		GPIOF->ODR |=0x01<<6;
		GPIOF->ODR |=0x01<<9;
		GPIOF->ODR |=0x01<<10;
		GPIOC->ODR |=0x01<<0;
	}		









