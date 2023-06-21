
#ifndef __BSP_H
#define __BSP_H

/**************************结构类型************************************/
typedef enum {APB1 = 0, APB2} APBType;
/**************************函数声明************************************/
void PC_USART_Config(u32 Baud);
void uart1_init(u32 bound);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch );
void NVIC_Config(void);
void CAN_GPIOConfig(void);
void CAN_Config(u8 velocity);
void CAN1_Config16BitFilter(u16 id1, u16 id2, u16 mid1, u16 mid2);
void Send_CANFrame(CanTxMsg* TxMessage);
/**************************全局变量************************************/
extern __IO u8 RxFlay;
/**************************PC串口配置**********************************/
#define RCC_APBxPeriph_PC_USART_IO  RCC_APB2Periph_GPIOA
#define RCC_APBxPeriph_PC_USART		RCC_APB1Periph_USART2
#define PC_USART_TXD				GPIO_Pin_2
#define PC_USART_RXD				GPIO_Pin_3
#define PC_USART_IO					GPIOA
#define PC_USART	                USART2
#define PC_PinRemap					DISABLE
#define PC_USARTAPB					APB1
//#define PC_USART_IRQHandler			USART2_IRQHandler
/**************************CAN配置*************************************/
#define CAN_500K                    4
#define CAN_125K                    16
#define RCC_APBxPeriph_CAN_IO       RCC_APB2Periph_GPIOB
#define CAN_RXD					    GPIO_Pin_8
#define CAN_TXD						GPIO_Pin_9
#define CAN_IO						GPIOB
#define CAN_PinRemap				ENABLE  
/**********************************************************************/
#endif
