#include "includes.h"
//                           标准帧    数据帧                
CanTxMsg Eng_Cmd = {0x7DF,0,CAN_ID_STD,CAN_RTR_DATA,8,0x02,0x01,0x0C,0x00,0x00,0x00,0x00,0x00};//请求发动机转数
extern u16 EngineRPM;//每分钟发动机转数
#define Modular_Run_Red    GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)(1-GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5)))
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
u8 canindex=0;
u8 ledindex=0;
u16 con=0; 
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
			canindex++;
		if(canindex>=6)
		{
			canindex=0;
			con=1;
		}
			ledindex++;
		if(ledindex>=20)
		{
		  ledindex=0;
			Modular_Run_Red;
		//con=1;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
	}
}


int main(void)
{
			GPIO_InitTypeDef  GPIO_InitStructure;
			delay_init(72);
			uart1_init(115200);
			CAN_Config(CAN_500K);//CAN通信速率设置
			NVIC_Config();
			CAN1_Config16BitFilter(0x700, 0x700, 0x700, 0x700); //CAN滤波器设置
			TIM3_Int_Init(99,7199);//10Khz的计数频率，计数到5000为500ms  100 10ms
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
			GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高
			while(1)
			{
					if(con==1)
					{
								con=0;
									Send_CANFrame(&Eng_Cmd);//向ECU请求数据 发动机转数
					}

					if(RxFlay==1)//接收到正确返回数据 中断接收ECU返回的数据，在stm32f10x_it.c中的USB_LP_CAN1_RX0_IRQHandler函数
					{
								RxFlay=0;//清零CAN接收标志
								Usart_SendByte(USART1,0xaa);
								//Usart_SendByte(USART1,0x55);
							  Usart_SendByte(USART1,EngineRPM>>8);
			          Usart_SendByte(USART1,EngineRPM);
								//Usart_SendByte(USART1,0xaa);
								Usart_SendByte(USART1,0x55);
					}		
			}
}


