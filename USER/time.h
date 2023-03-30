#ifndef _time_H
#define _time_H

#include "stm32f10x.h"


extern unsigned char mode;
extern unsigned char  flag;//黄灯闪烁

//时间显示
extern unsigned char  nanbei_dis;   //南北数码管时间(实时显示)
extern unsigned char  dongxi_dis;   //东西数码管时间(实时显示)

//手动设置的时间
extern unsigned char  nanbei_set;   //南北数码管时间
extern unsigned char  dongxi_set;   //东西数码管时间

//自动模式下通行时间
extern unsigned char  nanbei;   //南北数码管时间
extern unsigned char  dongxi;   //东西数码管时间

extern unsigned char  huang;     //黄灯时间

extern void led_kz(unsigned char c1,unsigned char c2);
void TIM3_Int_Init(u16 arr,u16 psc);

#endif
