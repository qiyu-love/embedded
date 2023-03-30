#include "stm32f10x.h"
#include "Delay.h"
#include "smg.h"
#include "time.h"


#define k1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define k2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define k3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define k4 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)

//定义0～9十个数字的字型码表
unsigned int table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0xff};   
unsigned int wei[]={0x0fe,0x0fd,0x0fb,0x0f7,0x0ef,0x0df,0xff,0xff}; //位码;

unsigned char mode=0;  //0运行自动  1进入设置   2设置主干道  3设置支路  4运行手动   5特殊情况

//时间显示
unsigned char  nanbei_dis=20;   //南北数码管时间(实时显示)
unsigned char  dongxi_dis=15;   //东西数码管时间(实时显示)

//手动设置的时间
unsigned char  nanbei_set=40;   //南北数码管时间
unsigned char  dongxi_set=40;   //东西数码管时间

//自动模式下通行时间
unsigned char  nanbei=20;   //南北数码管时间
unsigned char  dongxi=15;   //东西数码管时间

unsigned char  huang=3;     //黄灯时间
extern unsigned step;
unsigned char  flag=0;//黄灯闪烁

//数码管显示函数
void display(unsigned char a,unsigned char b)
{
	GPIO_Write(GPIOB,wei[0]);
	GPIO_Write(GPIOC,table[a/10]);
	Delay(23);
	GPIO_Write(GPIOB,0x0ff);
	Delay(23);
	
	GPIO_Write(GPIOB,wei[1]);
	GPIO_Write(GPIOC,table[a%10]);
	Delay(23);
	GPIO_Write(GPIOB,0x0ff);
	Delay(23);
	
	GPIO_Write(GPIOB,wei[2]);
	GPIO_Write(GPIOC,table[b/10]);
	Delay(23);
	GPIO_Write(GPIOB,0x0ff);
	Delay(23);
	
	GPIO_Write(GPIOB,wei[3]);
	GPIO_Write(GPIOC,table[b%10]);
	Delay(23);
	GPIO_Write(GPIOB,0x0ff);
	Delay(23);
}

//红绿灯颜色控制  0红灯  1绿灯  2黄灯
void led_kz(unsigned char c1,unsigned char c2)
{
	if(c1==0)//南北红灯
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);	
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
		GPIO_SetBits(GPIOA,GPIO_Pin_2);	
	}
  else if(c1==1)//南北绿灯
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_0);	
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
	}
	else if(c1==2)//南北黄灯
	{

		 GPIO_ResetBits(GPIOA,GPIO_Pin_0);	
		 GPIO_SetBits(GPIOA,GPIO_Pin_1);	
		 GPIO_ResetBits(GPIOA,GPIO_Pin_2);	

	}
	
	if(c2==0)//南北红灯
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_3);	
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);	
	}
  else if(c2==1)//南北绿灯
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);	
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	
		GPIO_SetBits(GPIOA,GPIO_Pin_5);	
	}
	else if(c2==2)//南北黄灯
	{

		GPIO_ResetBits(GPIOA,GPIO_Pin_3);	
		GPIO_SetBits(GPIOA,GPIO_Pin_4);	
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);	

	}
	
}




void key()
{
	if(k1==0)	
	{	
		if(mode==0)
		{
			nanbei_dis=nanbei_set;
			dongxi_dis=dongxi_set;
			mode=1;//进入设置
    }
		else if(mode==1)
		{
			nanbei_dis=nanbei_set;
			dongxi_dis=dongxi_set;
			mode=2;//设置主干道
    }
		else if(mode==2)
		{
			nanbei_dis=nanbei_set;
			dongxi_dis=dongxi_set;
			mode=3;//设置支路
    }
		else if(mode==3)
		{
			nanbei_dis=nanbei_set;
			dongxi_dis=dongxi_set;
			mode=4;//运行设置的时间
    }
		else if(mode==4)
		{
			nanbei_dis=nanbei;
			dongxi_dis=dongxi;
			mode=0;//运行自动循环
    }
		else 
		{
			nanbei_dis=nanbei;
			dongxi_dis=dongxi;
			mode=0;
    }
		step=0;
		while(!k1);	 
	}	
	
		if(k2==0)	//jia
		{
			if(mode==2)
			{
				if(nanbei_set<60)
				nanbei_set++;
      }
			else if(mode==3)
			{
				if(dongxi_set<60)
				dongxi_set++;
      }
			while(!k2);	
    }
		
		if(k3==0)	//jian
		{
			if(mode==2)
			{
				if(nanbei_set>10)
				{
					nanbei_set--;
        }
      }
			else if(mode==3)
			{
				if(dongxi_set>10)
				{
					dongxi_set--;
        }
      }
			while(!k3);	
    }
		
		if(k4==0)	
		{
			mode=5;
			while(!k4);	
    }
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	TIM3_Int_Init(799,99);
	SMG_Init();
	nanbei_dis=nanbei;//初始显示的值
	dongxi_dis=dongxi;
	while(1)
	{
		key();
		if((mode==0)||(mode==4))//显示倒计时(运行时)
		{
			 display(nanbei_dis,dongxi_dis);
    }
		else if((mode==1)||(mode==2)||(mode==3))//显示设置的时间（设置时）
		{	 
			display(nanbei_set,dongxi_set);			 
    }
		else//紧急特殊
		{
			GPIO_Write(GPIOB,0x0ff);//关闭数码管显示
			led_kz(0,0);//全部红灯
    }
		
	}
}
