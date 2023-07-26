#ifndef RFM300_Driver_h
#define RFM300_Driver_h

#include "driver/gpio.h"
#include "cmt2300_spi.h"
#include "cmt2300_regs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

typedef struct {
	cmt2300_io_t spi;
	int intr1;
    int intr2;
	int intr3;
	bool fixed_pkt_length;
	uint16_t payload_length;
	uint8_t pkt_rssi;
	bool rssi_trig; 
	uint8_t node_id_size;
	uint16_t node_id;

	SemaphoreHandle_t radio_mux;
} cmt2300_dev_t;

typedef void* cmt2300_handle_t;
	
/** Hardware brief **/    
#define GPO1_PIN	8
#define GPO2_PIN	7
#define GPO3_PIN	6

#define TX_ANT    	5
#define RX_ANT	  	4

#define	GPO1In()	do{}while(0) //pinMode(GPO1_PIN,INPUT)
#define GPO1_H() 	do{}while(0) //(digitalRead(GPO1_PIN)==1)	
#define GPO1_L()	do{}while(0) //(digitalRead(GPO1_PIN)==0)

#define	GPO2In()	do{}while(0) //pinMode(GPO2_PIN,INPUT)
#define GPO2_H() 	do{}while(0) //(digitalRead(GPO2_PIN)==1)	
#define GPO2_L()	do{}while(0) //(digitalRead(GPO2_PIN)==0)

#define	GPO3In()	do{}while(0) // pinMode(GPO3_PIN,INPUT)
// #define GPO3_H() 	1//do{}while(0) //(digitalRead(GPO3_PIN)==1)
#define GPIO3_IS_HIGH(IO) (gpio_get_level(IO->intr3) == 1)
// #define GPO3_L()	0//do{}while(0) //(digitalRead(GPO3_PIN)==0)
#define GPIO3_IS_LOW(IO)  (gpio_get_level(IO->intr3) == 0)

//*********************************************************
//System bank
//*********************************************************
#define CMT23_LNA_CTL			0x0C
	
#define CMT23_DUTY_CTL			0x0D

	#define	DUTY_MASK			0xE0
	
	#define	TIMER_RX_EN			(1<<4)
	#define	TIMER_SLEEP_EN		(1<<3)
	#define TX_DUTY_CYC_EN		(1<<2)
	#define	RX_DUTY_CYC_EN		(1<<1)
	#define	DUTY_CYC_PAUSE		(1<<0)		

#define CMT23_EXIT_STATE		0x0E

	#define	EXIT_MASK			0xF0
	
	#define EXIT_TO_SLEEP		0
	#define	EXIT_TO_STBY		1
	#define EXIT_TO_FS			2
	
	#define TX_EXIT_TO_SLEEP	(0<<2)
	#define TX_EXIT_TO_STBY 	(1<<2)
	#define TX_EXIT_TO_FS 		(2<<2)
		
#define CMT23_SL_TIMER_CTL1		0x0F

#define CMT23_SL_TIMER_CTL2 	0x10
	
#define CMT23_RX_TIMER_CTL1		0x11

#define CMT23_RX_TIMER_CTL2 	0x12

#define CMT23_EXT_TIMER_CTL1 	0x13

#define CMT23_EXT_TIMER_CTL2 	0x14	

#define CMT23_EXT_METHOD		0x15

#define CMT23_RSSI_CTL			0x16

#define CMT23_CLK_CTL			0x17

//*********************************************************
//Data rate bank
//*********************************************************
#define	CMT23_CDR_CTL1			0x2B
	
	#define CDR_AVG_MASK		0x8F
	#define CDR_AVG_SHIFT		4
	
	#define CDR_MODE_MASK		0xFC
	#define CDR_MODE_SHIFT		0
	
	#define CDR_RANGE_MASK		0xF3
	#define	CDR_RANGE_SHIFT		2

#define CMT23_CDR_CTL2			0x2C

	#define	CDR_3RD_EN			(1<<6)
	#define	CDR_4TH_EN			(1<<5)

