#include "ebyte_e220x.h"

#define XTAL_FREQ                                   ( double )32000000
#define FREQ_DIV                                    ( double )33554432 
#define FREQ_STEP                                   ( double )( XTAL_FREQ / FREQ_DIV )
#define XTA_TRIM_VALUE                              0x1C

#define LORA_PREAMBLE_LENGTH                        60         

#define CRC_IBM_SEED                                0xFFFF
#define CRC_CCITT_SEED                              0x1D0F
#define CRC_POLYNOMIAL_IBM                          0x8005
#define CRC_POLYNOMIAL_CCITT                        0x1021

#define REG_LR_CRCSEEDBASEADDR                      0x06BC
#define REG_LR_CRCPOLYBASEADDR                      0x06BE
#define REG_LR_WHITSEEDBASEADDR_MSB                 0x06B8
#define REG_LR_WHITSEEDBASEADDR_LSB                 0x06B9
#define REG_LR_PACKETPARAMS                         0x0704
#define REG_LR_PAYLOADLENGTH                        0x0702
#define REG_LR_SYNCWORDBASEADDRESS                  0x06C0
#define REG_LR_SYNCWORD                             0x0740

#define RANDOM_NUMBER_GENERATORBASEADDR             0x0819

#define REG_RX_GAIN                                 0x08AC
#define REG_XTA_TRIM                                0x0911
#define REG_XTB_TRIM                                0x0912
#define REG_OCP                                     0x08E7


#define    RADIO_GET_STATUS                          0xC0 
#define    RADIO_WRITE_REGISTER                      0x0D 
#define    RADIO_READ_REGISTER                       0x1D 
#define    RADIO_WRITE_BUFFER                        0x0E 
#define    RADIO_READ_BUFFER                         0x1E 
#define    RADIO_SET_SLEEP                           0x84 
#define    RADIO_SET_STANDBY                         0x80 
#define    RADIO_SET_FS                              0xC1 
#define    RADIO_SET_TX                              0x83 
#define    RADIO_SET_RX                              0x82 
#define    RADIO_SET_RXDUTYCYCLE                     0x94 
#define    RADIO_SET_CAD                             0xC5 
#define    RADIO_SET_TXCONTINUOUSWAVE                0xD1 
#define    RADIO_SET_TXCONTINUOUSPREAMBLE            0xD2 
#define    RADIO_SET_PACKETTYPE                      0x8A 
#define    RADIO_GET_PACKETTYPE                      0x11 
#define    RADIO_SET_RFFREQUENCY                     0x86 
#define    RADIO_SET_TXPARAMS                        0x8E 
#define    RADIO_SET_PACONFIG                        0x95 
#define    RADIO_SET_CADPARAMS                       0x88 
#define    RADIO_SET_BUFFERBASEADDRESS               0x8F 
#define    RADIO_SET_MODULATIONPARAMS                0x8B 
#define    RADIO_SET_PACKETPARAMS                    0x8C 
#define    RADIO_GET_RXBUFFERSTATUS                  0x13 
#define    RADIO_GET_PACKETSTATUS                    0x14 
#define    RADIO_GET_RSSIINST                        0x15 
#define    RADIO_GET_STATS                           0x10 
#define    RADIO_RESET_STATS                         0x00 
#define    RADIO_CFG_DIOIRQ                          0x08 
#define    RADIO_GET_IRQSTATUS                       0x12 
#define    RADIO_CLR_IRQSTATUS                       0x02 
#define    RADIO_CALIBRATE                           0x89 
#define    RADIO_CALIBRATEIMAGE                      0x98 
#define    RADIO_SET_REGULATORMODE                   0x96 
#define    RADIO_GET_ERROR                           0x17 
#define    RADIO_CLR_ERROR                           0x07 
#define    RADIO_SET_TCXOMODE                        0x97 
#define    RADIO_SET_TXFALLBACKMODE                  0x93 
#define    RADIO_SET_RFSWITCHMODE                    0x9D 
#define    RADIO_SET_STOPRXTIMERONPREAMBLE           0x9F 
#define    RADIO_SET_LORASYMBTIMEOUT                 0xA0 

#define    CHIP_MODE_STBY_RC                         0x02
#define    CHIP_MODE_STBY_XOSC                       0x03
#define    CHIP_MODE_FS                              0x04
#define    CHIP_MODE_RX                              0x05
#define    CHIP_MODE_X                               0x06 

/* Lora communication baudrate */
static const int8_t Ebyte_E220x_LoraBpsTable[8][3] =
{
  {LORA_BW_500 , LORA_SF11 , LORA_CR_4_5},  /* Lora 0.3 kbps  */
  {LORA_BW_500 , LORA_SF11 , LORA_CR_4_5},  /* Lora 1.2 kbps  */
  {LORA_BW_500 , LORA_SF11 , LORA_CR_4_5},  /* Lora 2.4 kbps  */
  {LORA_BW_250 , LORA_SF8  , LORA_CR_4_5},  /* Lora 4.8 kbps  */
  {LORA_BW_500 , LORA_SF8  , LORA_CR_4_6},  /* Lora 9.6 kbps  */
  {LORA_BW_500 , LORA_SF7  , LORA_CR_4_6},  /* Lora 19.2 kbps */
  {LORA_BW_500 , LORA_SF5  , LORA_CR_4_8},  /* Lora 38.4 kbps */ 
  {LORA_BW_500 , LORA_SF5  , LORA_CR_4_5},  /* Lora 62.5 kbps */ 
};

 /*!
 * FSK bandwidth definition
 */
typedef struct
{
    uint32_t bandwidth;
    uint8_t  RegValue;
}FskBandwidth_t;

/*!
 * Precomputed FSK bandwidth registers values
 */
const FskBandwidth_t FskBandwidths[] =
{
    { 4800  , 0x1F },
    { 5800  , 0x17 },
    { 7300  , 0x0F },
    { 9700  , 0x1E },
    { 11700 , 0x16 },
    { 14600 , 0x0E },
    { 19500 , 0x1D },
    { 23400 , 0x15 },
    { 29300 , 0x0D },
    { 39000 , 0x1C },
    { 46900 , 0x14 },
    { 58600 , 0x0C },
    { 78200 , 0x1B },
    { 93800 , 0x13 },
    { 117300, 0x0B },
    { 156200, 0x1A },
    { 187200, 0x12 },
    { 234300, 0x0A },
    { 312000, 0x19 },
    { 373600, 0x11 },
    { 467000, 0x09 },
    { 500000, 0x00 }, // Invalid Bandwidth
};

