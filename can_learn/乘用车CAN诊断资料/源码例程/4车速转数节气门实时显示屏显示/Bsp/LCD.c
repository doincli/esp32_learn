/******************************************************************************

*重要说明！
在.h文件中，#define Immediately时是立即显示当前画面
而如果#define Delay，则只有在执行了LCD_WR_REG(0x0007,0x0173);
之后才会显示，执行一次LCD_WR_REG(0x0007,0x0173)后，所有写入数
据都立即显示。
#define Delay一般用在开机画面的显示，防止显示出全屏图像的刷新过程
******************************************************************************/
#include "stm32f10x.h"
#include "LCD.h"
//#include "ff.h"	
#include <stdio.h>
//#include "FONT.h"
#include "GB1616.h"	//16*16汉字字模
#include "sheep.h"
//#include "spi.h"

/************************************************************************
  * @描述:   LCD所用IO引脚配置
  * @参数:   None
  * @返回值: None
  **********************************************************************/

 void LCD_SPI_Config ( void )
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE );	

	//SPI1口初始化
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin =  SPI1_MOSI| SPI1_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin = SPI1_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SPI1, &GPIO_InitStructure);
  GPIO_ResetBits(GPIO_SPI1,SPI1_CS);
	
	/* SPI1 configuration */                                            //初始化SPI1结构体
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1设置为两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //设置SPI1为主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                    //串行时钟在不操作时，时钟为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                    //第一个时钟沿开始采样数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS信号由软件（使用SSI位）管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  //SPI波特率预分频值为8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRC值计算的多项式

	SPI_Init(SPI1, &SPI_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPI1寄存器
	
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);                                              //使能SPI1外设


  RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );//LCD RS引脚配置 
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
Lcd初始化函数
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
//S6D05A1初始化
	
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
WriteData(0x60); //横屏，从左下角开始，从左到右，从下到上


//test_color();
//Lcd_Light_ON;
//macLCD_BL_1();

#if 1
WriteComm(0x36);//竖屏
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
//WriteComm(0x36);//竖屏
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
函数名：Lcd写命令函数
功能：向Lcd指定位置写入应有命令或数据
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_WR_REG(u16 Index,u16 CongfigTemp)
{
	*(__IO u16 *) (Bank1_LCD_C) = Index;	
	*(__IO u16 *) (Bank1_LCD_D) = CongfigTemp;
}
/************************************************
函数名：Lcd写开始函数
功能：控制Lcd控制引脚 执行写操作
************************************************/
void Lcd_WR_Start(void)
{
*(__IO u16 *) (Bank1_LCD_C) = 0x2C;
}
/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart、yStart、Xend、Yend随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的起始点
          Xend   y方向的终止点
          Yend   y方向的终止点
返回值：无
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
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart和 yStart随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong 要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
返回值：无
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
函数名：Lcd图像填充100*100
功能：向Lcd指定位置填充图像
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
  unsigned long i;
//	unsigned int j;

// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x0a); //横屏，从左下角开始，从左到右，从下到上
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
	{ //64标示自建汉字库中的个数，循环查询内码
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
	{ //64标示自建汉字库中的个数，循环查询内码
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
	for (k=0;k<64;k++) { //64标示自建汉字库中的个数，循环查询内码
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
	for (k=0;k<64;k++) { //64标示自建汉字库中的个数，循环查询内码
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
     n=32;//字体加行号
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
		WriteData(0x10);//设置为横屏的显示方向
		LCD24_PutString(292,160,"华",Blue2,Black,0);		
		LCD24_PutString(292,184,"行",Blue2,Black,0);
		LCD24_PutString(292,208,"技",Blue2,Black,0);
		LCD24_PutString(292,232,"术",Blue2,Black,0);
		LCD24_PutString(292,256,"有",Blue2,Black,0);
		LCD24_PutString(292,280,"限",Blue2,Black,0);
		LCD24_PutString(292,304,"公",Blue2,Black,0);
		LCD24_PutString(292,328,"司",Blue2,Black,0);
		LCD24_PutString(2,n*6+2,"车速",Red,Black,0);
		LCD24_PutString(2,n*2+2,"转速",Red,Black,0);	
		LCD24_PutString(2,n*9+2,"节气门位置",Red,Black,0);						
		LCD48_PutString(162,n*6+2,"200 Km/h",Magenta,Black,0);	
		LCD48_PutString(162,n*2+2,"80000Rpm",Magenta,Black,0);			
		LCD48_PutString(162,n*9+2,"128    %",Magenta,Black,0);
}
u8 SpeedADC=0;//车速  
u16 RotateSpeedADC=0;//转速
u8  jieqimenweiziADC=0;//节气门位置
void MainDisplaynum(u8 u8xianshi,u8 hangshu)
{
u8 liezhi=0;
//u8 n;
 // n=32;//字体加行号
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
 // n=32;//字体加行号
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

 