//*********************************************************
//Baseband bank
//*********************************************************	
#define CMT23_PKT1				0x38		//data mode & Preamble unit & Preamble rx size
	#define	PREAM_UNIT_SET		0x04		//0:nibble;  1:uint8_t

#define	CMT23_TX_PREAM_L 		0x39
#define	CMT23_TX_PREAM_H		0x3A
#define CMT23_PREAM_VALUE		0x3B		//
	
#define	CMT23_SYNC_CTL			0x3C		//SyncTol[3] & SyncSize[3] & SyncDcFree 	
	#define	SYNC_MAN_CODE		0x01

#define	CMT23_SYNC_ADDR_0		0x3D		//sync uint16_t base address
#define	CMT23_SYNC_ADDR_7		0x44		//0x3D~0x44 8Bytes SyncValue

#define	CMT23_PKT_CTRL1			0x45		//
	#define	PKT_FORMAT			0x01
	#define	PKT_LSB_PRIO		0x02		//Payload Bigend or Smallend
	#define	NODE_POSITION		0x04		//1:after length;   0:before length
	#define	AUTO_ACK			0x08
	
#define	CMT23_PKT_LEN			0x46		//Low Payload length 
	
#define	CMT23_NODE_CTL			0x47
	#define	NODE_DISABLE		0x00
	#define	NODE_ONLY			0x01		//only
	#define NODE_AND0			0x02		//with broadcast 0
	#define	NODE_ALL			0x03		//with broadcast 0 and 1
	
#define CMT23_NODE_ADDR_0		0x48		//base Node address 
#define CMT23_NODE_ADDR_1		0x49		//second Node address 
#define	CMT23_NODE_ADDR_2		0x4A		//the third node address
#define	CMT23_NODE_ADDR_3		0x4B		//the forth node address	

#define CMT23_CRC_CTL			0x4C
	#define	CRC_ENABLE			(1<<0)
	#define CRC_TYPE_CCITT		(0<<1)
	#define CRC_TYPE_IBM		(1<<1)
	#define CRC_TYPE_ITU		(2<<1)
	#define	CRC_DATA_ONLY		(1<<3)
	#define	CRC_BIT_INV			(1<<4)		//resutlt inverse
	#define	CRC_SWAP			(1<<5)
	#define	FEC_ENABLE			(1<<6)
	#define FEC_TYPE			(1<<7)
	

#define	CMT23_CRC_SEEDL			0x4D
#define	CMT23_CRC_SEEDH			0x4E

#define	CMT23_PKT_CTRL2			0x4F
	#define	MAN_ENABLE			(1<<0)
	#define MAN_TYPE			(1<<1)
	#define	WHT_ENABLE			(1<<2)
	#define	WHT_TYPE_CCITT		(0<<3)
	#define	WHT_TYPE_IBM		(1<<3)
	#define WHT_TYPE_PN7		(2<<3)
	#define WHT_PN7_SEED		(1<<5)

#define CMT23_WHT_SEED			0x50

#define	CMT23_PREFIX			0x51

#define	CMT23_TX_PKT_NUM		0x52

#define	CMT23_TX_PKT_GAP    	0x53
	
#define	CMT23_FIFO_TH			0x54
	#define FIFO_AUTO_RESTORE_EN (1<<7)
	
#define	CMT23_MODE_CTL			0x60
	#define	MODE_GO_EEPROM		(1<<0)			//0x01
	#define	MODE_GO_STBY		(1<<1)			//0x02
	#define	MODE_GO_RFS			(1<<2)			//0x04
	#define	MODE_GO_RX			(1<<3)			//0x08
	#define	MODE_GO_SLEEP		(1<<4)			//0x10
	#define	MODE_GO_TFS			(1<<5)			//0x20
	#define	MODE_GO_TX			(1<<6)			//0x40
	#define	MODE_GO_SWITCH		(1<<7)			//0x80		//only use for "Tx to Rx" or "Rx to Tx"
	
	