/***********************************ebyte esp config*****************************************/
void Ebyte_esp_gpio_init( ebyte_config_t ebyte_config )
{
    gpio_config_t rst_io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = ebyte_config.rst_io,
        .pull_down_en = 0,
        .pull_up_en = 1,
    };
    gpio_config(&rst_io_conf);

    gpio_config_t busy_io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = ebyte_config.busy_io,
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&busy_io_conf);

    // dio1 used for output irq signal in radio chip, set -1 for disable pin
    if (ebyte_config.dio1_io != -1) {
        gpio_config_t irq_io_conf = {
            .intr_type = GPIO_INTR_DISABLE,
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = ebyte_config.dio1_io,
            .pull_down_en = 0,
            .pull_up_en = 1,
        };
        gpio_config(&irq_io_conf);
    }

    // rxio used for output ant rx enable signal to radio chip, set -1 for disable pin
    if (ebyte_config.rxen_io != -1) {
        gpio_config_t rxen_io_conf = {
            .intr_type = GPIO_INTR_DISABLE,
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = ebyte_config.rxen_io,
            .pull_down_en = 0,
            .pull_up_en = 1,
        };
        gpio_config(&rxen_io_conf);
    }

    // tx_io used for output ant tx enable signal to radio chip, set -1 for disable pin
    if (ebyte_config.txen_io != -1) {
        gpio_config_t txen_io_conf = {
            .intr_type = GPIO_INTR_DISABLE,
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = ebyte_config.txen_io,
            .pull_down_en = 0,
            .pull_up_en = 1,
        };
        gpio_config(&txen_io_conf);
    }
}

void Ebyte_esp_spi_init( spi_device_handle_t* spi, spi_host_device_t spi_host, int miso_io, int mosi_io, int clk_io, int cs_io, int clk_speed )
{
    esp_err_t ret;
    spi_bus_config_t buscfg = {
        .miso_io_num = miso_io,
        .mosi_io_num = mosi_io,
        .sclk_io_num = clk_io,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = clk_speed,              //Clock out at 10 MHz
        .mode = 0,                                //SPI mode 0
        .spics_io_num = cs_io,                    //CS pin
        .queue_size = 7,                          //We want to be able to queue 7 transactions at a time
    };
    //Initialize the SPI bus
    ret = spi_bus_initialize(spi_host, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);
    //Attach the EBYTE to the SPI bus
    ret = spi_bus_add_device(spi_host, &devcfg, spi);
    ESP_ERROR_CHECK(ret);
}
/***********************************ebyte esp config*****************************************/


/* !
 * @brief reset LLCC68
 *
 * @note  Ebyte_E220x_Reset
 */
void Ebyte_E220x_Reset( ebyte_status_t *ebyte_status )
{
    Ebyte_Port_DelayMs( 10 );
    Ebyte_Port_IoControl( ebyte_status->rst_io, 0 );
    Ebyte_Port_DelayMs( 20 );
    Ebyte_Port_IoControl( ebyte_status->rst_io, 1 );
    Ebyte_Port_DelayMs( 20 );
}

/* !
 * @brief wait on busy status
 *
 * @note  Ebyte_E220x_WaitOnBusy
 */
void Ebyte_E220x_WaitOnBusy( int busy_io )
{
    while( Ebyte_Port_IoRead(busy_io) );
}

/* !
 * @brief wake up LLCC68
 *
 * @note  Ebyte_E220x_Wakeup
 */
void Ebyte_E220x_Wakeup( ebyte_status_t *ebyte_status )
{
    uint8_t commands[2] = {RADIO_GET_STATUS, 0x00};
    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 0 );
#endif

    Ebyte_Port_SpiTransmitAndReceivce( ebyte_status->spi, commands, 2, NULL, 0 );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 1 );
#endif

    Ebyte_E220x_WaitOnBusy( ebyte_status->busy_io );
}

/* !
 * @brief check the radio chip is ready or not 
 */
void Ebyte_E220x_CheckReady ( ebyte_status_t *ebyte_status )
{
    if ( ( ebyte_status->OperatingMode == MODE_SLEEP ) || ( ebyte_status->OperatingMode == MODE_RX_DC ) )
    {
        Ebyte_E220x_Wakeup (ebyte_status);
    }
    Ebyte_E220x_WaitOnBusy (ebyte_status->busy_io);
}

/* !
 * @brief write command to radio chip
 *
 * @param command RadioCommands_t
 * @param buffer  command data
 * @param size    size of command bytes
 */
void Ebyte_E220x_WriteCommand( ebyte_status_t *ebyte_status, uint8_t command, uint8_t *buffer, uint16_t size )
{
    uint8_t *commands = (uint8_t *)malloc((size + 1) * sizeof(uint8_t));
    assert( commands != NULL );

    commands[0] = command;
    memcpy(commands + 1, buffer, size * sizeof(uint8_t));

    Ebyte_E220x_CheckReady( ebyte_status );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 0 );
#endif

    Ebyte_Port_SpiTransmitAndReceivce( ebyte_status->spi, commands, size + 1, NULL, 0 );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 1 );
#endif

    free(commands);
    if( command != RADIO_SET_SLEEP )
    {
        Ebyte_E220x_WaitOnBusy( ebyte_status->busy_io );
    }

}

/* !
 * @brief read command from radio chip
 *
 * @param command RadioCommands_t
 * @param buffer  command data
 * @param size    size of command bytes
 */
void Ebyte_E220x_ReadCommand( ebyte_status_t *ebyte_status, uint8_t command, uint8_t *buffer, uint16_t size )
{
    uint8_t commands[2];

    commands[0] = command;
    commands[1] = 0x0;

    Ebyte_E220x_CheckReady( ebyte_status );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 0 );
#endif

    Ebyte_Port_SpiTransmitAndReceivce( ebyte_status->spi, commands, 2, buffer, size );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 1 );
#endif
    Ebyte_E220x_WaitOnBusy( ebyte_status->busy_io );
}

/* !
 * @brief write registers to radio chip
 *
 * @param address target reg addr
 * @param buffer  reg data
 * @param size    data size
 */
void Ebyte_E220x_WriteRegisters( ebyte_status_t *ebyte_status, uint16_t address, uint8_t *buffer, uint16_t size )
{
    uint8_t *commands = (uint8_t *)malloc((size + 3) * sizeof(uint8_t));
    assert( commands != NULL );

    commands[0] = RADIO_WRITE_REGISTER;
    commands[1] = ( address & 0xFF00 ) >> 8;
    commands[2] = address & 0x00FF;
    memcpy(commands + 3, buffer, size * sizeof(uint8_t));

    Ebyte_E220x_CheckReady( ebyte_status );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 0 );
#endif

    Ebyte_Port_SpiTransmitAndReceivce( ebyte_status->spi, commands, size + 3, NULL, 0 );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 1 );
#endif

    free(commands);
    Ebyte_E220x_WaitOnBusy( ebyte_status->busy_io );
}

/* !
 * @brief write register to radio chip
 *
 * @param address target reg addr
 * @param value   value to write
 */
void Ebyte_E220x_WriteRegister( ebyte_status_t *ebyte_status, uint16_t address, uint8_t value )
{
    Ebyte_E220x_WriteRegisters( ebyte_status, address, &value, 1 );
}

/* !
 * @brief read registers from radio chip
 *
 * @param address target reg addr
 * @param buffer  reg data
 * @param size    data size
 */
void Ebyte_E220x_ReadRegisters( ebyte_status_t *ebyte_status, uint16_t address, uint8_t *buffer, uint16_t size )
{
    uint8_t commands[4];

    commands[0] = RADIO_READ_REGISTER;
    commands[1] = ( address & 0xFF00 ) >> 8;
    commands[2] = address & 0x00FF;
    commands[3] = 0x0;

    Ebyte_E220x_CheckReady( ebyte_status );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 0 );
