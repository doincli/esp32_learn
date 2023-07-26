#ifndef CMT2300_SPI_H
#define CMT2300_SPI_H

#include <string.h>
#include <stdint.h>
#include "driver/spi_master.h"

#include "driver/gpio.h"

#define USE_HW_SPI     1
// SW SPI
#define SPI3_SPEED	   20
extern void ets_delay_us(uint32_t);
#define  _delay_us(us)  ets_delay_us(us)
#define  _delay_ms(ms)  ets_delay_us(ms*1000)

/** Hardware brief **/
typedef struct {
    int csb;
    int fcsb;
    int sclk;
    int sdio;
    int spi_if;
    spi_device_handle_t reg_spi;
    spi_device_handle_t fifo_spi;
} cmt2300_io_t;


#define SET_CSB(IO) gpio_set_level(IO->csb, 1)
#define CLR_CSB(IO) gpio_set_level(IO->csb, 0)

#define SET_FCSB(IO) gpio_set_level(IO->fcsb, 1)
#define CLR_FCSB(IO) gpio_set_level(IO->fcsb, 0)

#define SET_SCLK(IO) gpio_set_level(IO->sclk, 1)
#define CLR_SCLK(IO) gpio_set_level(IO->sclk, 0)

#define SET_SDIO(IO) gpio_set_level(IO->sdio, 1)
#define CLR_SDIO(IO) gpio_set_level(IO->sdio, 0)

#define SET_SDIO_MODE_OUTPUT(IO)  gpio_set_direction(IO->sdio, GPIO_MODE_OUTPUT)
#define SET_SDIO_MODE_INPUT(IO)   gpio_set_direction(IO->sdio, GPIO_MODE_INPUT)

#define SDIO_IS_HIGH(IO) (gpio_get_level(IO->sdio) == 1)
#define SDIO_IS_LOW(IO)  (gpio_get_level(IO->sdio) == 0)

void vSpi3Init(cmt2300_io_t* io);				/** initialize software SPI-3 **/
void vSpi3Write(cmt2300_io_t* io, uint16_t dat);			/** SPI-3 send one uint16_t **/
uint8_t bSpi3Read(cmt2300_io_t* io, uint8_t addr);			/** SPI-3 read one uint8_t **/

void vSpi3WriteFIFO(cmt2300_io_t* io, uint8_t dat);		/** SPI-3 send one uint8_t to FIFO **/
uint8_t bSpi3ReadFIFO(cmt2300_io_t* io);			/** SPI-3 read one uint8_t from FIFO **/
void vSpi3BurstWriteFIFO(cmt2300_io_t* io, uint8_t ptr[], uint8_t length);			/** SPI-3 burst send N uint8_t to FIFO**/
void vSpi3BurstReadFIFO(cmt2300_io_t* io, uint8_t ptr[], uint8_t length);			/** SPI-3 burst read N uint8_t to FIFO**/

void vSpi3WriteByte(cmt2300_io_t* io, uint8_t dat);		/** SPI-3 send one uint8_t **/
uint8_t bSpi3ReadByte(cmt2300_io_t* io);			/** SPI-3 read one uint8_t **/

#else
#warning "cmt2300_spi.h have been defined!"
#endif
