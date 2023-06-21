#include "includes.h"
//                           ��׼֡    ����֡                
CanTxMsg Eng_Cmd = {0x7DF,0,CAN_ID_STD,CAN_RTR_DATA,8,0x02,0x01,0x0C,0x00,0x00,0x00,0x00,0x00};//���󷢶���ת��
extern u16 EngineRPM;//ÿ���ӷ�����ת��
#define Modular_Run_Red    GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)(1-GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5)))
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
u8 canindex=0;
u8 ledindex=0;
u16 con=0; 
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
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
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
	}
}


int main(void)
{
			GPIO_InitTypeDef  GPIO_InitStructure;
			delay_init(72);
			uart1_init(115200);
			CAN_Config(CAN_500K);//CANͨ����������
			NVIC_Config();
			CAN1_Config16BitFilter(0x700, 0x700, 0x700, 0x700); //CAN�˲�������
			TIM3_Int_Init(99,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  100 10ms
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
			GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����
			while(1)
			{
					if(con==1)
					{
								con=0;
									Send_CANFrame(&Eng_Cmd);//��ECU�������� ������ת��
					}

					if(RxFlay==1)//���յ���ȷ�������� �жϽ���ECU���ص����ݣ���stm32f10x_it.c�е�USB_LP_CAN1_RX0_IRQHandler����
					{
								RxFlay=0;//����CAN���ձ�־
								Usart_SendByte(USART1,0xaa);
								//Usart_SendByte(USART1,0x55);
							  Usart_SendByte(USART1,EngineRPM>>8);
			          Usart_SendByte(USART1,EngineRPM);
								//Usart_SendByte(USART1,0xaa);
								Usart_SendByte(USART1,0x55);
					}		
			}
}