#endif

    Ebyte_Port_SpiTransmitAndReceivce( ebyte_status->spi, commands, 4, buffer, size );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 1 );
#endif

    Ebyte_E220x_WaitOnBusy( ebyte_status->busy_io );
}

/* !
 * @brief  read register from radio chip
 *
 * @param  address target reg addr
 * @return data of reg
 */
uint8_t Ebyte_E220x_ReadRegister( ebyte_status_t *ebyte_status, uint16_t address )
{
    uint8_t data;
    Ebyte_E220x_ReadRegisters( ebyte_status, address, &data, 1 );
    return data;
}

/* !
 * @brief write buffer to radio chip
 *
 * @param offset  buffer offset of radio chip
 * @param buffer  data of buffer
 * @param size    size of data
 */
void Ebyte_E220x_WriteBuffer( ebyte_status_t *ebyte_status, uint8_t offset, uint8_t *buffer, uint8_t size )
{
    uint8_t *commands = (uint8_t *)malloc((size + 2) * sizeof(uint8_t));
    assert( commands != NULL );

    commands[0] = RADIO_WRITE_BUFFER;
    commands[1] = offset;
    memcpy(commands + 2, buffer, size * sizeof(uint8_t));

    Ebyte_E220x_CheckReady( ebyte_status );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 0 );
#endif

    Ebyte_Port_SpiTransmitAndReceivce( ebyte_status->spi, commands, size + 2, NULL, 0 );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 1 );
#endif

    free(commands);
    Ebyte_E220x_WaitOnBusy( ebyte_status->busy_io );
}

/* !
 * @brief write buffer from radio chip
 *
 * @param offset  buffer offset of radio chip
 * @param buffer  data of buffer
 */
void Ebyte_E220x_ReadBuffer( ebyte_status_t *ebyte_status, uint8_t offset )
{
    uint8_t commands[3];

    commands[0] = RADIO_READ_BUFFER;
    commands[1] = offset;
    commands[2] = 0x0;

    Ebyte_E220x_CheckReady( ebyte_status );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 0 );
#endif

    Ebyte_Port_SpiTransmitAndReceivce( ebyte_status->spi, commands, 3, ebyte_status->RecvPacketBuffer, ebyte_status->recv_len );

    /* SPI Ƭѡ CS */
#if EBYTE_PORT_SPI_CS_SOFTWARE
    Ebyte_Port_IoControl( ebyte_status->cs_io, 1 );
#endif

    Ebyte_E220x_WaitOnBusy( ebyte_status->busy_io );
}
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/


void Ebyte_E220x_SetStandby( ebyte_status_t *ebyte_status, RadioStandbyModes_t standbyConfig )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_STANDBY, ( uint8_t* )&standbyConfig, 1 );
    if( standbyConfig == STDBY_RC )
    {
        ebyte_status->OperatingMode = MODE_STDBY_RC;
    }
    else
    {
        ebyte_status->OperatingMode = MODE_STDBY_XOSC;
    }
}


void Ebyte_E220x_Calibrate( ebyte_status_t *ebyte_status, CalibrationParams_t calibParam )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_CALIBRATE, ( uint8_t* )&calibParam, 1 );
}


void Ebyte_E220x_SetDio3AsTcxoCtrl( ebyte_status_t *ebyte_status, RadioTcxoCtrlVoltage_t tcxoVoltage, uint32_t timeout )
{
    uint8_t buf[4];

    buf[0] = tcxoVoltage & 0x07;
    buf[1] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
    buf[2] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
    buf[3] = ( uint8_t )( timeout & 0xFF );

    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_TCXOMODE, buf, 4 );
}


void Ebyte_E220x_SetDio2AsRfSwitchCtrl( ebyte_status_t *ebyte_status, uint8_t enable )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_RFSWITCHMODE, &enable, 1 );
}

void Ebyte_E220x_GetRxBufferStatus( ebyte_status_t *ebyte_status, uint8_t *rxStartBufferPointer )
{
    uint8_t status[2];

    Ebyte_E220x_ReadCommand( ebyte_status, RADIO_GET_RXBUFFERSTATUS, status, 2 );

    ebyte_status->recv_len = status[0];
    *rxStartBufferPointer = status[1];
}


void Ebyte_E220x_SetPayload( ebyte_status_t *ebyte_status, uint8_t *payload, uint8_t size )
{
    Ebyte_E220x_WriteBuffer( ebyte_status, 0x00, payload, size );
}


uint8_t Ebyte_E220x_GetPayload( ebyte_status_t *ebyte_status, uint8_t maxSize )
{
    uint8_t offset = 0;

    Ebyte_E220x_GetRxBufferStatus( ebyte_status, &offset );
    if( ebyte_status->recv_len > maxSize )
    {
        return 1;
    }
    Ebyte_E220x_ReadBuffer( ebyte_status, offset );
    return 0;
}

/* !
 * @brief set radio chip interrupts
 *
 * @param irqMask    RadioIrqMasks_t        
 *                   @arg IRQ_RADIO_NONE                                 
 *                   @arg IRQ_TX_DONE                             
 *                   @arg IRQ_RX_DONE                             
 *                   @arg IRQ_PREAMBLE_DETECTED                   
 *                   @arg IRQ_SYNCWORD_VALID                      
 *                   @arg IRQ_HEADER_VALID                        
 *                   @arg IRQ_HEADER_ERROR                        
 *                   @arg IRQ_CRC_ERROR                           
 *                   @arg IRQ_CAD_DONE                            
 *                   @arg IRQ_CAD_ACTIVITY_DETECTED               
 *                   @arg IRQ_RX_TX_TIMEOUT                       
 *                   @arg IRQ_RADIO_ALL                           
 *
 * @param dio1Mask   RadioIrqMasks_t
 * @param dio2Mask   RadioIrqMasks_t
 * @param dio3Mask   RadioIrqMasks_t
 */
void Ebyte_E220x_SetDioIrqParams( ebyte_status_t *ebyte_status, uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask )
{
    uint8_t buf[8];

    buf[0] = ( uint8_t )( ( irqMask >> 8 ) & 0x00FF );
    buf[1] = ( uint8_t )( irqMask & 0x00FF );
    buf[2] = ( uint8_t )( ( dio1Mask >> 8 ) & 0x00FF );
    buf[3] = ( uint8_t )( dio1Mask & 0x00FF );
    buf[4] = ( uint8_t )( ( dio2Mask >> 8 ) & 0x00FF );
    buf[5] = ( uint8_t )( dio2Mask & 0x00FF );
    buf[6] = ( uint8_t )( ( dio3Mask >> 8 ) & 0x00FF );
    buf[7] = ( uint8_t )( dio3Mask & 0x00FF );
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_CFG_DIOIRQ, buf, 8 );
}


/* !
 * @brief clear radio chip interrupts
 *
 * @param irq clr_irq_mask
 *            @arg IRQ_RADIO_NONE                          = 0x0000,
 *            @arg IRQ_TX_DONE                             = 0x0001,
 *            @arg IRQ_RX_DONE                             = 0x0002,
 *            @arg IRQ_PREAMBLE_DETECTED                   = 0x0004,
 *            @arg IRQ_SYNCWORD_VALID                      = 0x0008,
 *            @arg IRQ_HEADER_VALID                        = 0x0010,
 *            @arg IRQ_HEADER_ERROR                        = 0x0020,
 *            @arg IRQ_CRC_ERROR                           = 0x0040,
 *            @arg IRQ_CAD_DONE                            = 0x0080,
 *            @arg IRQ_CAD_ACTIVITY_DETECTED               = 0x0100,
 *            @arg IRQ_RX_TX_TIMEOUT                       = 0x0200,
 *            @arg IRQ_RADIO_ALL                           = 0xFFFF,
 */
