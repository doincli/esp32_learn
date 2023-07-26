#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "rfm300_driver.h"
#include "esp_log.h"

static const char* TAG = "RFM300"; 
/**********************************************************
State Ctrl
**********************************************************/
/**********************************************************
**Name:     bGoTx
**Function: Entry Tx Mode
**Input:    none
**Output:   none
**********************************************************/
uint8_t bGoTx(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint8_t tmp, i;

	#if 0
	vSpi3Write(((uint16_t)CMT23_MODE_CTL<<8)+MODE_GO_TFS);	
	for(i=0; i<100; i++){
		_delay_us(100);	
		tmp = (MODE_MASK_STA & bSpi3Read(CMT23_MODE_STA));	
		if(tmp==MODE_STA_TFS)
			break;
	}
	if(i>=100)
		return(false);
	#endif

	vSpi3Write(&dev->spi, ((uint16_t)CMT23_MODE_CTL<<8)+MODE_GO_TX);		
	for(i=0; i<100; i++){
		_delay_us(100);	
		tmp = (MODE_MASK_STA & bSpi3Read(&dev->spi, CMT23_MODE_STA));	
		if(tmp==MODE_STA_TX)
			break;
	}
	if(i>=100)
		return(false);
	else
		return(true);
}

/**********************************************************
**Name:     bGoRx
**Function: Entry Rx Mode
**Input:    none
**Output:   none
**********************************************************/
uint8_t bGoRx(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint8_t tmp, i;
	#if 0
	dev->rssi_trig = false;
	vSpi3Write(((uint16_t)CMT23_MODE_CTL<<8)+MODE_GO_RFS);	
	for(i=0; i<100; i++){
		_delay_us(100);	
		tmp = (MODE_MASK_STA & bSpi3Read(CMT23_MODE_STA));	
		if(tmp==MODE_STA_RFS)
			break;
	}
	if(i>=100)
		return(false);
	#endif

	vSpi3Write(&dev->spi, ((uint16_t)CMT23_MODE_CTL<<8)+MODE_GO_RX);		
	for(i=0; i<100; i++){
		_delay_us(100);	
		tmp = (MODE_MASK_STA & bSpi3Read(&dev->spi, CMT23_MODE_STA));	
		if(tmp==MODE_STA_RX)
			break;
	}
	if(i>=100)
		return(false);
	else
		return(true);
}

/**********************************************************
**Name:     bGoSleep
**Function: Entry Sleep Mode
**Input:    none
**Output:   none
**********************************************************/
uint8_t bGoSleep(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint8_t tmp;

	vSpi3Write(&dev->spi, ((uint16_t)CMT23_MODE_CTL<<8)+MODE_GO_SLEEP);	
	_delay_us(100);		//enough?
	tmp = (MODE_MASK_STA & bSpi3Read(&dev->spi, CMT23_MODE_STA));	
	if(tmp==MODE_GO_SLEEP)
		return(true);
	else
		return(false);
}

/**********************************************************
**Name:     bGoStandby
**Function: Entry Standby Mode
**Input:    none
**Output:   none
**********************************************************/
uint8_t bGoStandby(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	uint8_t tmp, i;	

	dev->rssi_trig = false;
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_MODE_CTL<<8)+MODE_GO_STBY);	
	for(i=0; i<100; i++){
		_delay_us(100);	
		tmp = (MODE_MASK_STA & bSpi3Read(&dev->spi, CMT23_MODE_STA));	
		if(tmp==MODE_STA_STBY) {
			break;
		}
	}

	if(i>=100) {
		return(false);
	} else {
		return(true);
	}
}

/**********************************************************
**Name:     vSoftReset
**Function: Software reset Chipset
**Input:    none
**Output:   none
**********************************************************/
void vSoftReset(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	vSpi3Write(&dev->spi, ((uint16_t)CMT23_SOFTRST<<8)+0xFF); 
	_delay_ms(1);				//enough?
}

