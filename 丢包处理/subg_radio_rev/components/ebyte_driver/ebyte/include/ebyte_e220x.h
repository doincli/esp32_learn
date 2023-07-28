#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "ebyte_conf.h"
#include "ebyte_callback.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp32c6/rom/ets_sys.h"

/* EBYTE_E220_400M22S */
#define RF_FREQUENCY_START                          410000000
#define RF_FREQUENCY_CH_STEP                        1000000
#define RF_FREQUENCY_CH                             23        // 410 + 23 * 1 = 433MHz

#define RF_TX_OUTPUT_POWER                          22        // dBm

#define RF_RX_BUFFER_SIZE                           255 
   
#define LORA_MAC_PUBLIC_SYNCWORD                    0x3444

#define LORA_MAC_PRIVATE_SYNCWORD                   0x1424

#define RF_LORA_AIR_BPS                             2         // 0=0.3k  1=1.2k  2=2.4k  3=4.8k  4=9.6k  5=19.5k  6=38.4k  7=62.5k       
  
#define FSK_PREAMBLE_LENGTH                         20   
  
#define FSK_MAC_PUBLIC_SYNCWORD_LENGTH              2 
#define FSK_MAC_PUBLIC_SYNCWORD_7                   0x57
#define FSK_MAC_PUBLIC_SYNCWORD_6                   0x4B
#define FSK_MAC_PUBLIC_SYNCWORD_5                   0x00
#define FSK_MAC_PUBLIC_SYNCWORD_4                   0x00                  
#define FSK_MAC_PUBLIC_SYNCWORD_3                   0x00 
#define FSK_MAC_PUBLIC_SYNCWORD_2                   0x00 
#define FSK_MAC_PUBLIC_SYNCWORD_1                   0x00    
#define FSK_MAC_PUBLIC_SYNCWORD_0                   0x00    
      
/**
 * 
 *   DataRate          FrenquencyDeviation        BW
 *    4.8kbps                16KHz                31KHz
 *    9.6kbps                20KHz                31KHz
 *     20kbps                30KHz                50KHz
 *     40kbps                40KHz              62.5KHz
 *    100kbps               100KHz               166KHz
 */                                                               

#define RF_FSK_DATA_RATE                            4800     // 4.8kbps  

#define RF_FSK_FREQUENCY_DEVIATION                  16000    // 16k    

#define RF_FSK_BANDWIDTH                            31000    // 31k   

typedef union RadioStatus_u
{
    unsigned char Value;
    struct
    {   //bit order is lsb -> msb
        unsigned char Reserved  : 1;  //!< Reserved
        unsigned char CmdStatus : 3;  //!< Command status
        unsigned char ChipMode  : 3;  //!< Chip mode
        unsigned char CpuBusy   : 1;  //!< Flag for CPU radio busy
    }Fields;
}RadioStatus_t;

typedef enum
{
    PACKET_TYPE_GFSK                        = 0x00,
    PACKET_TYPE_LORA                        = 0x01,
    PACKET_TYPE_NONE                        = 0x0F,
}RadioPacketTypes_t;

typedef enum
{
    IRQ_HEADER_ERROR_CODE                   = 0x01,
    IRQ_SYNCWORD_ERROR_CODE                 = 0x02,
    IRQ_CRC_ERROR_CODE                      = 0x04,
}IrqErrorCode_t;

enum IrqPblSyncHeaderCode_t
{
    IRQ_PBL_DETECT_CODE                     = 0x01,
    IRQ_SYNCWORD_VALID_CODE                 = 0x02,
    IRQ_HEADER_VALID_CODE                   = 0x04,
};

typedef enum
{
    STDBY_RC                                = 0x00,
    STDBY_XOSC                              = 0x01,
}RadioStandbyModes_t;

typedef enum
{
    RADIO_RAMP_10_US                        = 0x00,
    RADIO_RAMP_20_US                        = 0x01,
    RADIO_RAMP_40_US                        = 0x02,
    RADIO_RAMP_80_US                        = 0x03,
    RADIO_RAMP_200_US                       = 0x04,
    RADIO_RAMP_800_US                       = 0x05,
    RADIO_RAMP_1700_US                      = 0x06,
    RADIO_RAMP_3400_US                      = 0x07,
}RadioRampTimes_t;

typedef enum
{
    LORA_CAD_01_SYMBOL                      = 0x00,
    LORA_CAD_02_SYMBOL                      = 0x01,
    LORA_CAD_04_SYMBOL                      = 0x02,
    LORA_CAD_08_SYMBOL                      = 0x03,
    LORA_CAD_16_SYMBOL                      = 0x04,
}RadioLoRaCadSymbols_t;