void Ebyte_E220x_ClearIrqStatus( ebyte_status_t *ebyte_status, uint16_t irq )
{
    uint8_t buf[2];

    buf[0] = ( uint8_t )( ( ( uint16_t )irq >> 8 ) & 0x00FF );
    buf[1] = ( uint8_t )( ( uint16_t )irq & 0x00FF );
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_CLR_IRQSTATUS, buf, 2 );

    ebyte_status->irqStatus &= (~irq);
}


void Ebyte_E220x_SetTx( ebyte_status_t *ebyte_status, uint32_t timeout )
{
    uint8_t buf[3];

    ebyte_status->OperatingMode = MODE_TX;

    if ( ebyte_status->txen_io != -1 && ebyte_status->rxen_io != -1 ) {
        Ebyte_Port_IoControl( ebyte_status->rxen_io, 0 );
        Ebyte_Port_IoControl( ebyte_status->txen_io, 1 );
    }

    Ebyte_E220x_SetStandby(ebyte_status, STDBY_RC);
    // Ebyte_E220x_WriteRegister(spi, REG_XTA_TRIM, XTA_TRIM_VALUE); 
    // Ebyte_E220x_WriteRegister(spi, REG_XTB_TRIM, XTA_TRIM_VALUE); 

    Ebyte_E220x_SetDioIrqParams(ebyte_status, IRQ_TX_DONE | IRQ_RX_TX_TIMEOUT,
                              IRQ_TX_DONE | IRQ_RX_TX_TIMEOUT,
                              IRQ_RADIO_NONE ,
                              IRQ_RADIO_NONE);
    
    Ebyte_E220x_ClearIrqStatus ( ebyte_status, IRQ_RADIO_ALL );
    
    buf[0] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
    buf[1] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
    buf[2] = ( uint8_t )( timeout & 0xFF );
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_TX, buf, 3 );
}

void Ebyte_E220x_SetLoraPacketParams( ebyte_status_t *ebyte_status, uint8_t size )
{
    uint8_t buf[9] = { 0 };
    uint16_t preambleLen = LORA_PREAMBLE_LENGTH;

    buf[0] = ( preambleLen >> 8 ) & 0xFF;          
    buf[1] = preambleLen;                         
    buf[2] = LORA_PACKET_VARIABLE_LENGTH;          
    buf[3] = size;                                 
    buf[4] = LORA_CRC_ON;                          
    buf[5] = LORA_IQ_NORMAL;                       

    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_PACKETPARAMS, buf, 6 );
}

void Ebyte_E220x_SetCrcSeed( ebyte_status_t *ebyte_status, uint16_t seed )
{
    uint8_t buf[2];

    buf[0] = ( uint8_t )( ( seed >> 8 ) & 0xFF );
    buf[1] = ( uint8_t )( seed & 0xFF );

    switch( ebyte_status->PacketType )
    {
    case PACKET_TYPE_GFSK:
        Ebyte_E220x_WriteRegisters( ebyte_status, REG_LR_CRCSEEDBASEADDR, buf, 2 );
        break;

    default:
        break;
    }
}

void Ebyte_E220x_SetCrcPolynomial( ebyte_status_t *ebyte_status, uint16_t polynomial )
{
    uint8_t buf[2];

    buf[0] = ( uint8_t )( ( polynomial >> 8 ) & 0xFF );
    buf[1] = ( uint8_t )( polynomial & 0xFF );

    switch( ebyte_status->PacketType )
    {
    case PACKET_TYPE_GFSK:
        Ebyte_E220x_WriteRegisters( ebyte_status, REG_LR_CRCPOLYBASEADDR, buf, 2 );
        break;

    default:
        break;
    }
}

void Ebyte_E220x_SetFskPacketParams( ebyte_status_t *ebyte_status, uint8_t size )
{
    uint8_t buf[9] = { 0 };  
    uint8_t crcVal = 0;
    uint16_t preambleLength = 0;

    Ebyte_E220x_SetCrcSeed( ebyte_status, CRC_CCITT_SEED );
    Ebyte_E220x_SetCrcPolynomial( ebyte_status, CRC_POLYNOMIAL_CCITT );
    crcVal = RADIO_CRC_2_BYTES_INV;
    preambleLength = FSK_PREAMBLE_LENGTH<<3;      

    buf[0] = ( preambleLength >> 8 ) & 0xFF;
    buf[1] = preambleLength;
    buf[2] = RADIO_PREAMBLE_DETECTOR_08_BITS;   
    buf[3] = FSK_MAC_PUBLIC_SYNCWORD_LENGTH<<3; 
    buf[4] = RADIO_ADDRESSCOMP_FILT_OFF;        
    buf[5] = RADIO_PACKET_VARIABLE_LENGTH;      
    buf[6] = size;                              
    buf[7] = crcVal;                            
    buf[8] = RADIO_DC_FREEWHITENING;

    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_PACKETPARAMS, buf, 9 );
}

void Ebyte_E220x_SetRx( ebyte_status_t *ebyte_status, uint32_t timeout )
{
    uint8_t buf[3];

    if ( ebyte_status->txen_io != -1 && ebyte_status->rxen_io != -1 ) {
        Ebyte_Port_IoControl( ebyte_status->rxen_io, 1 );
        Ebyte_Port_IoControl( ebyte_status->txen_io, 0 );
    }

    Ebyte_E220x_SetStandby(ebyte_status, STDBY_RC);

    // Ebyte_E220x_WriteRegister(ebyte_status, REG_XTA_TRIM, XTA_TRIM_VALUE); 
    // Ebyte_E220x_WriteRegister(ebyte_status, REG_XTB_TRIM, XTA_TRIM_VALUE); 

    Ebyte_E220x_SetDioIrqParams(ebyte_status, IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT,
                          IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT,
                          IRQ_RADIO_NONE ,
                          IRQ_RADIO_NONE);    
    
    if( ebyte_status->rdo_mode == RADIO_MODE_LORA )
    {
        Ebyte_E220x_SetLoraPacketParams(ebyte_status, 0xFF);
    }
    else{
        Ebyte_E220x_SetFskPacketParams(ebyte_status, 0xFF);
    }
    
    Ebyte_E220x_ClearIrqStatus ( ebyte_status, IRQ_RADIO_ALL );

    buf[0] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
    buf[1] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
    buf[2] = ( uint8_t )( timeout & 0xFF );
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_RX, buf, 3 );
}

/* !
 * @brief get interrupts from radio chip
 *
 * @return  irq status
 * @note    irq status bit mask
 *          bit0 : IRQ_TX_DONE                
 *          bit1 : IRQ_RX_DONE                
 *          bit2 : IRQ_PREAMBLE_DETECTED      
 *          bit3 : IRQ_SYNCWORD_VALID         
 *          bit4 : IRQ_HEADER_VALID           
 *          bit5 : IRQ_HEADER_ERROR           
 *          bit6 : IRQ_CRC_ERROR             
 *          bit7 : IRQ_CAD_DONE               
 *          bit8 : IRQ_CAD_ACTIVITY_DETECTED 
 *          bit9 : IRQ_RX_TX_TIMEOUT         
 */
