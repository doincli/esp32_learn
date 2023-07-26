#include "cmt2300_spi.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

#if !USE_HW_SPI
/**********************************************************
**Name: 	vSpi3Init
**Func: 	Init Spi-3 Config
**Note: 	
**********************************************************/
void vSpi3Init(cmt2300_io_t* io)
{
	gpio_config_t io_conf;
	io_conf.intr_type = 0;
	io_conf.mode = GPIO_MODE_INPUT_OUTPUT;
	io_conf.pin_bit_mask = (1ULL << (io->csb)) | (1ULL << (io->fcsb)) | (1ULL << (io->sclk)) | (1ULL << (io->sdio)) ;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	SET_CSB(io);
	SET_FCSB(io);
	SET_SDIO(io);
	CLR_SDIO(io);
}

/**********************************************************
**Name: 	vSpi3WriteByte
**Func: 	SPI-3 send one uint8_t
**Input:
**Output:  
**********************************************************/
void vSpi3WriteByte(cmt2300_io_t* io, uint8_t dat)
{
 	uint8_t bitcnt;	
 
	SET_FCSB(io); 
	SET_SDIO_MODE_OUTPUT(io);
	SET_SDIO(io);
 	CLR_SCLK(io);		
	CLR_CSB(io);

 	for(bitcnt=8; bitcnt!=0; bitcnt--) {
		CLR_SCLK(io);
		_delay_us(SPI3_SPEED);
 		if(dat&0x80) {
			SET_SDIO(io);
		} else {
			CLR_SDIO(io);
		}
		SET_SCLK(io);
 		dat <<= 1; 		
 		_delay_us(SPI3_SPEED);
	}
	CLR_SCLK(io);
	SET_SDIO(io);
}

/**********************************************************
**Name: 	bSpi3ReadByte
**Func: 	SPI-3 read one uint8_t
**Input:
**Output:  
**********************************************************/
uint8_t bSpi3ReadByte(cmt2300_io_t* io)
{
	uint8_t RdPara = 0;
 	uint8_t bitcnt;
  
	CLR_CSB(io);
	SET_SDIO_MODE_INPUT(io);		
 	
 	for(bitcnt=8; bitcnt!=0; bitcnt--) {
		CLR_SCLK(io);
 		RdPara <<= 1;
 		_delay_us(SPI3_SPEED);
		SET_SCLK(io);
 		_delay_us(SPI3_SPEED);
		if (SDIO_IS_HIGH(io)) {
 			RdPara |= 0x01;
		} else {
 			RdPara |= 0x00;
		}
	} 
	CLR_SCLK(io);
	SET_SDIO_MODE_OUTPUT(io);
	SET_SDIO(io);

	_delay_us(SPI3_SPEED);
	SET_CSB(io);	
	_delay_us(SPI3_SPEED);
	
 	return(RdPara);	
}

/**********************************************************
**Name:	 	vSpi3Write
**Func: 	SPI Write One uint16_t
**Input: 	Write uint16_t
**Output:	none
**********************************************************/
void vSpi3Write(cmt2300_io_t* io, uint16_t dat)
{
 	vSpi3WriteByte(io, (uint8_t)(dat>>8)&0x7F);
 	vSpi3WriteByte(io, (uint8_t)dat);
	_delay_us(SPI3_SPEED);
	SET_CSB(io);
	_delay_us(SPI3_SPEED);
}

/**********************************************************
**Name:	 	bSpi3Read
**Func: 	SPI-3 Read One uint8_t
**Input: 	readout addresss
**Output:	readout uint8_t
**********************************************************/
uint8_t bSpi3Read(cmt2300_io_t* io, uint8_t addr)
{
  	vSpi3WriteByte(io, addr|0x80);
 	return(bSpi3ReadByte(io));
}

/**********************************************************
**Name:	 	vSpi3WriteFIFO
**Func: 	SPI-3 send one uint8_t to FIFO
**Input: 	one uint8_t buffer
**Output:	none
**********************************************************/
void vSpi3WriteFIFO(cmt2300_io_t* io, uint8_t dat)
{
 	uint8_t bitcnt;		
	SET_CSB(io);
	SET_SDIO_MODE_OUTPUT(io);	
	CLR_SCLK(io);
	CLR_FCSB(io);

	for(bitcnt=8; bitcnt!=0; bitcnt--) {
		CLR_SCLK(io);
 		
 		if(dat&0x80) {
			SET_SDIO(io);	
		} else {
			CLR_SDIO(io);
		}
		_delay_us(SPI3_SPEED);
		SET_SCLK(io);
		_delay_us(SPI3_SPEED);
 		dat <<= 1;
	}
	CLR_SCLK(io);
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
	SET_FCSB(io);
	SET_SDIO(io);
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
}

/**********************************************************
**Name:	 	bSpi3ReadFIFO
**Func: 	SPI-3 read one uint8_t to FIFO
**Input: 	none
**Output:	one uint8_t buffer
**********************************************************/
uint8_t bSpi3ReadFIFO(cmt2300_io_t* io)
{
	uint8_t RdPara = 0;
 	uint8_t bitcnt;	
 	
	SET_CSB(io);
	SET_SDIO_MODE_INPUT(io);
	CLR_SCLK(io);
	CLR_FCSB(io);
		
 	for(bitcnt=8; bitcnt!=0; bitcnt--) {
		CLR_SCLK(io);
 		RdPara <<= 1;
 		_delay_us(SPI3_SPEED);
		SET_SCLK(io);
		_delay_us(SPI3_SPEED);
 		if(SDIO_IS_HIGH(io)) {
 			RdPara |= 0x01;		//NRZ MSB
		} else {
 		 	RdPara |= 0x00;		//NRZ MSB
		}
	}
 	
	CLR_SCLK(io);
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
	SET_FCSB(io);
	SET_SDIO_MODE_OUTPUT(io);
	SET_SDIO(io);
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	_delay_us(SPI3_SPEED);		//Time-Critical
 	return(RdPara);
}

/**********************************************************
**Name:	 	vSpi3BurstWriteFIFO
**Func: 	burst wirte N uint8_t to FIFO
**Input: 	array length & head pointer
**Output:	none
**********************************************************/
void vSpi3BurstWriteFIFO(cmt2300_io_t* io, uint8_t ptr[], uint8_t length)
{
 	uint8_t i;
 	if(length!=0x00) {
 		for(i=0;i<length;i++) {
 			vSpi3WriteFIFO(io, ptr[i]);
		}
	}
 	return;
}

/**********************************************************
**Name:	 	vSpiBurstRead
**Func: 	burst wirte N uint8_t to FIFO
**Input: 	array length  & head pointer
**Output:	none
**********************************************************/
void vSpi3BurstReadFIFO(cmt2300_io_t* io, uint8_t ptr[], uint8_t length)
{
	uint8_t i;
 	if(length!=0) {
 		for(i=0;i<length;i++) {
 			ptr[i] = bSpi3ReadFIFO(io);
		}
	}	
 	return;
}

#endif