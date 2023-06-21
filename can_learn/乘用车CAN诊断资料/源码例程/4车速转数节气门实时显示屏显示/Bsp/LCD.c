/******************************************************************************

*��Ҫ˵����
��.h�ļ��У�#define Immediatelyʱ��������ʾ��ǰ����
�����#define Delay����ֻ����ִ����LCD_WR_REG(0x0007,0x0173);
֮��Ż���ʾ��ִ��һ��LCD_WR_REG(0x0007,0x0173)������д����
�ݶ�������ʾ��
#define Delayһ�����ڿ����������ʾ����ֹ��ʾ��ȫ��ͼ���ˢ�¹���
******************************************************************************/
#include "stm32f10x.h"
#include "LCD.h"
//#include "ff.h"	
#include <stdio.h>
//#include "FONT.h"
#include "GB1616.h"	//16*16������ģ
#include "sheep.h"
//#include "spi.h"

/************************************************************************
  * @����:   LCD����IO��������
  * @����:   None
  * @����ֵ: None
  **********************************************************************/

 void LCD_SPI_Config ( void )
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE );	

	//SPI1�ڳ�ʼ��
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin =  SPI1_MOSI| SPI1_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin = SPI1_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);
  GPIO_ResetBits(GPIO_SPI1,SPI1_CS);
	
	/* SPI1 configuration */                                            //��ʼ��SPI1�ṹ��
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1����Ϊ����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //����SPI1Ϊ��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                    //����ʱ���ڲ�����ʱ��ʱ��Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                    //��һ��ʱ���ؿ�ʼ��������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS�ź��������ʹ��SSIλ������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  //SPI������Ԥ��ƵֵΪ8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRCֵ����Ķ���ʽ

	SPI_Init(SPI1, &SPI_InitStructure);                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPI1�Ĵ���
	
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);                                              //ʹ��SPI1����


  RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );//LCD RS�������� 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

 void SPI_LCD_SendByte(u16 byte)
 {
 	SPI_I2S_SendData(SPI1, byte);

 }


 void WriteComm(u16 CMD)
{			
	LCD_RS(0);
	// macLCD_RS_0();
	SPI_LCD_SendByte(CMD);
}
void  WriteData(u16 tem_data)
{			
	LCD_RS(1);
	// macLCD_RS_1();
	SPI_LCD_SendByte(tem_data);
}
void Delay(__IO u32 nCount)
{	
	volatile int i;
	for(i=0;i<720;i++)
    for(; nCount != 0; nCount--);
}  



/**********************************************
Lcd��ʼ������
Initial condition  (DB0-15,RS,CSB,WRD,RDB,RESETB="L") 
***********************************************/
void Lcd_Initialize(void)
{	
	int i;
// LCD_FSMC_Config();
// LCD_Rst();

// 	WriteComm(0x00);
// 	a = *(__IO u16 *) (Bank1_LCD_D); 



// 	printf("ID=%04x\r\n",a);
// 	a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);
// a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);
// 	a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);
// a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);
// 	a = *(__IO u16 *) (Bank1_LCD_D); 
// 	printf("ID=%04x\r\n",a);	
	
// 	
		////////////////////HX8357C+TM3.2////////////

SPI_CS(1);
Delay(10);
SPI_CS(0);

//WriteComm(0x0000);
Delay(10);
//S6D05A1��ʼ��
	
WriteComm(0xF0);
WriteData(0x5A);
WriteData(0x5A);

WriteComm(0xF1);
WriteData(0x5A);
WriteData(0x5A);



WriteComm(0xF2);
WriteData(0x3B);
WriteData(0x33);
WriteData(0x03);
WriteData(0x0C);
WriteData(0x08);
WriteData(0x08);
WriteData(0x08);
WriteData(0x00);
WriteData(0x08);
WriteData(0x08);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x33);
WriteData(0x0C);
WriteData(0x08);
WriteData(0x0C);
WriteData(0x08);