void Ebyte_E220x_GetIrqStatus( ebyte_status_t *ebyte_status )
{
    uint8_t irqStatus[2];

    Ebyte_E220x_ReadCommand( ebyte_status, RADIO_GET_IRQSTATUS, irqStatus, 2 );

    ebyte_status->irqStatus = ( irqStatus[0] << 8 ) | irqStatus[1];
}

RadioStatus_t Ebyte_E220x_GetStatus( ebyte_status_t *ebyte_status )
{
    uint8_t stat = 0;
    RadioStatus_t status;

    Ebyte_E220x_ReadCommand( ebyte_status, RADIO_GET_STATUS, ( uint8_t * )&stat, 1 );
    status.Value = stat;
    return status;
}

RadioError_t Ebyte_E220x_GetDeviceErrors( ebyte_status_t *ebyte_status )
{
    RadioError_t error;

    Ebyte_E220x_ReadCommand( ebyte_status, RADIO_GET_ERROR, ( uint8_t * )&error, 2 );
    return error;
}

void Ebyte_E220x_SetLoraSyncWord( ebyte_status_t *ebyte_status, uint16_t  syncWord )
{
    Ebyte_E220x_WriteRegister( ebyte_status, REG_LR_SYNCWORDBASEADDRESS, ( syncWord >> 8 ) & 0xFF  );
    Ebyte_E220x_WriteRegister( ebyte_status, REG_LR_SYNCWORDBASEADDRESS + 1,  syncWord  & 0xFF );
}


void Ebyte_E220x_SetFskSyncWord( ebyte_status_t *ebyte_status )
{
    uint8_t syncWord[8] = {0};
    
    syncWord[0]=FSK_MAC_PUBLIC_SYNCWORD_7;
    syncWord[1]=FSK_MAC_PUBLIC_SYNCWORD_6;
    syncWord[2]=FSK_MAC_PUBLIC_SYNCWORD_5;
    syncWord[3]=FSK_MAC_PUBLIC_SYNCWORD_4;
    syncWord[4]=FSK_MAC_PUBLIC_SYNCWORD_3;
    syncWord[5]=FSK_MAC_PUBLIC_SYNCWORD_2;
    syncWord[6]=FSK_MAC_PUBLIC_SYNCWORD_1;
    syncWord[7]=FSK_MAC_PUBLIC_SYNCWORD_0;
    
    Ebyte_E220x_WriteRegisters( ebyte_status, REG_LR_SYNCWORDBASEADDRESS, syncWord, 8 );
}


void Ebyte_E220x_SetWhiteningSeed( ebyte_status_t *ebyte_status, uint16_t seed )
{
    uint8_t regValue = 0;

    switch( ebyte_status->PacketType )
    {
    case PACKET_TYPE_GFSK:
        regValue = Ebyte_E220x_ReadRegister( ebyte_status, REG_LR_WHITSEEDBASEADDR_MSB ) & 0xFE;
        regValue = ( ( seed >> 8 ) & 0x01 ) | regValue;
        Ebyte_E220x_WriteRegister( ebyte_status, REG_LR_WHITSEEDBASEADDR_MSB, regValue ); // only 1 bit.
        Ebyte_E220x_WriteRegister( ebyte_status, REG_LR_WHITSEEDBASEADDR_LSB, ( uint8_t )seed );
        break;

    default:
        break;
    }
}


uint32_t Ebyte_E220x_GetRandom( ebyte_status_t *ebyte_status )
{
    uint8_t buf[] = { 0, 0, 0, 0 };

    // Set radio in continuous reception
    Ebyte_E220x_SetRx( ebyte_status, 0 );

    Ebyte_Port_DelayMs( 1 );

    Ebyte_E220x_ReadRegisters( ebyte_status, RANDOM_NUMBER_GENERATORBASEADDR, buf, 4 );

    Ebyte_E220x_SetStandby( ebyte_status, STDBY_RC );

    return ( ( uint32_t )buf[0] << 24 ) | ( ( uint32_t )buf[1] << 16 ) | ( ( uint32_t )buf[2] << 8 ) | buf[3];;
}

/* !
 * @brief set radio chip into sleep mode
 *
 * @param cmd mask
 *        @arg 0x00: cold start and RTC timeout disable
 *        @arg 0x01: cold start and wake-up on RTC timeout(RC64k)
 *        @arg 0x04: warm start and RTC timeout disable
 *        @arg 0x05: warm start and wake-up on RTC timeout(RC64k)
 */
void Ebyte_E220x_SetSleep( ebyte_status_t *ebyte_status, uint8_t cmd )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_SLEEP, &cmd, 1 );
    ebyte_status->OperatingMode = MODE_SLEEP;
}


void Ebyte_E220x_SetFs( ebyte_status_t *ebyte_status )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_FS, 0, 0 );
    ebyte_status->OperatingMode = MODE_FS;
}


void Ebyte_E220x_SetRxBoosted( ebyte_status_t *ebyte_status, uint32_t timeout )
{
    uint8_t buf[3];

    ebyte_status->OperatingMode = MODE_RX;

    Ebyte_E220x_WriteRegister( ebyte_status, REG_RX_GAIN, 0x96 ); // max LNA gain, increase current by ~2mA for around ~3dB in sensivity

    buf[0] = ( uint8_t )( ( timeout >> 16 ) & 0xFF );
    buf[1] = ( uint8_t )( ( timeout >> 8 ) & 0xFF );
    buf[2] = ( uint8_t )( timeout & 0xFF );
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_RX, buf, 3 );
}


void Ebyte_E220x_SetRxDutyCycle( ebyte_status_t *ebyte_status, uint32_t rxTime, uint32_t sleepTime )
{
    uint8_t buf[6];

    buf[0] = ( uint8_t )( ( rxTime >> 16 ) & 0xFF );
    buf[1] = ( uint8_t )( ( rxTime >> 8 ) & 0xFF );
    buf[2] = ( uint8_t )( rxTime & 0xFF );
    buf[3] = ( uint8_t )( ( sleepTime >> 16 ) & 0xFF );
    buf[4] = ( uint8_t )( ( sleepTime >> 8 ) & 0xFF );
    buf[5] = ( uint8_t )( sleepTime & 0xFF );
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_RXDUTYCYCLE, buf, 6 );
    ebyte_status->OperatingMode = MODE_RX_DC;
}


void Ebyte_E220x_SetCad( ebyte_status_t *ebyte_status )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_CAD, 0, 0 );
    ebyte_status->OperatingMode = MODE_CAD;
}


void Ebyte_E220x_SetTxContinuousWave( ebyte_status_t *ebyte_status )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_TXCONTINUOUSWAVE, 0, 0 );
}


void Ebyte_E220x_SetTxInfinitePreamble( ebyte_status_t *ebyte_status )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_TXCONTINUOUSPREAMBLE, 0, 0 );
}