typedef enum
{
    LORA_CAD_ONLY                           = 0x00,
    LORA_CAD_RX                             = 0x01,
    LORA_CAD_LBT                            = 0x10,
}RadioCadExitModes_t;

typedef enum
{
    MOD_SHAPING_OFF                         = 0x00,
    MOD_SHAPING_G_BT_03                     = 0x08,
    MOD_SHAPING_G_BT_05                     = 0x09,
    MOD_SHAPING_G_BT_07                     = 0x0A,
    MOD_SHAPING_G_BT_1                      = 0x0B,
}RadioModShapings_t;

typedef enum
{
    RX_BW_4800                              = 0x1F,
    RX_BW_5800                              = 0x17,
    RX_BW_7300                              = 0x0F,
    RX_BW_9700                              = 0x1E,
    RX_BW_11700                             = 0x16,
    RX_BW_14600                             = 0x0E,
    RX_BW_19500                             = 0x1D,
    RX_BW_23400                             = 0x15,
    RX_BW_29300                             = 0x0D,
    RX_BW_39000                             = 0x1C,
    RX_BW_46900                             = 0x14,
    RX_BW_58600                             = 0x0C,
    RX_BW_78200                             = 0x1B,
    RX_BW_93800                             = 0x13,
    RX_BW_117300                            = 0x0B,
    RX_BW_156200                            = 0x1A,
    RX_BW_187200                            = 0x12,
    RX_BW_234300                            = 0x0A,
    RX_BW_312000                            = 0x19,
    RX_BW_373600                            = 0x11,
    RX_BW_467000                            = 0x09,
}RadioRxBandwidth_t;

typedef enum
{
    LORA_SF5                                = 0x05,
    LORA_SF6                                = 0x06,
    LORA_SF7                                = 0x07,
    LORA_SF8                                = 0x08,
    LORA_SF9                                = 0x09,
    LORA_SF10                               = 0x0A,
    LORA_SF11                               = 0x0B,
    LORA_SF12                               = 0x0C,
}RadioLoRaSpreadingFactors_t;

typedef enum
{
    LORA_BW_500                             = 6,
    LORA_BW_250                             = 5,
    LORA_BW_125                             = 4,
    LORA_BW_062                             = 3,
    LORA_BW_041                             = 10,
    LORA_BW_031                             = 2,
    LORA_BW_020                             = 9,
    LORA_BW_015                             = 1,
    LORA_BW_010                             = 8,
    LORA_BW_007                             = 0,
}RadioLoRaBandwidths_t;

typedef enum
{
    LORA_CR_4_5                             = 0x01,
    LORA_CR_4_6                             = 0x02,
    LORA_CR_4_7                             = 0x03,
    LORA_CR_4_8                             = 0x04,
}RadioLoRaCodingRates_t;

typedef enum
{
    RADIO_PREAMBLE_DETECTOR_OFF             = 0x00,         //!< Preamble detection length off
    RADIO_PREAMBLE_DETECTOR_08_BITS         = 0x04,         //!< Preamble detection length 8 bits
    RADIO_PREAMBLE_DETECTOR_16_BITS         = 0x05,         //!< Preamble detection length 16 bits
    RADIO_PREAMBLE_DETECTOR_24_BITS         = 0x06,         //!< Preamble detection length 24 bits
    RADIO_PREAMBLE_DETECTOR_32_BITS         = 0x07,         //!< Preamble detection length 32 bit
}RadioPreambleDetection_t;

typedef enum
{
    RADIO_ADDRESSCOMP_FILT_OFF              = 0x00,         //!< No correlator turned on, i.e. do not search for SyncWord
    RADIO_ADDRESSCOMP_FILT_NODE             = 0x01,
    RADIO_ADDRESSCOMP_FILT_NODE_BROAD       = 0x02,
}RadioAddressComp_t;

typedef enum
{
    RADIO_PACKET_FIXED_LENGTH               = 0x00,         //!< The packet is known on both sides, no header included in the packet
    RADIO_PACKET_VARIABLE_LENGTH            = 0x01,         //!< The packet is on variable size, header included
}RadioPacketLengthModes_t;

typedef enum
{
    RADIO_CRC_OFF                           = 0x01,         //!< No CRC in use
    RADIO_CRC_1_BYTES                       = 0x00,
    RADIO_CRC_2_BYTES                       = 0x02,
    RADIO_CRC_1_BYTES_INV                   = 0x04,
    RADIO_CRC_2_BYTES_INV                   = 0x06,
    RADIO_CRC_2_BYTES_IBM                   = 0xF1,
    RADIO_CRC_2_BYTES_CCIT                  = 0xF2,
}RadioCrcTypes_t;

typedef enum
{
    RADIO_DC_FREE_OFF                       = 0x00,
    RADIO_DC_FREEWHITENING                  = 0x01,
}RadioDcFree_t;

