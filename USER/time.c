#include "time.h"

unsigned step=0;

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // ±÷” πƒ‹
	
	//∂® ±∆˜TIM3≥ı ºªØ
	TIM_TimeBaseStructure.TIM_Period = arr; //…Ë÷√‘⁄œ¬“ª∏ˆ∏¸–¬ ¬º˛◊∞»ÎªÓ∂Øµƒ◊‘∂Ø÷ÿ◊∞‘ÿºƒ¥Ê∆˜÷‹∆⁄µƒ÷µ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //…Ë÷√”√¿¥◊˜Œ™TIMx ±÷”∆µ¬ ≥˝ ˝µƒ‘§∑÷∆µ÷µ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //…Ë÷√ ±÷”∑÷∏Ó:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMœÚ…œº∆ ˝ƒ£ Ω
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //∏˘æ›÷∏∂®µƒ≤Œ ˝≥ı ºªØTIMxµƒ ±º‰ª˘ ˝µ•Œª
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); // πƒ‹÷∏∂®µƒTIM3÷–∂œ,‘ –Ì∏¸–¬÷–∂œ

	//÷–∂œ”≈œ»º∂NVIC…Ë÷√
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3÷–∂œ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //œ»’º”≈œ»º∂0º∂
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //¥””≈œ»º∂3º∂
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÕ®µ¿±ª πƒ‹
	NVIC_Init(&NVIC_InitStructure);  //≥ı ºªØNVICºƒ¥Ê∆˜

	TIM_Cmd(TIM3, ENABLE);  // πƒ‹TIMx
}


void jishi(unsigned char sj1,unsigned char  sj2)//≤Œ ˝£∫ƒœ±±Õ®–– ±º‰   ∂´Œ˜Õ®–– ±º‰
{
	switch (step)
	{
		case 0://ƒœ±±¬Ã£¨∂´Œ˜∫Ï
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
			else //ƒœ±±ª∆£¨∂´Œ˜∫Ï
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
				if(flag==1)//øÿ÷∆ƒœ±±ª∆µ∆…¡À∏
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
			else//ƒœ±±∫Ï£¨∂´Œ˜¬Ã
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
			else//ƒœ±±∫Ï£¨∂´Œ˜ª∆
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
				if(flag==1)//øÿ÷∆∂´Œ˜ª∆µ∆…¡À∏
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


//∂® ±∆˜3÷–∂œ∑˛ŒÒ≥Ã–Ú
void TIM3_IRQHandler(void)   //TIM3÷–∂œ
{
  static unsigned int a=0;
	
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //ºÏ≤ÈTIM3∏¸–¬÷–∂œ∑¢…˙”Î∑Ò
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //«Â≥˝TIMx∏¸–¬÷–∂œ±Í÷
		a++;
    if(a>50)
		{
			if(mode==0)//◊‘∂Ø‘À––
			{
			  jishi(nanbei,dongxi);
			}
			else if(mode==4)// ÷∂Ø…Ë÷√Õ®–– ±º‰
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