void Ebyte_E220x_SetStopRxTimerOnPreambleDetect( ebyte_status_t *ebyte_status, uint8_t enable )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_STOPRXTIMERONPREAMBLE, ( uint8_t* )&enable, 1 );
}

void Ebyte_E220x_SetLoRaSymbNumTimeout( ebyte_status_t *ebyte_status, uint8_t SymbNum )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_LORASYMBTIMEOUT, &SymbNum, 1 );
}

/* !
 * @brief set power mode
 *
 * @param mode
 *        @arg USE_LDO   :use LDO power
 *        @arg USE_DCDC  :use DCDC power
 */
void Ebyte_E220x_SetRegulatorMode( ebyte_status_t *ebyte_status, RadioRegulatorMode_t mode )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_REGULATORMODE, ( uint8_t* )&mode, 1 );
}


void Ebyte_E220x_CalibrateImage( ebyte_status_t *ebyte_status, uint32_t freq )
{
    uint8_t calFreq[2];

    if( freq > 900000000 )
    {
        calFreq[0] = 0xE1;
        calFreq[1] = 0xE9;
    }
    else if( freq > 850000000 )
    {
        calFreq[0] = 0xD7;
        calFreq[1] = 0xD8;
    }
    else if( freq > 770000000 )
    {
        calFreq[0] = 0xC1;
        calFreq[1] = 0xC5;
    }
    else if( freq > 460000000 )
    {
        calFreq[0] = 0x75;
        calFreq[1] = 0x81;
    }
    else if( freq > 425000000 )
    {
        calFreq[0] = 0x6B;
        calFreq[1] = 0x6F;
    }
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_CALIBRATEIMAGE, calFreq, 2 );
}


void Ebyte_E220x_SetPaConfig( ebyte_status_t *ebyte_status, uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut )
{
    uint8_t buf[4];

    buf[0] = paDutyCycle;
    buf[1] = hpMax;
    buf[2] = deviceSel;
    buf[3] = paLut;
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_PACONFIG, buf, 4 );
}


void Ebyte_E220x_SetRxTxFallbackMode( ebyte_status_t *ebyte_status, uint8_t fallbackMode )
{
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_TXFALLBACKMODE, &fallbackMode, 1 );
}


void Ebyte_E220x_SetRfFrequency( ebyte_status_t *ebyte_status, uint32_t frequency )
{
    uint8_t buf[4];
    uint32_t freq = 0;

    CalibrationParams_t calibrationParams;
    calibrationParams.Value = 0xFF;
    Ebyte_E220x_Calibrate(ebyte_status, calibrationParams);
    
    Ebyte_E220x_CalibrateImage( ebyte_status, frequency );

    Ebyte_E220x_SetStandby(ebyte_status, STDBY_RC);
    // Ebyte_E220x_WriteRegister(spi, REG_XTA_TRIM, XTA_TRIM_VALUE); 
    // Ebyte_E220x_WriteRegister(spi, REG_XTB_TRIM, XTA_TRIM_VALUE); 
    
    freq = ( uint32_t )( ( double )frequency / ( double )FREQ_STEP );
    buf[0] = ( uint8_t )( ( freq >> 24 ) & 0xFF );
    buf[1] = ( uint8_t )( ( freq >> 16 ) & 0xFF );
    buf[2] = ( uint8_t )( ( freq >> 8 ) & 0xFF );
    buf[3] = ( uint8_t )( freq & 0xFF );
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_RFFREQUENCY, buf, 4 );
}

/* !
 * @brief set communication packet type
 *
 * @param packetType
 *        @arg PACKET_TYPE_GFSK: FSK
 *        @arg PACKET_TYPE_LORA: LORA
 */
void Ebyte_E220x_SetPacketType( ebyte_status_t *ebyte_status, RadioPacketTypes_t packetType )
{
    ebyte_status->PacketType = packetType;
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_PACKETTYPE, ( uint8_t* )&packetType, 1 );
}

void Ebyte_E220x_SetModem( ebyte_status_t *ebyte_status, RadioPacketTypes_t modem )
{
    switch( modem )
    {
    default:
    case PACKET_TYPE_GFSK:
        Ebyte_E220x_SetPacketType( ebyte_status, PACKET_TYPE_GFSK );
        break;
    case PACKET_TYPE_LORA:
        Ebyte_E220x_SetPacketType( ebyte_status, PACKET_TYPE_LORA );
        break;
    }
}

void Ebyte_E220x_SetTxPaParams( ebyte_status_t *ebyte_status, int8_t power, RadioRampTimes_t rampTime )
{
    uint8_t buf[2];

    Ebyte_E220x_SetPaConfig( ebyte_status, 0x04, 0x07, 0x00, 0x01 );
    if( power > 22 )
    {
        power = 22;
    }
    else if( power < -3 )
    {
        power = -3;
    }
    Ebyte_E220x_WriteRegister( ebyte_status, REG_OCP, 0x38 ); // current max 160mA for the whole device

    buf[0] = power;
    buf[1] = ( uint8_t )rampTime;
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_TXPARAMS, buf, 2 );
}


/* !
 * @brief LoRa communication params
 *
 * @param datarate  
 *                  @arg LORA_SF5
 *                  @arg LORA_SF6
 *                  @arg LORA_SF7
 *                  @arg LORA_SF8
 *                  @arg LORA_SF9
 *                  @arg LORA_SF10
 *                  @arg LORA_SF11
 *                  @arg LORA_SF12
 *
 * @param bandwidth 
 *                  @arg LORA_BW_500
 *                  @arg LORA_BW_250
 *                  @arg LORA_BW_125
 *                  @arg LORA_BW_062
 *                  @arg LORA_BW_041
 *                  @arg LORA_BW_031
 *                  @arg LORA_BW_020
 *                  @arg LORA_BW_015
 *                  @arg LORA_BW_010
 *                  @arg LORA_BW_007
 *
 * @param coderate  
 *                  @arg LORA_CR_4_5
 *                  @arg LORA_CR_4_6
 *                  @arg LORA_CR_4_7
 *                  @arg LORA_CR_4_8
 *
 * @note  depends on Ebyte_E220x_SetPacketType() as LORA
 *        LoRa Rs=BW/(2^SF)
 *        LoRa DR=SF*( BW/2^SF)*CR
 *        Communication frequency band：169MHz   bandwidth：250K-500KHz 
 */
void Ebyte_E220x_SetModulationLoraParams( ebyte_status_t *ebyte_status, RadioLoRaSpreadingFactors_t  datarate, RadioLoRaBandwidths_t bandwidth, RadioLoRaCodingRates_t coderate )
{
    uint8_t buf[8] = { 0 };

    buf[0] = datarate ;
    buf[1] = bandwidth;
    buf[2] = coderate ;
    buf[3] = 0x01;

    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_MODULATIONPARAMS, buf, 4 );
}

/* !
 * @brief FSK params
 * 
 * @param fdev FrequencyDeviation
 * @param bandwidth 
 * @param shaping
 *      @arg MOD_SHAPING_OFF        0x00
 *      @arg MOD_SHAPING_G_BT_03    0x08
 *      @arg MOD_SHAPING_G_BT_05    0x09
 *      @arg MOD_SHAPING_G_BT_07    0x0A
 *      @arg MOD_SHAPING_G_BT_1     0x0B
 * @param bitrate 
 *
 * @note fdev > datarate
 */