#define	CMT23_MODE_STA			0x61	
	#define RSTN_IN_EN			(1<<5)			//0x20
	#define	EEP_CPY_DIS			(1<<4)			//0x10		//import
	#define	MODE_MASK_STA		0x0F
	#define	MODE_STA_IDLE		0x00
	#define	MODE_STA_SLEEP		0x01
	#define	MODE_STA_STBY		0x02
	#define	MODE_STA_RFS		0x03
	#define MODE_STA_TFS		0x04
	#define	MODE_STA_RX			0x05
	#define	MODE_STA_TX			0x06
	#define	MODE_STA_EEPROM		0x07
	#define	MODE_STA_TUNE		0x08
	
	
#define	CMT23_EN_CTL			0x62
	#define	LD_STOP_EN			(1<<5)			//0x20
	#define	LBD_STOP_EN			(1<<4)			//0x10
	#define	EEP_STOP			(1<<3)			//0x08
	#define	EEP_START			(1<<2)			//0x04
	#define	EEP_PWRON			(1<<1)			//0x02
	#define	EEP_NPROTECT		(1<<0)			//0x01

#define	CMT23_FREQ_CHNL			0x63			//Channel Number	

#define	CMT23_FREQ_OFS			0x64			//Channel OFFSET	

#define	CMT23_IO_SEL			0x65
	#define	GPIO1_DIO			(0<<0)			//0x00
	#define GPIO1_INT1			(1<<0)			//0x01
	#define	GPIO1_INT2			(2<<0)			//0x02
	#define GPIO1_DCLK			(3<<0)			//0x03
	
	#define	GPIO2_INT1			(0<<2)			//0x00
	#define	GPIO2_INT2			(1<<2)			//0x04
	#define	GPIO2_DIO			(2<<2)			//0x08
	#define	GPIO2_DCLK			(3<<2)			//0x0C
	
	#define	GPIO3_CLKO			(0<<4)			//0x00
	#define GPIO3_DIO			(1<<4)			//0x10
	#define	GPIO3_INT2			(2<<4)			//0x20
	#define	GPIO3_DCLK			(3<<4)			//0x30
	
	#define	GPIO4_NRST			(0<<6)			//0x00
	#define	GPIO4_INT1			(1<<6)			//0x40
	#define	GPIO4_DIO			(2<<6)			//0x80
	#define	GPIO4_DCLK			(3<<6)			//0xC0

#define	CMT23_INT1_CTL			0x66
	#define	RF_SWT1_EN 			(1<<7)			//GPIO1 浼氳緭鍑? RX_ACTIVE锛孏PIO2 浼氳緭鍑? TX_ACTIVE
	#define RF_SWT2_EN          (1<<6)			//GPIO1 浼氳緭鍑? RX_ACTIVE锛? GPIO2 浼氳緭鍑? RX_ACTIVE 鍙栧弽锛屽嵆瀹屽叏宸垎
	#define	INT_POLAR			(1<<5)
	#define	INT_MASK			0xE0
		
#define	CMT23_INT2_CTL			0x67
	#define LOSC_OUT_EN			0x40
	#define	TX_DATA_INV			0x20
	#define	INT_RX_ACTIVE		0x00	
	#define	INT_TX_ACTIVE		0x01
	#define	INT_RSSI_VALID		0x02
	#define	INT_PREAM_PASS		0x03
	#define	INT_SYNC_PASS		0x04
	#define	INT_NODE_PASS		0x05
	#define	INT_CRC_PASS		0x06
	#define	INT_PKT_DONE		0x07
	#define	INT_SLEEP_TMO		0x08
	#define	INT_RX_TMO			0x09
	#define	INT_TX_DONE			0x0A
	#define	INT_FIFO_NMTY_RX	0x0B
	#define INT_FIFO_TH_RX		0x0C
	#define	INT_FIFO_FULL_RX	0x0D
	#define	INT_FIFO_WBYTE_RX	0x0E
	#define	INT_FIFO_OVF_RX		0x0F
	#define	INT_FIFO_NMTY_TX	0x10
	#define	INT_FIFO_TH_TX		0x11
	#define	INT_FIFO_FULL_TX	0x12
	#define	INT_STBY_STATE		0x13
	#define	INT_FS_STATE		0x14
	#define	INT_RX_STATE		0x15
	#define	INT_TX_STATE		0x16
	#define	INT_LBD_STATE		0x17
	#define	INT_TRX_ACTIVE		0x18		//?
	#define INT_PKT_ERR_COL		0x19

