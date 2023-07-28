#include "ebyte_conf.h"
#include "ebyte_e220x.h"
#include "driver/spi_master.h"

typedef struct
{
    void   ( *Install )( ebyte_config_t ebyte_config, ebyte_status_t *ebyte_status );
    void   ( *Unstall )( ebyte_status_t *ebyte_status );
    void   ( *Init )( ebyte_status_t *ebyte_status );
    uint16_t   ( *Send )( ebyte_status_t *ebyte_status, uint8_t *payload, uint8_t size, uint32_t timeout );
    void   ( *EnterSleepMode )( ebyte_status_t *ebyte_status, uint8_t cmd );    
    void   ( *EnterReceiveMode )( ebyte_status_t *ebyte_status, uint32_t timeout );  
    uint16_t   ( *StartPollTask)( ebyte_status_t *ebyte_status, uint8_t *receive_data );
}Ebyte_RF_t; 


extern const Ebyte_RF_t Ebyte_RF;