void Ebyte_E220x_SetModulationFskParams( ebyte_status_t *ebyte_status, uint32_t fdev, uint8_t bandwidth, RadioModShapings_t shaping, uint32_t bitrate )
{
    uint32_t tempVal = 0;
    uint8_t buf[8] = { 0 };
    
    tempVal = ( uint32_t )( 32 * ( ( double )XTAL_FREQ / ( double )bitrate ) );
    buf[0] = ( tempVal >> 16 ) & 0xFF;
    buf[1] = ( tempVal >> 8 ) & 0xFF;
    buf[2] = tempVal & 0xFF;
    buf[3] = shaping;
    buf[4] = bandwidth;
    tempVal = ( uint32_t )( ( double )fdev / ( double )FREQ_STEP );
    buf[5] = ( tempVal >> 16 ) & 0xFF;
    buf[6] = ( tempVal >> 8 ) & 0xFF;
    buf[7] = ( tempVal& 0xFF );
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_MODULATIONPARAMS, buf, 8 );
}

void Ebyte_E220x_SetCadParams( ebyte_status_t *ebyte_status, RadioLoRaCadSymbols_t cadSymbolNum, uint8_t cadDetPeak, uint8_t cadDetMin, RadioCadExitModes_t cadExitMode, uint32_t cadTimeout )
{
    uint8_t buf[7];

    buf[0] = ( uint8_t )cadSymbolNum;
    buf[1] = cadDetPeak;
    buf[2] = cadDetMin;
    buf[3] = ( uint8_t )cadExitMode;
    buf[4] = ( uint8_t )( ( cadTimeout >> 16 ) & 0xFF );
    buf[5] = ( uint8_t )( ( cadTimeout >> 8 ) & 0xFF );
    buf[6] = ( uint8_t )( cadTimeout & 0xFF );
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_CADPARAMS, buf, 7 );
    ebyte_status->OperatingMode = MODE_CAD;
}

void Ebyte_E220x_SetBufferBaseAddress( ebyte_status_t *ebyte_status, uint8_t txBaseAddress, uint8_t rxBaseAddress )
{
    uint8_t buf[2];

    buf[0] = txBaseAddress;
    buf[1] = rxBaseAddress;
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_SET_BUFFERBASEADDRESS, buf, 2 );
}

/*!
 * Returns the known FSK bandwidth registers value
 *
 * \param [IN] bandwidth Bandwidth value in Hz
 * \retval regValue Bandwidth register value.
 */
uint8_t Ebyte_E220x_GetFskBandwidthRegValue( uint32_t bandwidth )
{
    uint8_t i;

    if( bandwidth == 0 )
    {
        return( 0x1F );
    }

    for( i = 0; i < ( sizeof( FskBandwidths ) / sizeof( FskBandwidth_t ) ) - 1; i++ )
    {
        if( ( bandwidth >= FskBandwidths[i].bandwidth ) && ( bandwidth < FskBandwidths[i + 1].bandwidth ) )
        {
            return FskBandwidths[i+1].RegValue;
        }
    }
    // ERROR: Value not found
    while( 1 );
}

int8_t Ebyte_E220x_GetRssiInst( ebyte_status_t *ebyte_status )
{
    uint8_t buf[1];
    int8_t rssi = 0;

    Ebyte_E220x_ReadCommand( ebyte_status, RADIO_GET_RSSIINST, buf, 1 );
    rssi = -buf[0] >> 1;
    return rssi;
}

void Ebyte_E220x_GetFskPacketStatus( ebyte_status_t *ebyte_status )
{
    uint8_t status[3];

    Ebyte_E220x_ReadCommand( ebyte_status, RADIO_GET_PACKETSTATUS, status, 3 );  
  
    ebyte_status->PacketStatus.Params.Gfsk.RxStatus = status[0];
    ebyte_status->PacketStatus.Params.Gfsk.RssiSync = -status[1] >> 1;
    ebyte_status->PacketStatus.Params.Gfsk.RssiAvg = -status[2] >> 1;
    ebyte_status->PacketStatus.Params.Gfsk.FreqError = 0;
}

void Ebyte_E220x_GetLoraPacketStatus( ebyte_status_t *ebyte_status )
{
    uint8_t status[3];

    Ebyte_E220x_ReadCommand( ebyte_status, RADIO_GET_PACKETSTATUS, status, 3 );

    ebyte_status->PacketStatus.Params.LoRa.RssiPkt = -status[0] >> 1;
    ( status[1] < 128 ) ? ( ebyte_status->PacketStatus.Params.LoRa.SnrPkt = status[1] >> 2 ) : ( ebyte_status->PacketStatus.Params.LoRa.SnrPkt = ( ( status[1] - 256 ) >> 2 ) );
    ebyte_status->PacketStatus.Params.LoRa.SignalRssiPkt = -status[2] >> 1;
    ebyte_status->PacketStatus.Params.LoRa.FreqError = ebyte_status->FrequencyError;

}

void Ebyte_E220x_ClearDeviceErrors( ebyte_status_t *ebyte_status )
{
    uint8_t buf[2] = { 0x00, 0x00 };
    Ebyte_E220x_WriteCommand( ebyte_status, RADIO_CLR_ERROR, buf, 2 );
}

/*!
 * radio chip send data
 *
 * @param payload the data buffer waiting to send
 * @param size    send buffer size
 * @param timeout operate waiting time (if set to 0, device will stay in rx mode until reception occurs)
 */
uint16_t Ebyte_E220x_SendPayload( ebyte_status_t *ebyte_status, uint8_t *payload, uint8_t size, uint32_t timeout )
{
    uint16_t irq_return;

    Ebyte_E220x_SetStandby(ebyte_status, STDBY_RC);

    if( ebyte_status->rdo_mode == RADIO_MODE_LORA )
    {
        Ebyte_E220x_SetLoraPacketParams( ebyte_status, size );
    }
    else
    {
        Ebyte_E220x_SetFskPacketParams( ebyte_status, size );
    }
    
    Ebyte_E220x_SetPayload( ebyte_status, payload, size );

    Ebyte_E220x_SetTx( ebyte_status, timeout );

    do
    {
        Ebyte_Port_DelayMs( 1 );
        Ebyte_E220x_GetIrqStatus(ebyte_status);
    }
    while( !ebyte_status->irqStatus );
    
    irq_return = ebyte_status->irqStatus;

    Ebyte_Port_TransmitCallback( ebyte_status->irqStatus );
    
    Ebyte_E220x_ClearIrqStatus ( ebyte_status, IRQ_RADIO_ALL );

    return irq_return;
}

/*!
 * init ebyte status struct
 *
 * @param ebyte_config       ebyte config struct
 * @param ebyte_status       the status struct will be inited
 * @param tx_irq_handler     tx handler which will be triggered when tx irq occurs (set to NULL if not use)
 * @param tx_irq_handler     rx handler which will be triggered when rx irq occurs (set to NULL if not use)
 */