typedef enum
{
    LORA_PACKET_VARIABLE_LENGTH             = 0x00,         //!< The packet is on variable size, header included
    LORA_PACKET_FIXED_LENGTH                = 0x01,         //!< The packet is known on both sides, no header included in the packet
    LORA_PACKET_EXPLICIT                    = LORA_PACKET_VARIABLE_LENGTH,
    LORA_PACKET_IMPLICIT                    = LORA_PACKET_FIXED_LENGTH,
}RadioLoRaPacketLengthsMode_t;

typedef enum
{
    LORA_CRC_ON                             = 0x01,         //!< CRC activated
    LORA_CRC_OFF                            = 0x00,         //!< CRC not used
}RadioLoRaCrcModes_t;

typedef enum
{
    LORA_IQ_NORMAL                          = 0x00,
    LORA_IQ_INVERTED                        = 0x01,
}RadioLoRaIQModes_t;

typedef enum
{
    TCXO_CTRL_1_6V                          = 0x00,
    TCXO_CTRL_1_7V                          = 0x01,
    TCXO_CTRL_1_8V                          = 0x02,
    TCXO_CTRL_2_2V                          = 0x03,
    TCXO_CTRL_2_4V                          = 0x04,
    TCXO_CTRL_2_7V                          = 0x05,
    TCXO_CTRL_3_0V                          = 0x06,
    TCXO_CTRL_3_3V                          = 0x07,
}RadioTcxoCtrlVoltage_t;

typedef enum
{
    IRQ_RADIO_NONE                          = 0x0000,
    IRQ_TX_DONE                             = 0x0001,
    IRQ_RX_DONE                             = 0x0002,
    IRQ_PREAMBLE_DETECTED                   = 0x0004,
    IRQ_SYNCWORD_VALID                      = 0x0008,
    IRQ_HEADER_VALID                        = 0x0010,
    IRQ_HEADER_ERROR                        = 0x0020,
    IRQ_CRC_ERROR                           = 0x0040,
    IRQ_CAD_DONE                            = 0x0080,
    IRQ_CAD_ACTIVITY_DETECTED               = 0x0100,
    IRQ_RX_TX_TIMEOUT                       = 0x0200,
    IRQ_RADIO_ALL                           = 0xFFFF,
}RadioIrqMasks_t;

typedef struct
{
    RadioPacketTypes_t                    packetType;       //!< Packet to which the packet status are referring to.
    unsigned short PacketReceived;
    unsigned short CrcOk;
    unsigned short LengthError;
}RxCounter_t;

typedef union
{
    struct
    {
        unsigned char RC64KEnable    : 1;                             //!< Calibrate RC64K clock
        unsigned char RC13MEnable    : 1;                             //!< Calibrate RC13M clock
        unsigned char PLLEnable      : 1;                             //!< Calibrate PLL
        unsigned char ADCPulseEnable : 1;                             //!< Calibrate ADC Pulse
        unsigned char ADCBulkNEnable : 1;                             //!< Calibrate ADC bulkN
        unsigned char ADCBulkPEnable : 1;                             //!< Calibrate ADC bulkP
        unsigned char ImgEnable      : 1;
        unsigned char                : 1;
    }Fields;
    unsigned char Value;
}CalibrationParams_t;

typedef union
{
    struct
    {
        unsigned char WakeUpRTC               : 1;                    //!< Get out of sleep mode if wakeup signal received from RTC
        unsigned char Reset                   : 1;
        unsigned char WarmStart               : 1;
        unsigned char Reserved                : 5;
    }Fields;
    unsigned char Value;
}SleepParams_t;

typedef union
{
    struct
    {
        unsigned char Rc64kCalib              : 1;                    //!< RC 64kHz oscillator calibration failed
        unsigned char Rc13mCalib              : 1;                    //!< RC 13MHz oscillator calibration failed
        unsigned char PllCalib                : 1;                    //!< PLL calibration failed
        unsigned char AdcCalib                : 1;                    //!< ADC calibration failed
        unsigned char ImgCalib                : 1;                    //!< Image calibration failed
        unsigned char XoscStart               : 1;                    //!< XOSC oscillator failed to start
        unsigned char PllLock                 : 1;                    //!< PLL lock failed
        unsigned char BuckStart               : 1;                    //!< Buck converter failed to start
        unsigned char PaRamp                  : 1;                    //!< PA ramp failed
        unsigned char                         : 7;                    //!< Reserved
    }Fields;
    unsigned short Value;
}RadioError_t;

typedef enum
{
    USE_LDO                                 = 0x00, // default
    USE_DCDC                                = 0x01,
}RadioRegulatorMode_t;