//WriteComm(0xF3);
//WriteData(0x00);
//WriteData(0x00);
//WriteData(0x00);
//WriteData(0x00);

WriteComm(0xF4);
WriteData(0x07);

WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);

WriteData(0x04);
WriteData(0x70);
WriteData(0x03);
WriteData(0x04);
WriteData(0x70);
WriteData(0x03);


WriteComm(0xF5);
WriteData(0x00);
WriteData(0x46);//Set VCOMH
WriteData(0x70);//Set VCOM Amplitude
WriteData(0x00);
WriteData(0x00);
WriteData(0x02);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x46);
WriteData(0x70);


WriteComm(0xF6);
WriteData(0x03);
WriteData(0x00);
WriteData(0x08);
WriteData(0x03);
WriteData(0x03);
WriteData(0x00);
WriteData(0x03);
WriteData(0x00);

WriteComm(0xF7);
WriteData(0x48);
WriteData(0x80);//RGB_MCU(DM)
WriteData(0x10);
WriteData(0x02);
WriteData(0x00);
//////////////////////////////////////////////////////////////
WriteComm(0xF8);
WriteData(0x11);
WriteData(0x00);

WriteComm(0xF9); //Gamma Selection
WriteData(0x14);

WriteComm(0xFA); //Positive Gamma Control
WriteData(0x33);
WriteData(0x07);
WriteData(0x04);
WriteData(0x1A);
WriteData(0x18);
WriteData(0x1C);
WriteData(0x24);
WriteData(0x1D);
WriteData(0x26);
WriteData(0x28);
WriteData(0x2F);
WriteData(0x2E);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);

WriteComm(0xFB); //Positive Gamma Control
WriteData(0x33);
WriteData(0x03);
WriteData(0x00);
WriteData(0x2E);
WriteData(0x2F);
WriteData(0x28);
WriteData(0x26);
WriteData(0x1D);
WriteData(0x24);
WriteData(0x1C);
WriteData(0x18);
WriteData(0x1A);
WriteData(0x04);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);

WriteComm(0xF9); //Gamma Selection
WriteData(0x12);

WriteComm(0xFA); //Positive Gamma Control
WriteData(0x36);
WriteData(0x07);
WriteData(0x04);
WriteData(0x1C);
WriteData(0x1C);
WriteData(0x23);
WriteData(0x28);
WriteData(0x1C);
WriteData(0x25);
WriteData(0x26);
WriteData(0x2E);
WriteData(0x2B);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);

WriteComm(0xFB); //Positive Gamma Control
WriteData(0x33);
WriteData(0x06);
WriteData(0x00);
WriteData(0x2B);
WriteData(0x2E);
WriteData(0x26);
WriteData(0x25);
WriteData(0x1C);
WriteData(0x28);
WriteData(0x23);
WriteData(0x1C);
WriteData(0x1C);
WriteData(0x04);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);

WriteComm(0xF9); //Gamma Selection
WriteData(0x11);

WriteComm(0xFA); //Positive Gamma Control
WriteData(0x33);
WriteData(0x07);
WriteData(0x04);
WriteData(0x30);
WriteData(0x32);
WriteData(0x34);
WriteData(0x35);
WriteData(0x11);
WriteData(0x1D);
WriteData(0x20);
WriteData(0x28);
WriteData(0x20);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);

WriteComm(0xFB); //Positive Gamma Control
WriteData(0x33);
WriteData(0x03);
WriteData(0x00);
WriteData(0x20);
WriteData(0x28);
WriteData(0x20);
WriteData(0x1D);
WriteData(0x11);
WriteData(0x35);
WriteData(0x34);
WriteData(0x32);
WriteData(0x30);
WriteData(0x04);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);

WriteComm(0x44);
WriteData(0x00);
WriteData(0x01);

WriteComm(0x2A);
WriteData(0x00);
WriteData(0x00);
WriteData(0x01);
WriteData(0x3F);

WriteComm(0x2B);
WriteData(0x00);
WriteData(0x00);
WriteData(0x01);
WriteData(0xDF);


