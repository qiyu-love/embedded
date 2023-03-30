#include "stm32f10x.h"
#include "smg.h"
void SMG_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
//使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = 0x00ff;					//PC0-PC7引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //配置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //GPIOC速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);				//初始化PC0-PC7
	
  GPIO_InitStructure.GPIO_Pin = 0x003f;					//PB0-PC5引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //配置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //GPIOB速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);				//初始化PB0-PB5
	
	//灯
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //配置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //GPIOB速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	

	
	//按键
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 			
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;  //浮空输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;      //速度
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}
