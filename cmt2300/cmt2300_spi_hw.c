#include "cmt2300_spi.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

#if USE_HW_SPI
/**********************************************************
**Name: 	vSpi3Init
**Func: 	Init Spi-3 Config
**Note: 	
**********************************************************/
void vSpi3Init(cmt2300_io_t* io)
{
    spi_bus_config_t buscfg = {
        .mosi_io_num = io->sdio ,//PIN_NUM_MOSI,
        .miso_io_num = -1,//PIN_NUM_MOSI,
        .sclk_io_num = io->sclk,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        // .max_transfer_sz = 4096 * 3
    };
    spi_device_interface_config_t reg_devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .clock_speed_hz = 5*1000*1000,
        .duty_cycle_pos = 128,
        .mode = 0,
        .spics_io_num = io->csb,
        .queue_size = 10,
		.flags = SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_3WIRE,
    };
	spi_device_interface_config_t fifo_devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .clock_speed_hz = 1*1000*1000,
        .duty_cycle_pos = 128,
        .mode = 0,
        .spics_io_num = io->fcsb,
        .queue_size = 10,
		.flags = SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_3WIRE,
    };

    spi_bus_initialize(io->spi_if, &buscfg, 0);
    spi_bus_add_device(io->spi_if, &reg_devcfg,  &io->reg_spi);
    spi_bus_add_device(io->spi_if, &fifo_devcfg, &io->fifo_spi);
}

/**********************************************************
**Name:	 	vSpi3Write
**Func: 	SPI Write One uint16_t
**Input: 	Write uint16_t
**Output:	none
**********************************************************/
void vSpi3Write(cmt2300_io_t* io, uint16_t dat)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 2 * 8;
	t.tx_data[0] = (dat>>8) & 0xff;
	t.tx_data[1] = dat & 0xff;
	t.flags = SPI_TRANS_USE_TXDATA;
    spi_device_transmit(io->reg_spi, &t);
}

/**********************************************************
**Name:	 	bSpi3Read
**Func: 	SPI-3 Read One uint8_t
**Input: 	readout addresss
**Output:	readout uint8_t
**********************************************************/
uint8_t bSpi3Read(cmt2300_io_t* io, uint8_t addr)
{
    spi_transaction_ext_t t;
    memset(&t, 0, sizeof(t));
    t.address_bits = 8;
    t.base.addr = addr | 0x80;
    t.base.rxlength = 1 * 8;
    t.base.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_VARIABLE_ADDR;

    spi_device_transmit(io->reg_spi, &t);
	return t.base.rx_data[0];
}

/**********************************************************
**Name:	 	vSpi3WriteFIFO
**Func: 	SPI-3 send one uint8_t to FIFO
**Input: 	one uint8_t buffer
**Output:	none
**********************************************************/
void vSpi3WriteFIFO(cmt2300_io_t* io, uint8_t dat)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 1 * 8;
	t.tx_data[0] = dat;
	t.flags = SPI_TRANS_USE_TXDATA;
    // printf("Transmitting FIFO %d bytes: 0x%02x\n", 1, dat);
    spi_device_transmit(io->fifo_spi, &t);
}

/**********************************************************
**Name:	 	bSpi3ReadFIFO
**Func: 	SPI-3 read one uint8_t to FIFO
**Input: 	none
**Output:	one uint8_t buffer
**********************************************************/
uint8_t bSpi3ReadFIFO(cmt2300_io_t* io)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.rxlength = 1 * 8;
	t.flags = SPI_TRANS_USE_RXDATA;
    spi_device_transmit(io->fifo_spi, &t);
	// printf("Reading FIFO %d bytes: 0x%02x\n", 1, val);
	return t.rx_data[0];
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