/**********************************************************
**Name:     bReadStatus
**Function: read chipset status
**Input:    none
**Output:   none
**********************************************************/
uint8_t bReadStatus(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	return(MODE_MASK_STA & bSpi3Read(&dev->spi, CMT23_MODE_STA));		
}

/**********************************************************
**Name:     bReadRssi
**Function: Read Rssi
**Input:    true------dBm;
            false-----Code;
**Output:   none
**********************************************************/
uint8_t bReadRssi(cmt2300_handle_t handle, uint8_t unit_dbm)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	if(unit_dbm)
		return(bSpi3Read(&dev->spi, CMT23_RSSI_DBM));
	else		
		return(bSpi3Read(&dev->spi, CMT23_RSSI_CODE));
}

void set_node_id(cmt2300_handle_t handle, uint16_t node)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	vSpi3Write(&dev->spi, (uint16_t)(CMT23_NODE_ADDR_3<<8)| ((node>>8)&0x00FF));
	vSpi3Write(&dev->spi, (uint16_t)(CMT23_NODE_ADDR_2<<8)| (node&0x00FF));	

	//vSpi3Write((uint16_t)(CMT23_NODE_ADDR_3<<8)|node&0x00FF);
}

uint16_t get_node_id(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint16_t node;
	node = bSpi3Read(&dev->spi, CMT23_NODE_ADDR_3);	   //msb first
	node <<= 8;
	node |= bSpi3Read(&dev->spi, CMT23_NODE_ADDR_2);    //second uint8_t
	return node;
}

uint8_t get_node_setting(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	 return bSpi3Read(&dev->spi, 0x47);	//node condigure register
}

/**********************************************************
GPIO & Interrupt CFG
**********************************************************/
/**********************************************************
**Name:     vGpioFuncCfg
**Function: GPIO Function config
**Input:    none
**Output:   none
**********************************************************/
void vGpioFuncCfg(cmt2300_handle_t handle, uint8_t io_cfg)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_IO_SEL<<8)+io_cfg);
}

/**********************************************************
**Name:     vIntSrcCfg
**Function: config interrupt source  
**Input:    int_1, int_2
**Output:   none
**********************************************************/
void vIntSrcCfg(cmt2300_handle_t handle, uint8_t int_1, uint8_t int_2)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint8_t tmp;
	tmp = INT_MASK & bSpi3Read(&dev->spi, CMT23_INT1_CTL);
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_INT1_CTL<<8)+(tmp|int_1));

	tmp = INT_MASK & bSpi3Read(&dev->spi, CMT23_INT2_CTL);
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_INT2_CTL<<8)+(tmp|int_2));
}

/**********************************************************
**Name:     vEnableDIN
**Function: for direct mode, add by vison
**Input:    none
**Output:   
**********************************************************/
void vEnableDIN(cmt2300_handle_t handle, uint8_t pin)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint8_t tmp;
	tmp = 0x1F & bSpi3Read(&dev->spi, CMT23_FIFO_CTL);
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_FIFO_CTL<<8)+(tmp|DIN_EN|pin));
}

/**********************************************************
**Name:     vEnableDIN
**Function: for direct mode, add by vison
**Input:    none
**Output:   
**********************************************************/
void vDisableDIN(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint8_t tmp;
	tmp = 0x1F & bSpi3Read(&dev->spi, CMT23_FIFO_CTL);
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_FIFO_CTL<<8)+(tmp|DIN_DIS));
}
/**********************************************************
**Name:     vEnableAntSwitch
**Function:  
**Input:    
**Output:   none
**********************************************************/
void vEnableAntSwitch(cmt2300_handle_t handle, uint8_t mode)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint8_t tmp;
	tmp = bSpi3Read(&dev->spi, CMT23_INT1_CTL);
	tmp&= 0x3F;
	switch(mode)
	{
		case 1:
			tmp |= RF_SWT1_EN; break;		//GPO1=RxActive; GPO2=TxActive	
		case 2:
			tmp |= RF_SWT2_EN; break;		//GPO1=RxActive; GPO2=!RxActive
		case 0:
		default:
			break;							//Disable	
	}
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_INT1_CTL<<8)+tmp);
}