WriteComm(0x36);//Memory Data Access Control
WriteData(0x00);

WriteComm(0x3A);//SET 65K Color
WriteData(0x55);

WriteComm(0x11); 
Delay(120);

WriteComm(0x29);//Display on
WriteComm(0x2C);//Write GRAM

Delay(10);
WriteComm(0x36); //Set_address_mode
WriteData(0x60); //�����������½ǿ�ʼ�������ң����µ���


//test_color();
//Lcd_Light_ON;
//macLCD_BL_1();

#if 1
WriteComm(0x36);//����
WriteData(0x40);//
BlockWrite(0,319,0,479);
// 	while(!(SPI1->SR&0x0002));
for(i=0;i<320*480;i++)
{
	LCD_RS(1);
	SPI_LCD_SendByte( BLUE>>8);
		LCD_RS(1);
	SPI_LCD_SendByte( BLUE);
}
for(i=0;i<320*480;i++)
{
		LCD_RS(1);
	SPI_LCD_SendByte(YELLOW>>8);
		LCD_RS(1);
	SPI_LCD_SendByte(YELLOW);
}
for(i=0;i<320*480;i++)
{
		LCD_RS(1);
	SPI_LCD_SendByte(Black>>8);
		LCD_RS(1);
	SPI_LCD_SendByte( Black);
}
#endif






// while(1)
//{
//WriteComm(0x36);//����
//WriteData(0x40);//
//BlockWrite(0,319,0,479);
//// 	while(!(SPI1->SR&0x0002));
//for(i=0;i<320*480;i++)
//{
//	 macLCD_RS_1() ;
//	SPI_LCD_SendByte((u8)(BLUE>>8));
//	 macLCD_RS_1();
//	SPI_LCD_SendByte((u8)BLUE);
//}
//for(i=0;i<320*480;i++)
//{
//	 macLCD_RS_1();
//	SPI_LCD_SendByte((u8)(YELLOW>>8));
//	 macLCD_RS_1();
//		SPI_LCD_SendByte((u8)YELLOW);
//}

//WriteComm(0x36);
//WriteData(0x60);//
////LCD_Fill_Pic(0,0,480,320, gImage_sheep);
//}
// while(1)
// {
// 	display_picture("T35_05.BMP");
// 	display_picture("T35_01.BMP");
// 	display_picture("T35_02.BMP");
// 	display_picture("T35_03.BMP");
// 	display_picture("T35_04.BMP");
// 	display_picture("T35_00.BMP");