#define	CMT23_INT_EN			0x68
	#define	SLEEP_TMO_EN		(1<<7)		//0x80
	#define	RX_TMO_EN			(1<<6)		//0x40
	#define	TX_DONE_EN			(1<<5)		//0x20
	#define	PREAMBLE_PASS_EN	(1<<4)		//0x10
	#define	SYNC_PASS_EN		(1<<3)		//0x08
	#define	NODE_PASS_EN		(1<<2)		//0x04
	#define	CRC_PASS_EN			(1<<1)		//0x02
	#define	PKT_DONE_EN			(1<<0)		//0x01

#define	CMT23_FIFO_CTL			0x69
	#define	DIN_EN				(1<<7)
  #define DIN_DIS       (0<<7)
	#define GPIO1_DIN			(0<<5)
	#define GPIO2_DIN			(1<<5)
	#define GPIO3_DIN			(2<<5)
	#define	RX_FIFO_CLR_DIS		(1<<4)		//0: Entry Rx, auto clear fifo
	#define	TX_FIFO_RD_EN		(1<<3)		//0: Tx FIFO can not be read
	#define	FIFO_RX_TX_SEL		(1<<2)		//when "FIFO_SHARE_EN=1" active, 0=use for Rx; 1=use for Tx
	#define	FIFO_SHARE_EN		(1<<1)		//0: not share, both 32Byte for Tx/Rx FIFO; 1: total 64Byte
	#define	SPI_FIFO_RD_WR_SEL	(1<<0)		//0: SPI use to read FIFO;  1: SPI use to write FIFO;

#define CMT23_INT_CLR1			0x6A
	#define	SLEEP_TIMEOUT_FLAG	(1<<5)		//0x20
	#define	RX_TIMEOUT_FLAG		(1<<4)		//0x10
	#define	TX_DONE_FLAG		(1<<3)		//0x08
	#define	TX_DONE_CLR			(1<<2)		//0x04
	#define	SLEEP_TIMEOUT_CLR	(1<<1)		//0x02
	#define	RX_TIMEOUT_CLR		(1<<0)		//0x01	

#define	CMT23_INT_CLR2			0x6B
	#define	LBD_CLR				(1<<5)		//0x20
	#define	PREAMBLE_PASS_CLR	(1<<4)		//0x10
	#define	SYNC_PASS_CLR		(1<<3)		//0x08
	#define	NODE_PASS_CLR		(1<<2)		//0x04
	#define	CRC_PASS_CLR		(1<<1)		//0x02
	#define	RX_DONE_CLR			(1<<0)		//0x01

#define	CMT23_FIFO_CLR			0x6C
	#define	FIFO_RESTORE		(1<<2)		//0x04
	#define	FIFO_CLR_RX			(1<<1)		//0x02
	#define	FIFO_CLR_TX			(1<<0)		//0x01

#define	CMT23_INT_FLG			0x6D
	#define	LBD_STATUS_FLAG		(1<<7)		//0x80
	#define	COLLISION_ERR_FLAG	(1<<6)		//0x40
	#define	DC_ERR_FLAG			(1<<5)		//0x20
	#define	PREAMBLE_PASS_FLAG	(1<<4)		//0x10
	#define	SYNC_PASS_FLAG		(1<<3)		//0x08
	#define	NODE_PASS_FLAG		(1<<2)		//0x04
	#define	CRC_PASS_FLAG		(1<<1)		//0x02
	#define	RX_DONE_FLAG		(1<<0)		//0x01

#define	CMT23_FIFO_FLG			0x6E
	#define RX_FIFO_FULL_FLAG	(1<<6)		//0x40
	#define	RX_FIFO_NMTY_FLAG	(1<<5)		//0x20
	#define	RX_FIFO_THRESH_FLAG	(1<<4)		//0x10
	#define	RX_FIFO_OV_FLAG		(1<<3)		//0x08	
	#define	TX_FIFO_FULL_FLAG	(1<<2)		//0x04
	#define	TX_FIFO_NMTY_FLAG	(1<<1)		//0x02
	#define	TX_FIFO_THRESH_FLAG	(1<<0)		//0x01
	