/**********************************************************
**Name:     vIntSrcEnable
**Function: enable interrupt source 
**Input:    en_int
**Output:   none
**********************************************************/
void vIntSrcEnable(cmt2300_handle_t handle, uint8_t en_int)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_INT_EN<<8)+en_int);				
}

/********* ***************/
void vSetClockDiv(cmt2300_handle_t handle, uint8_t div, uint8_t enable)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint8_t tmp;
	tmp = bSpi3Read(&dev->spi, CMT23_CLK_CTL);
	// Serial.println(tmp,HEX); 	//print old value
	ESP_LOGI(TAG, "CMT23_CLK_CTL: 0x%02x\n", tmp);
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_CLK_CTL<<8)+((tmp&0xC0)|(enable<<5)|div)); 
	// Serial.println(((uint16_t)CMT23_CLK_CTL<<8)+(tmp&0xC0|enable<<5|div),HEX);
	ESP_LOGI(TAG, "check val: 0x%04x\n", ((uint16_t)CMT23_CLK_CTL<<8)+((tmp&0xC0)|enable<<5|div));
}


/**********************************************************
**Name:     vIntSrcFlagClr
**Function: clear flag
**Input:    none
**Output:   equ CMT23_INT_EN
**********************************************************/
uint8_t bIntSrcFlagClr(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	//uint8_t tmp;
	//uint8_t int_clr2 = 0;
	//uint8_t int_clr1 = 0;
	uint8_t flg = 0;
#if 0
	tmp = bSpi3Read(CMT23_INT_FLG);
	if(tmp&LBD_STATUS_FLAG)		//LBD_FLG_Active
		int_clr2 |= LBD_CLR;

	if(tmp&PREAMBLE_PASS_FLAG)		//Preamble Active
	{
		int_clr2 |= PREAMBLE_PASS_CLR;
		flg |= PREAMBLE_PASS_EN;
	}
	if(tmp&SYNC_PASS_FLAG)			//Sync Active
	{
		int_clr2 |= SYNC_PASS_CLR;		
		flg |= SYNC_PASS_EN;		
	}
	if(tmp&NODE_PASS_FLAG)			//Node Addr Active
	{
		int_clr2 |= NODE_PASS_CLR;	
		flg |= NODE_PASS_EN;
	}
	if(tmp&CRC_PASS_FLAG)			//Crc Pass Active
	{
		int_clr2 |= CRC_PASS_CLR;
		flg |= CRC_PASS_EN;
	}
	if(tmp&RX_DONE_FLAG)			//Rx Done Active
	{
		int_clr2 |= RX_DONE_CLR;
		flg |= PKT_DONE_EN;
	}
		
	if(tmp&COLLISION_ERR_FLAG)		
		int_clr2 |= RX_DONE_CLR;
	if(tmp&DC_ERR_FLAG)
		int_clr2 |= RX_DONE_CLR;
		
	vSpi3Write(((uint16_t)CMT23_INT_CLR2<<8)+int_clr2);	//Clear flag

	tmp = bSpi3Read(CMT23_INT_CLR1);
	if(tmp&TX_DONE_FLAG)
	{
		int_clr1 |= TX_DONE_CLR;
		flg |= TX_DONE_EN;
	}	
	if(tmp&SLEEP_TIMEOUT_FLAG)
	{
		int_clr1 |= SLEEP_TIMEOUT_CLR;
		flg |= SLEEP_TMO_EN;
	} 
	if(tmp&RX_TIMEOUT_FLAG)
	{
		int_clr1 |= RX_TIMEOUT_CLR;
		flg |= RX_TMO_EN;
	}	
	vSpi3Write(((uint16_t)CMT23_INT_CLR1<<8)+int_clr1);	//Clear flag 
#endif
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_INT_CLR1<<8)+0x07); //Clear flag 
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_INT_CLR2<<8)+0xFF);	//Clear flag 

	return(flg);
}