void Ebyte_E220x_Install( ebyte_config_t ebyte_config, ebyte_status_t *ebyte_status )
{
    ebyte_status->spi_id = ebyte_config.spi_id;

    ebyte_status->xmutex = xSemaphoreCreateMutex();

    ebyte_status->OperatingMode = MODE_STDBY_RC;

    ebyte_status->PacketType = PACKET_TYPE_NONE;

    ebyte_status->rdo_mode = ebyte_config.radio_mode;

    ebyte_status->FrequencyError = 0;

    ebyte_status->irqStatus = 0x0;

    ebyte_status->cs_io = ebyte_config.cs_io;

    ebyte_status->busy_io = ebyte_config.busy_io;

    ebyte_status->rst_io = ebyte_config.rst_io;

    ebyte_status->rxen_io = ebyte_config.rxen_io;

    ebyte_status->txen_io = ebyte_config.txen_io;

    Ebyte_esp_gpio_init( ebyte_config );

    Ebyte_esp_spi_init( &ebyte_status->spi, ebyte_config.spi_id, ebyte_config.miso_io, ebyte_config.mosi_io, ebyte_config.sclk_io, ebyte_config.cs_io, ebyte_config.spi_speed_hz );

    ebyte_status->RecvPacketBuffer = (uint8_t *)malloc(RF_RX_BUFFER_SIZE * sizeof(uint8_t));
    assert( ebyte_status->RecvPacketBuffer != NULL );
}

/*!
 * deinit ebyte status struct, remove spi device, free spi bus and free malloc receive buffer
 *
 * @param ebyte_status       the status struct will be deinited
 */
void Ebyte_E220x_Unstall( ebyte_status_t *ebyte_status )
{
    esp_err_t ret;

    ret = spi_bus_remove_device(ebyte_status->spi);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_free(ebyte_status->spi_id);
    ESP_ERROR_CHECK(ret);

    free(ebyte_status->RecvPacketBuffer);
}

/*!
 * init radio chip by commands with status struct
 *
 * @param ebyte_status     the ebyte status struct
 */
void Ebyte_E220x_Init( ebyte_status_t *ebyte_status )
{
    bool enable = false;
    uint8_t SymbNum = 0;

    Ebyte_E220x_Reset( ebyte_status );
    Ebyte_E220x_Wakeup( ebyte_status );

    Ebyte_E220x_SetStandby( ebyte_status, STDBY_RC );
    // Ebyte_E220x_WriteRegister(ebyte_status, REG_XTA_TRIM, XTA_TRIM_VALUE); 
    // Ebyte_E220x_WriteRegister(ebyte_status, REG_XTB_TRIM, XTA_TRIM_VALUE); 

    if ( ebyte_status->rxen_io == -1 && ebyte_status->txen_io == -1) {
        Ebyte_E220x_SetDio2AsRfSwitchCtrl( ebyte_status, 1 );
    }

    ebyte_status->OperatingMode = MODE_STDBY_RC;

    Ebyte_E220x_SetStandby(ebyte_status, STDBY_RC);

    Ebyte_E220x_SetRegulatorMode( ebyte_status, USE_DCDC );

    Ebyte_E220x_SetBufferBaseAddress( ebyte_status, 0x00, 0x00 );

    Ebyte_E220x_SetTxPaParams( ebyte_status, 22, RADIO_RAMP_200_US );

    Ebyte_E220x_SetDioIrqParams(ebyte_status, IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT,
                        IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT,
                        IRQ_RADIO_NONE ,
                        IRQ_RADIO_NONE); 

    uint32_t fs = ( RF_FREQUENCY_START + ( RF_FREQUENCY_CH * RF_FREQUENCY_CH_STEP ) );
    Ebyte_E220x_SetRfFrequency( ebyte_status, fs );
    
    if( ebyte_status->rdo_mode == RADIO_MODE_LORA )
    {
        /***************************TX config************************************/
        Ebyte_E220x_SetModem( ebyte_status, PACKET_TYPE_LORA );

        /* LoRa 2.4KHz(SF11)  BW(500KHz) CR(4/5) */
        RadioLoRaBandwidths_t bw = (RadioLoRaBandwidths_t)(Ebyte_E220x_LoraBpsTable[RF_LORA_AIR_BPS][0]);
        RadioLoRaSpreadingFactors_t sf = (RadioLoRaSpreadingFactors_t)(Ebyte_E220x_LoraBpsTable[RF_LORA_AIR_BPS][1]);
        RadioLoRaCodingRates_t cr = ((RadioLoRaCodingRates_t)Ebyte_E220x_LoraBpsTable[RF_LORA_AIR_BPS][2] );
        Ebyte_E220x_SetModulationLoraParams ( ebyte_status, sf,  bw, cr);

        Ebyte_E220x_SetLoraPacketParams( ebyte_status, 0xFF );
        /***************************RX config************************************/
        Ebyte_E220x_SetStopRxTimerOnPreambleDetect( ebyte_status, enable );
        Ebyte_E220x_SetLoRaSymbNumTimeout( ebyte_status, SymbNum );
    }          
    else {
        Ebyte_E220x_SetModem( ebyte_status, PACKET_TYPE_GFSK );
    
        uint32_t bw = Ebyte_E220x_GetFskBandwidthRegValue( RF_FSK_BANDWIDTH );
        Ebyte_E220x_SetModulationFskParams( ebyte_status, RF_FSK_FREQUENCY_DEVIATION , bw , MOD_SHAPING_G_BT_1, RF_FSK_DATA_RATE);
        
        Ebyte_E220x_SetFskPacketParams( ebyte_status, 0xFF );
        
        Ebyte_E220x_SetFskSyncWord( ebyte_status );
        
        Ebyte_E220x_SetWhiteningSeed( ebyte_status, 0x01FF );
    }

    Ebyte_E220x_SetRx( ebyte_status, 0 );
}

/*!
 * Query the interrupt status of the radio chip, if rx irq occured, trigger rx handler
 *
 * @param ebyte_status     the ebyte status struct
 */
uint16_t Ebyte_E220x_IntOrPollTask( ebyte_status_t *ebyte_status, uint8_t *receive_data )
{
    Ebyte_E220x_GetIrqStatus(ebyte_status);

    uint16_t irq_return = ebyte_status->irqStatus;

    if( ebyte_status->irqStatus != 0 )
    {
        if( ( ebyte_status->irqStatus & IRQ_RX_DONE ) ==  IRQ_RX_DONE )
        {
            if( ebyte_status->rdo_mode == RADIO_MODE_LORA )
            {
                Ebyte_E220x_GetLoraPacketStatus( ebyte_status );
            }          
            else {
                Ebyte_E220x_GetFskPacketStatus( ebyte_status );
            }        
            Ebyte_E220x_GetPayload( ebyte_status, RF_RX_BUFFER_SIZE );
            
            Ebyte_Port_ReceiveCallback( ebyte_status->irqStatus, ebyte_status->RecvPacketBuffer, receive_data, ebyte_status->recv_len );
            
            Ebyte_E220x_SetRx( ebyte_status, 0 );
        }
        else 
        {
            Ebyte_Port_ReceiveCallback( ebyte_status->irqStatus, ebyte_status->RecvPacketBuffer, receive_data, ebyte_status->recv_len );
        }

        Ebyte_E220x_ClearIrqStatus ( ebyte_status, IRQ_RADIO_ALL );
    }

    return irq_return;
}