#define	CMT23_RSSI_CODE			0x6F
#define	CMT23_RSSI_DBM			0x70
#define	CMT23_LBD_RESULT		0x71

#define	CMT23_EE_DAT_LOW		0x76
#define	CMT23_EE_DAT_HIG		0x77
#define	CMT23_EE_ADD_USER		0x78
#define	CMT23_EE_CTL			0x79
#define	CMT23_EE_ADD_CONF		0x7A
#define	CMT23_EE_EXT_CTL		0x7B
#define	CMT23_SOFTRST			0x7F

	
// extern	uint8_t  FixedPktLength;						//false: for contain packet length in Tx message, the same mean with variable lenth
 		                                            //true : for doesn't include packet length in Tx message, the same mean with fixed length
// extern	uint16_t  PayloadLength;						//unit: uint8_t  range: 1-2047                                      
		
// extern	uint8_t  PktRssi;
		
// extern	uint8_t  RssiTrig;

// extern uint8_t  node_id_size;			//0: none,disable,  max:4
// extern uint16_t  node_id;			   //16bit,

	 		                                            
//State Ctrl
uint8_t bGoTx(cmt2300_handle_t handle);
uint8_t bGoRx(cmt2300_handle_t handle);
uint8_t bGoSleep(cmt2300_handle_t handle);	
// uint8_t bGoStandby(void);	
uint8_t bGoStandby(cmt2300_handle_t handle);
// void vSoftReset(void);
void vSoftReset(cmt2300_handle_t handle);

uint8_t bReadStatus(cmt2300_handle_t handle);
uint8_t bReadRssi(cmt2300_handle_t handle, uint8_t unit_dbm);

//GPIO & Interrupt CFG
void vGpioFuncCfg(cmt2300_handle_t handle, uint8_t io_cfg);
void vIntSrcCfg(cmt2300_handle_t handle, uint8_t int_1, uint8_t int_2);
void vEnableAntSwitch(cmt2300_handle_t handle, uint8_t mode);					//
void vIntSrcEnable(cmt2300_handle_t handle, uint8_t en_int);
// uint8_t bIntSrcFlagClr(void);
uint8_t bIntSrcFlagClr(cmt2300_handle_t handle);
uint8_t vClearFIFO(cmt2300_handle_t handle);
void vEnableRdFifo(cmt2300_handle_t handle);
void vEnableWrFifo(cmt2300_handle_t handle);
void vSetTxPayloadLength(cmt2300_handle_t handle, uint16_t length);
void vEnableDIN(cmt2300_handle_t handle, uint8_t pin);			//add by vison
void vDisableDIN(cmt2300_handle_t handle);

//CFG
// void vInit(void);
// void vCfgBank(uint16_t cfg[], uint8_t length);
void vCfgBank(cmt2300_handle_t handle, uint16_t cfg[], uint8_t length);


// uint8_t bGetMessage(uint8_t msg[]);
uint8_t bGetMessage(cmt2300_handle_t handle, uint8_t msg[]);
uint8_t bGetMessageByFlag(cmt2300_handle_t handle, uint8_t msg[]);
uint8_t bSendMessage(cmt2300_handle_t handle, uint8_t msg[], uint8_t length);

//control
void vSetClockDiv(cmt2300_handle_t handle, uint8_t div, uint8_t enable);
void set_node_id(cmt2300_handle_t handle, uint16_t node);
uint16_t get_node_id(cmt2300_handle_t handle);
uint8_t get_node_setting(cmt2300_handle_t handle);


cmt2300_handle_t cmt2300_init(int csb, int fcsb, int sclk, int sdio, int intr1, int intr2, int intr3, int spi_hw_if);
void cmt2300_send_data(cmt2300_handle_t handle, uint8_t* data, int len);
int cmt2300_recv_data(cmt2300_handle_t handle, uint8_t* buf, int *rssi, TickType_t ticks_to_wait);
					 	
#else
	#warning "HopeDuino_CMT2300A.h have been defined!"

#endif