typedef enum
{
    MODE_SLEEP                              = 0x00,         //! The radio is in sleep mode
    MODE_STDBY_RC,                                          //! The radio is in standby mode with RC oscillator
    MODE_STDBY_XOSC,                                        //! The radio is in standby mode with XOSC oscillator
    MODE_FS,                                                //! The radio is in frequency synthesis mode
    MODE_TX,                                                //! The radio is in transmit mode
    MODE_RX,                                                //! The radio is in receive mode
    MODE_RX_DC,                                             //! The radio is in receive duty cycle mode
    MODE_CAD                                                //! The radio is in channel activity detection mode
}RadioOperatingModes_t;

typedef struct
{
    bool Previous;
    bool Current;
}RadioPublicNetwork_t;

typedef struct
{
    RadioPacketTypes_t                    packetType;      //!< Packet to which the packet status are referring to.
    struct
    {
        struct
        {
            unsigned char RxStatus;
            char RssiAvg;                                //!< The averaged RSSI
            char RssiSync;                               //!< The RSSI measured on last packet
            unsigned int FreqError;
        }Gfsk;
        struct
        {
            char RssiPkt;                                //!< The RSSI of the last packet
            char SnrPkt;                                 //!< The SNR of the last packet
            char SignalRssiPkt;
            unsigned int FreqError;
        }LoRa;
    }Params;
}PacketStatus_t;

typedef enum
{
    RADIO_MODE_FSK                          = 0x00,
    RADIO_MODE_LORA                         = 0x01,
}TX_RX_Mode_t;

typedef struct
{
    spi_host_device_t spi_id;        ///< spi host id for communication with radio chip
    TX_RX_Mode_t radio_mode;         ///< 0=FSK  1=LoRa
    int spi_speed_hz;                ///< spi communication speed
    int miso_io;                     ///< GPIO pin for spi miso
    int mosi_io;                     ///< GPIO pin for spi mosi
    int sclk_io;                     ///< GPIO pin for spi sclk
    int cs_io;                       ///< GPIO pin for spi cs0
    int busy_io;                     ///< GPIO pin for radio chip busy signal
    int rst_io;                      ///< GPIO pin for radio chip rst command
    int dio1_io;                     ///< GPIO pin for radio chip irq signal, or -1 if not used.
    int rxen_io;                     ///< GPIO pin for radio chip ant rx enable, or -1 if not used.
    int txen_io;                     ///< GPIO pin for radio chip ant tx enable, or -1 if not used.
} ebyte_config_t;

typedef struct
{
    spi_device_handle_t spi;
    spi_host_device_t spi_id;        ///< spi host id for communication with radio chip
    SemaphoreHandle_t xmutex;           ///< group level spinlock
    RadioOperatingModes_t OperatingMode;
    RadioPacketTypes_t PacketType;
    PacketStatus_t PacketStatus;
    TX_RX_Mode_t rdo_mode;
    uint32_t FrequencyError;         
    uint16_t irqStatus;              ///< radio chip irq status
    int cs_io;                       ///< GPIO pin for spi cs0
    int busy_io;                     ///< GPIO pin for radio chip busy signal
    int rst_io;                      ///< GPIO pin for radio chip rst command
    int rxen_io;                     ///< GPIO pin for radio chip ant rx enable
    int txen_io;                     ///< GPIO pin for radio chip ant tx enable
    uint8_t recv_len;                ///< radio received data len (bytes)
    uint8_t* RecvPacketBuffer;       ///< receive data buffer
} ebyte_status_t;

void Ebyte_E220x_Init( ebyte_status_t *ebyte_status );
void Ebyte_E220x_SetRfFrequency( ebyte_status_t *ebyte_status, uint32_t frequency );
void Ebyte_E220x_SetRx( ebyte_status_t *ebyte_status, uint32_t timeout );
uint16_t Ebyte_E220x_SendPayload( ebyte_status_t *ebyte_status, uint8_t *payload, uint8_t size, uint32_t timeout );
void Ebyte_E220x_ClearIrqStatus( ebyte_status_t *ebyte_status, uint16_t irq );
void Ebyte_E220x_SetDioIrqParams( ebyte_status_t *ebyte_status, uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask );
void Ebyte_E220x_ClearDeviceErrors( ebyte_status_t *ebyte_status );
uint16_t Ebyte_E220x_IntOrPollTask( ebyte_status_t *ebyte_status, uint8_t *receive_data );
void Ebyte_E220x_SetSleep( ebyte_status_t *ebyte_status, uint8_t cmd );
void Ebyte_E220x_Install( ebyte_config_t ebyte_config, ebyte_status_t *ebyte_status );
void Ebyte_E220x_Unstall( ebyte_status_t *ebyte_status );