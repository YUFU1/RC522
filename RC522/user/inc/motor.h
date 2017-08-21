#ifndef __MOTOR_H
#define __MOTOR_H	

#include "io_bit.h"

//由于还不知道电机的接口，所以暂时就法写	
//下面的io口是其他开发板上的
#define qmotor1 PFout(1)       // PF1前左马达启动信号
#define qmotor2 PFout(2)       // PF2前右马达启信号	
#define qmotor3 PFout(3)       // PF3后左马达启动信号
#define qmotor4 PFout(4)       // PF4后右马达启动信号

void motor_Init(void);           //初始化

void motorCW(void);//正转60°

void motorCCW(void);//反转60°


#endif