/**********************************************************
**Name:     bClearFIFO
**Function: clear FIFO buffer
**Input:    none
**Output:   FIFO state
**********************************************************/
uint8_t vClearFIFO(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint8_t tmp;	
	tmp = bSpi3Read(&dev->spi, CMT23_FIFO_FLG);
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_FIFO_CLR<<8)+FIFO_CLR_RX+FIFO_CLR_TX);
	return(tmp);
}

#if 1		//splite fifo, tx:32bytes, rx:32bytes;
void vEnableWrFifo(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;
	uint8_t tmp;
	tmp = bSpi3Read(&dev->spi, CMT23_FIFO_CTL);
	tmp |= (SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL);
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_FIFO_CTL<<8)+tmp);
}

void vEnableRdFifo(cmt2300_handle_t handle)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	uint8_t tmp;
	tmp = bSpi3Read(&dev->spi, CMT23_FIFO_CTL);
	tmp &= (~(SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL));
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_FIFO_CTL<<8)+tmp);
}
#endif

#if 0		//modify by vison
void vEnableWrFifo(void)
{
	uint8_t tmp;
	tmp = bSpi3Read(CMT23_FIFO_CTL);
	tmp |= (SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL);
	vSpi3Write(((uint16_t)CMT23_FIFO_CTL<<8)+tmp|FIFO_SHARE_EN);	//enbale merge fifo
}

void vEnableRdFifo(void)
{
	uint8_t tmp;
	tmp = bSpi3Read(CMT23_FIFO_CTL);
	tmp &= (~(SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL));
	vSpi3Write(((uint16_t)CMT23_FIFO_CTL<<8)+tmp|FIFO_SHARE_EN);	//enable merge fifo
}
#endif

void vCfgBank(cmt2300_handle_t handle, uint16_t cfg[], uint8_t length)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	uint8_t i;

	if(length!=0){	
		for(i=0; i<length; i++)	
			vSpi3Write(&dev->spi, cfg[i]);
	}	
}

uint8_t bGetMessage(cmt2300_handle_t handle, uint8_t msg[])
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	uint8_t i;	

	vEnableRdFifo(dev);	
	if(dev->fixed_pkt_length){
		vSpi3BurstReadFIFO(&dev->spi, msg, dev->fixed_pkt_length);
		i = dev->fixed_pkt_length;
	}else{
		i = bSpi3ReadFIFO(&dev->spi);
		vSpi3BurstReadFIFO(&dev->spi, msg, i);
	}
	return(i);
}

uint8_t bGetMessageByFlag(cmt2300_handle_t handle, uint8_t msg[])
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	uint8_t tmp;
	uint8_t tmp1;
	uint8_t rev = 0;
	tmp = bSpi3Read(&dev->spi, CMT23_INT_FLG);
	if((tmp&SYNC_PASS_FLAG)&&(!dev->rssi_trig)){
		dev->pkt_rssi = bReadRssi(handle, false);
		dev->rssi_trig = true;
	}

	tmp1 = bSpi3Read(&dev->spi, CMT23_CRC_CTL);
	vEnableRdFifo(handle);	 
	if(tmp1&CRC_ENABLE)		//Enable CrcCheck
	{
		if(tmp&CRC_PASS_FLAG){
			if(dev->fixed_pkt_length){
				vSpi3BurstReadFIFO(&dev->spi, msg, dev->fixed_pkt_length);
				rev = dev->fixed_pkt_length;
			}else{	
				rev = bSpi3ReadFIFO(&dev->spi);	
				vSpi3BurstReadFIFO(&dev->spi, msg, rev);
			}
			dev->rssi_trig = false;
		}
	}else{
		if(tmp&RX_DONE_FLAG){
			if(dev->fixed_pkt_length){
				vSpi3BurstReadFIFO(&dev->spi, msg, dev->fixed_pkt_length);
				rev = dev->fixed_pkt_length;
			}else{	
				rev = bSpi3ReadFIFO(&dev->spi);	
				vSpi3BurstReadFIFO(&dev->spi, msg, rev);
			}	
			dev->rssi_trig = false;		
		}
	}
 
	if(tmp&COLLISION_ERR_FLAG)rev = 0xFF;
	return(rev);
}

