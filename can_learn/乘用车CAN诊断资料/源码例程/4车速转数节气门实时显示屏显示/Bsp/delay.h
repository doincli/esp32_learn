
#ifndef __DELAY_H
#define __DELAY_H 			   

/********delay.c结构定义*********/

/********delay.c函数定义*********/
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_s(u32 ns);
#endif





























