#include "esp_system.h"
#include "esp_err.h"
#include "driver/spi_master.h"

void Ebyte_Port_IoControl( int io_num, uint8_t cmd );
int Ebyte_Port_IoRead( int io_num );
void Ebyte_Port_DelayMs( uint16_t time );
void Ebyte_Port_SpiTransmitAndReceivce( spi_device_handle_t spi, uint8_t *send, uint16_t tx_len, uint8_t *receive, uint16_t rx_len );