uint8_t bSendMessage(cmt2300_handle_t handle, uint8_t msg[], uint8_t length)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	bIntSrcFlagClr(handle);
#if 1	
	//mode1
	vSetTxPayloadLength(handle, length);
	bGoStandby(handle);
	vEnableWrFifo(handle);	
	vSpi3BurstWriteFIFO(&dev->spi, msg, length);
	bGoTx(handle);
#else
	//mode2
	vSetTxPayloadLength(length);
	bGoTx();
	vEnableWrFifo();	
	vSpi3BurstWriteFIFO(msg, length);
#endif	
	return(true);
}

void vSetTxPayloadLength(cmt2300_handle_t handle,  uint16_t length)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	uint8_t tmp;	
	uint8_t len;
	bGoStandby(handle);
	tmp = bSpi3Read(&dev->spi, CMT23_PKT_CTRL1);
	tmp&= 0x8F;

	if(length!=0){
		if(dev->fixed_pkt_length){
		  tmp &= ~(1<<0);		//clear bit0
		  len = length - 1;
		}
		else{
		  tmp |= (1<<0);	   //set bit0
		  len = length;
		}
	}else
		len = 0;

	tmp|= (((uint8_t)(len>>8)&0x07)<<4);
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_PKT_CTRL1<<8)+tmp);
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_PKT_LEN<<8)+(uint8_t)len);	//Payload length
}









static void cmt2300_wait_tx_done(cmt2300_dev_t* dev)
{
	if (dev->intr3 >= 0) {
		while(GPIO3_IS_LOW(dev));
	} else {
		while (1) {
			uint8_t tmp = bSpi3Read(&dev->spi, CMT23_INT_CLR1);
			if(tmp & TX_DONE_FLAG) {
				break;
			}
		}
	}
}


void cmt2300_send_data(cmt2300_handle_t handle, uint8_t* data, int len)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	xSemaphoreTake(dev->radio_mux, portMAX_DELAY);

	vIntSrcCfg(dev, INT_FIFO_NMTY_TX, INT_TX_DONE);    //IRQ maping, set INT2 as TX_DONE(on GPIO3)
	vIntSrcEnable(dev, TX_DONE_EN);                   
	vClearFIFO(dev);
	bGoSleep(dev);

	bSendMessage(dev, data, len);
	// while(GPIO3_IS_LOW(dev));
	cmt2300_wait_tx_done(dev);

	bIntSrcFlagClr(dev);
	vClearFIFO(dev); 
	bGoSleep(dev);
	xSemaphoreGive(dev->radio_mux);
}

static bool cmt2300_is_rx_done(cmt2300_dev_t* dev)
{
	if (dev->intr3 >= 0) {
		return GPIO3_IS_HIGH(dev);
	} else {
		uint8_t tmp = bSpi3Read(&dev->spi, CMT23_INT_FLG);
		return ((tmp & RX_DONE_FLAG) == RX_DONE_FLAG);
	}
}