// }
}
void test_color(void)
{
	unsigned short i,j;
	BlockWrite(0,319,0,479);
	for(i=0;i<480;i++)
	{	for(j=0;j<320;j++)
		{
			if(j<40)
			{
				WriteData((BLACK>>8));
				WriteData(BLACK); 
			}
			else if(j<80)
			{
				WriteData((BLUE>>8));
				WriteData(BLUE);
			}
			else if(j<120)
			{
				WriteData((BRED>>8));
				WriteData(BRED);
			}
			else if(j<160)
			{
				WriteData((GRED>>8));
				WriteData(GRED);
			}
			else if(j<200)
			{
				WriteData((RED>>8));
				WriteData((RED));
			}
			else if(j<240)
			{
				WriteData((GREEN>>8));
				WriteData((GREEN));
			}
			else if(j<280)
			{
				WriteData((YELLOW>>8));
				WriteData((YELLOW));
			}
			else if(j<320)
			{
				WriteData((BROWN>>8));
				WriteData(BROWN);
			}
		}
	}
}
/******************************************
��������Lcdд�����
���ܣ���Lcdָ��λ��д��Ӧ�����������
��ڲ�����Index ҪѰַ�ļĴ�����ַ
          ConfigTemp д������ݻ�����ֵ
******************************************/
void LCD_WR_REG(u16 Index,u16 CongfigTemp)
{
	*(__IO u16 *) (Bank1_LCD_C) = Index;	
	*(__IO u16 *) (Bank1_LCD_D) = CongfigTemp;
}
/************************************************
��������Lcdд��ʼ����
���ܣ�����Lcd�������� ִ��д����
************************************************/
void Lcd_WR_Start(void)
{
*(__IO u16 *) (Bank1_LCD_C) = 0x2C;
}
/**********************************************
��������Lcd��ѡ����
���ܣ�ѡ��Lcd��ָ���ľ�������

ע�⣺xStart��yStart��Xend��Yend������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���

��ڲ�����xStart x�������ʼ��
          ySrart y�������ʼ��
          Xend   y�������ֹ��
          Yend   y�������ֹ��
����ֵ����
***********************************************/
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	//HX8357-C
	
	 WriteComm(0x2A);
   WriteData(Xstart>>8);
   WriteData(Xstart);
   WriteData(Xend>>8);
   WriteData(Xend);
  
   WriteComm(0x2B);
   WriteData(Ystart>>8);
   WriteData(Ystart);
   WriteData(Yend>>8);
	 WriteData(Yend);
	 
	 WriteComm(0x2C);
}
/**********************************************
��������Lcd��ѡ����
���ܣ�ѡ��Lcd��ָ���ľ�������

ע�⣺xStart�� yStart������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���

��ڲ�����xStart x�������ʼ��
          ySrart y�������ֹ��
          xLong Ҫѡ�����ε�x���򳤶�
          yLong  Ҫѡ�����ε�y���򳤶�
����ֵ����
***********************************************/
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color)
{
	u32 temp;

	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	for (temp=0; temp<xLong*yLong; temp++)
	{
		WriteData(Color>>8);
		WriteData(Color);
	}
}

