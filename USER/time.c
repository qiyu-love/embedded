#include "time.h"

unsigned step=0;

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}


void jishi(unsigned char sj1,unsigned char  sj2)//参数：南北通行时间   东西通行时间
{
	switch (step)
	{
		case 0://南北绿，东西红
			nanbei_dis=sj1;
		  dongxi_dis =sj1+huang;
		  led_kz(1,0);
		  step=1;
			break;
		case 1:
			if(nanbei_dis>0)
			{
				nanbei_dis--;
				dongxi_dis--;
			}
			else //南北黄，东西红
			{
				nanbei_dis=huang-1;
				dongxi_dis--;
				led_kz(2,0);
				step=2;flag=1;
			}		
			break;
		case 2:
			if(nanbei_dis>0)
			{
				if(flag==1)//控制南北黄灯闪烁
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_0);	
		      GPIO_SetBits(GPIOA,GPIO_Pin_1);	
		      GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
        }
				else
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_0);	
		      GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
		      GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
        }
				nanbei_dis--;
				dongxi_dis--;
			}
			else//南北红，东西绿
			{
				nanbei_dis=sj2+huang;
				dongxi_dis=sj2;
				led_kz(0,1);
				step=3;
			}
			break;
		case 3:
			if(dongxi_dis>0)
			{
				nanbei_dis--;
				dongxi_dis--;
			}
			else//南北红，东西黄
			{
				nanbei_dis--;
				dongxi_dis=huang-1;
				led_kz(0,2);
				step=4;flag=1;
			}
			break;
		case 4:
			if(nanbei_dis>0)
			{
				if(flag==1)//控制东西黄灯闪烁
				{
						GPIO_ResetBits(GPIOA,GPIO_Pin_3);	
		        GPIO_SetBits(GPIOA,GPIO_Pin_4);	
		        GPIO_ResetBits(GPIOA,GPIO_Pin_5);	
        }
				else
				{
						GPIO_ResetBits(GPIOA,GPIO_Pin_3);	
	        	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	
		        GPIO_ResetBits(GPIOA,GPIO_Pin_5);	
        }
				nanbei_dis--;
				dongxi_dis--;
			}
			else
			{
				step=0;
			}
			break;
	}
}


//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
  static unsigned int a=0;
	
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标�
		a++;
    if(a>50)
		{
			if(mode==0)//自动运行
			{
			  jishi(nanbei,dongxi);
			}
			else if(mode==4)//手动设置通行时间
			{
				 jishi(nanbei_set,dongxi_set);
			}
			a=0;
			
				if(flag==0)
				{
						flag=1;
				}
				else 
				{
						flag=0;
				}
		}
   		

		
			
	}
}