int cmt2300_recv_data(cmt2300_handle_t handle, uint8_t* buf, int *rssi, TickType_t ticks_to_wait)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) handle;

	xSemaphoreTake(dev->radio_mux, portMAX_DELAY);

	vIntSrcCfg(dev, INT_FIFO_WBYTE_RX, INT_PKT_DONE);    //IRQ  Maping, set INT2 as PKT_DONE (on GPIO3)
	vIntSrcEnable(dev, CRC_PASS_EN|PKT_DONE_EN); 
	bIntSrcFlagClr(dev);
	vClearFIFO(dev);
	bGoSleep(dev);
	bGoRx(dev); 

	TickType_t start_tick = xTaskGetTickCount();

	int len = 0;
	while (1) {
		// if(GPIO3_IS_HIGH(dev)) {
		if (cmt2300_is_rx_done(dev)) {
			*rssi = bReadRssi(dev, 1)-128;
			len =bGetMessage(dev, buf);
			bIntSrcFlagClr(dev);
			vClearFIFO(dev); 
			break;
		} else {
			vTaskDelay(10/portTICK_PERIOD_MS);
			TickType_t now = xTaskGetTickCount();
			if (now - start_tick > ticks_to_wait) {
				*rssi = 0;
				len = 0;
				break;
			}
		}
	}
	xSemaphoreGive(dev->radio_mux);
	return len;
}


/**********************************************************
CFG
**********************************************************/
/**********************************************************
**Name:     cmt2300_init
**Function: Init. CMT2300A
**Input:    none
**Output:   none
**********************************************************/
cmt2300_handle_t cmt2300_init(int csb, int fcsb, int sclk, int sdio, int intr1, int intr2, int intr3, int spi_hw_if)
{
	cmt2300_dev_t* dev = (cmt2300_dev_t*) calloc(1, sizeof(cmt2300_dev_t));

	dev->spi.csb = csb;
	dev->spi.fcsb = fcsb;
	dev->spi.sclk = sclk;
	dev->spi.sdio = sdio;
	dev->spi.spi_if = spi_hw_if;

	dev->intr1 = intr1;
	dev->intr2 = intr2;
	dev->intr3 = intr3;

	uint8_t tmp;
	vSpi3Init(&dev->spi);

	uint64_t bit_mask = 0;
	if(dev->intr1 >= 0) {
		bit_mask |= (1ULL << dev->intr1);
	}
	if(dev->intr2 >= 0) {
		bit_mask |= (1ULL << dev->intr2);
	}
	if(dev->intr3 > 0) {
		bit_mask |= (1ULL << dev->intr3);
	}
	if (bit_mask) {
		gpio_config_t io_conf;
		io_conf.intr_type = 0;
		io_conf.mode = GPIO_MODE_INPUT;
		io_conf.pin_bit_mask = bit_mask;
		io_conf.pull_down_en = 0;
		io_conf.pull_up_en = 1;
		gpio_config(&io_conf);
	}

	dev->radio_mux = xSemaphoreCreateMutex();
	dev->fixed_pkt_length = false;
	
	// TX_ANTOut(); // check
	// RX_ANTOut(); // check

	vSoftReset(dev);
	_delay_ms(20);
	bGoStandby(dev);
	tmp = bSpi3Read(&dev->spi, CMT23_MODE_STA);
	tmp|= EEP_CPY_DIS;
	tmp&= (~RSTN_IN_EN);			//Disable RstPin	
	vSpi3Write(&dev->spi, ((uint16_t)CMT23_MODE_STA<<8)+tmp);
	bIntSrcFlagClr(dev);
	vCfgBank(dev, CMTBank, 12);
	vCfgBank(dev, SystemBank, 12);
	vCfgBank(dev, FrequencyBank, 8);
	vCfgBank(dev, DataRateBank, 24);
	vCfgBank(dev, BasebandBank, 29);
	vCfgBank(dev, TXBank, 11);
	vEnableAntSwitch(dev, 1); // GPO1=RxActive; GPO2=TxActive	
	vGpioFuncCfg(dev, GPIO1_DCLK+GPIO2_DCLK+GPIO3_INT2);   //GPIO Maping, set GPIO3 as INT2

	return (cmt2300_handle_t) dev;
}

