
/************************************************************************
*@�ļ�: Bsp.h
*@����:
*@�汾:
*@����:
*@����:
************************************************************************/
#include "includes.h"


#if 1
#pragma import(__use_no_semihosting)             
                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;         
_sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{   

  while((PC_USART->SR&0x40) == 0);
  PC_USART->DR = (u8)ch;      
  return ch;
}
#endif


/************************************************************************
  * @����:  ��������
  * @����:  Baud:���ڲ�����
  * @����ֵ: None
  **********************************************************************/
void PC_USART_Config(u32 Baud)
{
  u8 data;
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  if (PC_USARTAPB == APB1)
  {
    RCC_APB2PeriphClockCmd(RCC_APBxPeriph_PC_USART_IO | RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APBxPeriph_PC_USART,ENABLE);
  }
  else
  {
		RCC_APB2PeriphClockCmd(RCC_APBxPeriph_PC_USART_IO | RCC_APBxPeriph_PC_USART | RCC_APB2Periph_AFIO,ENABLE);
  }
  if (PC_PinRemap == ENABLE) //�ܽ�ӳ��
  {  				 
    GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
  }
  GPIO_InitStructure.GPIO_Pin = PC_USART_TXD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PC_USART_IO,&GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = PC_USART_RXD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PC_USART_IO,&GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = Baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_Init(PC_USART,&USART_InitStructure);
  data = data;
  data = PC_USART->DR;
  data = PC_USART->SR;
  //USART_ITConfig(PC_USART,USART_IT_RXNE,ENABLE); //���ڽ����жϴ�
  USART_Cmd(PC_USART,ENABLE);
}
void uart1_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}
/*
 * ��������Usart_SendByte
 * ����  ������һ���ֽ�
 * ����  ��pUSARTx�����ں� ch���ֽ�����
 * ���  : ��
 * ����  ���ⲿ����
 */
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	USART_SendData(pUSARTx,ch); /* ����һ���ֽ����ݵ�USART */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);/* �ȴ�������� */
}
/************************************************************************
  * @����:  �ж�����
  * @����:  None
  * @����ֵ: None
  **********************************************************************/
void NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; //CAN�ж�����--�����ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/************************************************************************
  * @����:   CAN����IO��������
  * @����:   None
  * @����ֵ: None
  **********************************************************************/
void CAN_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APBxPeriph_CAN_IO | RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
  GPIO_InitStructure.GPIO_Pin = CAN_RXD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(CAN_IO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CAN_TXD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CAN_IO, &GPIO_InitStructure);
  if (CAN_PinRemap == ENABLE)  //CANʹ���˹ܽ�ӳ�� 
  { 
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
  }
}
/************************************************************************
  * @����:  CAN��������
  * @����:  velocity ���ʷ�Ƶֵ
  * @����ֵ: None
  **********************************************************************/
void CAN_Config(u8 velocity)
{
  CAN_InitTypeDef  CAN_InitStructure;

  CAN_GPIOConfig();
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);
    
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_11tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
  CAN_InitStructure.CAN_Prescaler = velocity;
  CAN_Init(CAN1, &CAN_InitStructure);
}
/************************************************************************
  * @����:  CAN��׼֡�˲�������
  * @����:  id1,id2 Ч����   mid1,mid2 ������
  * @����ֵ: None
  **********************************************************************/
void CAN1_Config16BitFilter(u16 id1, u16 id2, u16 mid1, u16 mid2)                                                                             
{                                                                                                                         
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;                                                                       
                                                                                                                          
    CAN_FilterInitStructure.CAN_FilterNumber=1;                                                                           
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;                                                         
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;                                                        
    CAN_FilterInitStructure.CAN_FilterIdHigh=id1<<5;                                                                      
    CAN_FilterInitStructure.CAN_FilterIdLow=id2<<5;                                                                       
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=mid1<<5;                                                                  
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=mid2<<5;                                                                   
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;                                                           
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;                                                                  
                                                                                                                          
    CAN_FilterInit(&CAN_FilterInitStructure);
	  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);                                                                             
}

/************************************************************************
  * @����:  ����һ֡CAN����(�����������)
  * @����:  
  * @����ֵ: None
  **********************************************************************/
__IO u8 RxFlay;
void Send_CANFrame(CanTxMsg* TxMessage)
{
  u8 TransmitMailbox; //����
  u32 i;
  RxFlay = 0;

  TransmitMailbox = CAN_Transmit(CAN1, TxMessage); //���ͳ�ȥ������Ϣ
  while(CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK);//�ȴ��������
  for (i = 0;i < 10000000/3;i++)	//��ŵȴ�1.5��  �ȴ�ʱ���У���ȥCAN�жϽ���ECU���صı�����Ϣ
  {
    if (RxFlay == 1) break; //�жϳɹ����յ�ECU���صı�����Ϣ
  }
}