/******************************************
��������Lcdͼ�����100*100
���ܣ���Lcdָ��λ�����ͼ��
��ڲ�����Index ҪѰַ�ļĴ�����ַ
          ConfigTemp д������ݻ�����ֵ
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
  unsigned long i;
//	unsigned int j;

// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x0a); //�����������½ǿ�ʼ�������ң����µ���
	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	
	for (i = 0; i < pic_H*pic_V*2; i++)
	{
// 		WriteData(pic[i]);
		
	//macLCD_RS_1();
			LCD_RS(1);
	SPI_LCD_SendByte( pic[i]);
	
	}
// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0xaa);
}

void DrawPixel(u16 x, u16 y, u16 Color)
{
	BlockWrite(x,x,y,y);
// 	WriteComm(0x200);   
// 	WriteData(x);
// 	WriteComm(0x201);   
// 	WriteData(y);
// 	WriteComm(0x202);
	WriteData(Color>>8);
	WriteData(Color);
// 	*(__IO u16 *) (Bank1_LCD_D) = Color;
}


void PutGB1224_NUM(unsigned short x, unsigned short  y, unsigned char num, unsigned int fColor,unsigned int bColor,unsigned char flag)
{
	unsigned int i,j;
	unsigned short m;

	BlockWrite(x,x+15,y,y+23);
	//for (k=0;k<64;k++) 
	{ //64��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����
	//  if ((codeGB_48[k].Index[0]==c[0])&&(codeGB_48[k].Index[1]==c[1]))
	//	if ((codeGB_48[k].Index[0]==c[0]))
			{ 
			//	for(n=0;n<3;n++)
				{
    	for(i=0;i<48;i++) 
			{
				m=*(sz12X24+num*16*3+i);
			//	m=*(sz12X24+num*16*3+i+n*16);
			//	m=codeGB_48[k].Msk[i];
				for(j=0;j<8;j++) 
				{		
					if((m&0x80)==0x80) {
						DrawPixel(x+j,y,fColor);
						}
					else {
						if(flag) DrawPixel(x+j,y,bColor);
						}
					m=m<<1;
				} 
				if(i%2){y++;x=x-8;}
			else x=x+8;
			//if((i%3)==2){y++;x=x-16;}
			//else x=x+8;
			//	x=x+8;
		  }
		//	y++;x=x-16;
		}
			
		}  
	  }	
	}



void PutGB1224_ACCIS(unsigned short x, unsigned short  y, unsigned char num, unsigned int fColor,unsigned int bColor,unsigned char flag)
{
	unsigned int i,j;
	unsigned short m;

	BlockWrite(x,x+15,y,y+23);
	//for (k=0;k<64;k++) 
	{ //64��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����
	//  if ((codeGB_48[k].Index[0]==c[0])&&(codeGB_48[k].Index[1]==c[1]))
	//	if ((codeGB_48[k].Index[0]==c[0]))
			{ 
				//for(n=0;n<3;n++)
				{
    	for(i=0;i<48;i++) 
			{
				
				m=*(codenAsciiDot+num*16*3+i);
			//	m=codeGB_48[k].Msk[i];
				for(j=0;j<8;j++) 
				{		
					if((m&0x80)==0x80) {
						DrawPixel(x+j,y,fColor);
						}
					else {
						if(flag) DrawPixel(x+j,y,bColor);
						}
					m=m<<1;
				} 
				if(i%2){y++;x=x-8;}
				else x=x+8;
			//if((i%3)==2){y++;x=x-16;}
			//else x=x+8;
			//	x=x+8;
		  }
			//y++;x=x-16;
		}
			
		}  
	  }	
	}

void LCD48_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor,unsigned char flag) 
	{
	 unsigned char l=0;
		unsigned char k;
	while(*s) 
		{
			k=*s;
			if (k>32) k-=32; else k=0;
	//	PutGB1224(x+l*8,y,(unsigned char*)s,fColor,bColor,flag);
	PutGB1224_ACCIS(x+l*8,y,k,fColor,bColor,flag);	
		s+=1;
			l+=2;
		//	s+=1;
		//s==2;
// 		if( *s < 0x80) 
// 		    {
// 			LCD_PutChar(x+l*8,y,*s,fColor,bColor,flag);
// 			s++;l++;
// 			}
// 		else
// 		    {
// 			PutGB1616(x+l*8,y,(unsigned char*)s,fColor,bColor,flag);
// 			s+=2;l+=2;
// 			}
		}
	}
void PutGB2424(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor,unsigned char flag)
{
	unsigned int i,j,k;
	unsigned short m=codeGB_24[k].Msk[i];

	BlockWrite(x,x+23,y,y+23);
	for (k=0;k<64;k++) { //64��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����
	  if ((codeGB_24[k].Index[0]==c[0])&&(codeGB_24[k].Index[1]==c[1]))
			{ 
    	for(i=0;i<72;i++) 
			{
				m=codeGB_24[k].Msk[i];
				for(j=0;j<8;j++) 
				{		
					if((m&0x80)==0x80) {
						DrawPixel(x+j,y,fColor);
						}
					else {
						if(flag) DrawPixel(x+j,y,bColor);
						}
					m=m<<1;
				} 
				if((i%3)==2){y++;x=x-16;}
				else x=x+8;
		  }
		}  
	  }	
	}

void LCD24_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor,unsigned char flag) 
	{
	 unsigned char l=0;
	while(*s) {
		PutGB2424(x+l*8,y,(unsigned char*)s,fColor,bColor,flag);
		s+=2;l+=3;
// 		if( *s < 0x80) 
// 		    {
// 			LCD_PutChar(x+l*8,y,*s,fColor,bColor,flag);
// 			s++;l++;
// 			}
// 		else
// 		    {
// 			PutGB1616(x+l*8,y,(unsigned char*)s,fColor,bColor,flag);
// 			s+=2;l+=2;
// 			}
		}
	}
void PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor,unsigned char flag)
{
	unsigned int i,j,k;
	unsigned short m=codeGB_16[k].Msk[i];

	BlockWrite(x,x+15,y,y+15);
	for (k=0;k<64;k++) { //64��ʾ�Խ����ֿ��еĸ�����ѭ����ѯ����
	  if ((codeGB_16[k].Index[0]==c[0])&&(codeGB_16[k].Index[1]==c[1]))
			{ 
    	for(i=0;i<32;i++) 
			{
				m=codeGB_16[k].Msk[i];
				for(j=0;j<8;j++) 
				{		
					if((m&0x80)==0x80) {
						DrawPixel(x+j,y,fColor);
						}
					else {
						if(flag) DrawPixel(x+j,y,bColor);
						}
					m=m<<1;
				} 
				if(i%2){y++;x=x-8;}
				else x=x+8;
		  }
		}  
	  }	
	}
void LCD_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor,unsigned char flag) 
	{
	 unsigned char l=0;
	while(*s) {
		PutGB1616(x+l*8,y,(unsigned char*)s,fColor,bColor,flag);
		s+=2;l+=2;
// 		if( *s < 0x80) 
// 		    {
// 			LCD_PutChar(x+l*8,y,*s,fColor,bColor,flag);
// 			s++;l++;
// 			}
// 		else
// 		    {
// 			PutGB1616(x+l*8,y,(unsigned char*)s,fColor,bColor,flag);
// 			s+=2;l+=2;
// 			}
		}
	}

void MainDisplay(void)
{
     u8 n;
     n=32;//������к�
		Lcd_ColorBox(0,0,319,2,Blue2);
		Lcd_ColorBox(0,0,2,479,Blue2);
		Lcd_ColorBox(0,477,319,479,Blue2);	
		Lcd_ColorBox(317,0,319,479,Blue2);		
		Lcd_ColorBox(30,0,2,479,Blue2);
		Lcd_ColorBox(160,0,2,352,Blue2);		
		Lcd_ColorBox(32,n*5,287,2,Blue2);				
		Lcd_ColorBox(32,n*8,287,2,Blue2);	
		Lcd_ColorBox(32,n*11,287,2,Blue2);								
		WriteComm(0x36);
		WriteData(0x10);//����Ϊ��������ʾ����
		LCD24_PutString(292,160,"��",Blue2,Black,0);		
		LCD24_PutString(292,184,"��",Blue2,Black,0);
		LCD24_PutString(292,208,"��",Blue2,Black,0);
		LCD24_PutString(292,232,"��",Blue2,Black,0);
		LCD24_PutString(292,256,"��",Blue2,Black,0);
		LCD24_PutString(292,280,"��",Blue2,Black,0);
		LCD24_PutString(292,304,"��",Blue2,Black,0);
		LCD24_PutString(292,328,"˾",Blue2,Black,0);
		LCD24_PutString(2,n*6+2,"����",Red,Black,0);
		LCD24_PutString(2,n*2+2,"ת��",Red,Black,0);	
		LCD24_PutString(2,n*9+2,"������λ��",Red,Black,0);						
		LCD48_PutString(162,n*6+2,"200 Km/h",Magenta,Black,0);	
		LCD48_PutString(162,n*2+2,"80000Rpm",Magenta,Black,0);			
		LCD48_PutString(162,n*9+2,"128    %",Magenta,Black,0);
}
u8 SpeedADC=0;//����  
u16 RotateSpeedADC=0;//ת��
u8  jieqimenweiziADC=0;//������λ��
void MainDisplaynum(u8 u8xianshi,u8 hangshu)
{
u8 liezhi=0;
//u8 n;
 // n=32;//������к�
  //   LCD48_PutString(162,n*5+2,".",Magenta,Black,1);
if((u8xianshi/100)==0)
{
LCD48_PutString((162+((liezhi+2)*16)),hangshu*32+2," ",Magenta,Black,1);
//liezhi++;
}
else
{
PutGB1224_NUM((162+(liezhi*16)),hangshu*32+2,(u8xianshi/100),Magenta,Black,1);
liezhi++;
}
if(((u8xianshi/100)==0)&&((u8xianshi%100/10)==0))
{
LCD48_PutString((162+((liezhi+1)*16)),hangshu*32+2," ",Magenta,Black,1);
//liezhi++;

}
else
{
  PutGB1224_NUM((162+(liezhi*16)),hangshu*32+2,(u8xianshi%100/10),Magenta,Black,1);
  liezhi++;

}

  PutGB1224_NUM((162+(liezhi*16)),hangshu*32+2,(u8xianshi%10),Magenta,Black,1);

}






void MainDisplayzhuanshu(u16 u16xianshi,u8 hangshu)
{
	u8 liezhi=0;


//u8 n;
 // n=32;//������к�
  //   LCD48_PutString(162,n*5+2,".",Magenta,Black,1);


if((u16xianshi/10000)==0)
{
LCD48_PutString((162+((liezhi+4)*16)),hangshu*32+2," ",Magenta,Black,1);
}
else
{
PutGB1224_NUM((162+(liezhi*16)),hangshu*32+2,(u16xianshi/10000),Magenta,Black,1);
liezhi++;

}

if(((u16xianshi/10000)==0)&&((u16xianshi%10000/1000)==0))
{
	LCD48_PutString((162+((liezhi+3)*16)),hangshu*32+2," ",Magenta,Black,1);

}
else
{
  PutGB1224_NUM((162+(liezhi*16)),hangshu*32+2,(u16xianshi%10000/1000),Magenta,Black,1);
  liezhi++;

}
  if(((u16xianshi/10000)==0)&&((u16xianshi%10000/1000)==0)&&((u16xianshi%10000%1000/100)==0))
  	{
  	
	LCD48_PutString((162+((liezhi+2)*16)),hangshu*32+2," ",Magenta,Black,1);
  	}
  else
  {
	  PutGB1224_NUM((162+(liezhi*16)),hangshu*32+2,(u16xianshi%10000%1000/100),Magenta,Black,1);
	  liezhi++;

  }
  
  if(((u16xianshi/10000)==0)&&((u16xianshi%10000/1000)==0)&&((u16xianshi%10000%1000/100)==0)&&((u16xianshi%10000%1000%100/10)==0))
  	{ 	
	LCD48_PutString((162+((liezhi+1)*16)),hangshu*32+2," ",Magenta,Black,1);
  	}
  else
  	{
PutGB1224_NUM((162+(liezhi*16)),hangshu*32+2,(u16xianshi%10000%1000%100/10),Magenta,Black,1);
  liezhi++;

  }
  PutGB1224_NUM((162+(liezhi*16)),hangshu*32+2,(u16xianshi%10000%1000%100%10),Magenta,Black,1);


}

















	
typedef __packed struct
{
	u8  pic_head[2];				//1
	u16 pic_size_l;			    //2
	u16 pic_size_h;			    //3
	u16 pic_nc1;				    //4
	u16 pic_nc2;				    //5
	u16 pic_data_address_l;	    //6
	u16 pic_data_address_h;		//7	
	u16 pic_message_head_len_l;	//8
	u16 pic_message_head_len_h;	//9
	u16 pic_w_l;					//10
	u16 pic_w_h;				    //11
	u16 pic_h_l;				    //12
	u16 pic_h_h;				    //13	
	u16 pic_bit;				    //14
	u16 pic_dip;				    //15
	u16 pic_zip_l;			    //16
	u16 pic_zip_h;			    //17
	u16 pic_data_size_l;		    //18
	u16 pic_data_size_h;		    //19
	u16 pic_dipx_l;			    //20
	u16 pic_dipx_h;			    //21	
	u16 pic_dipy_l;			    //22
	u16 pic_dipy_h;			    //23
	u16 pic_color_index_l;	    //24
	u16 pic_color_index_h;	    //25
	u16 pic_other_l;			    //26
	u16 pic_other_h;			    //27
	u16 pic_color_p01;		    //28
	u16 pic_color_p02;		    //29
	u16 pic_color_p03;		    //30
	u16 pic_color_p04;		    //31
	u16 pic_color_p05;		    //32
	u16 pic_color_p06;		    //33
	u16 pic_color_p07;		    //34
	u16 pic_color_p08;			//35			
}BMP_HEAD;

BMP_HEAD bmp;

 


