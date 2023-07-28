#include <string.h>
#include "ebyte_port.h"
#include "driver/gpio.h"

/* !
 * @brief 
 * 
 * @param send command and receive data from radio chip
 * @note Ebyte_Port_SpiTransmitAndReceivce
 */
void Ebyte_Port_SpiTransmitAndReceivce( spi_device_handle_t spi, uint8_t *send, uint16_t tx_len, uint8_t *receive, uint16_t rx_len )
{
    spi_device_acquire_bus(spi, portMAX_DELAY);

    esp_err_t ret;
    spi_transaction_t t;

    memset(&t, 0, sizeof(t));
    t.length = tx_len * 8;
    t.tx_buffer = send;
    if (rx_len > 0) {
        t.flags = SPI_TRANS_CS_KEEP_ACTIVE;
    }
    ret = spi_device_polling_transmit(spi, &t);
    assert( ret == ESP_OK );

    if (rx_len > 0) {
        memset(&t, 0, sizeof(t));
        t.length = rx_len * 8;
        t.rx_buffer = receive;
        ret = spi_device_polling_transmit(spi, &t);
        assert( ret == ESP_OK );
    }

    // Release bus
    spi_device_release_bus(spi);
}

/* !
 * @brief Controls the state of pins
 * 
 * @param cmd pin status
 *            @arg 0: disable
 *            @arg 1: enable
 * @note Ebyte_Port_IoControl
 */
void Ebyte_Port_IoControl( int io_num, uint8_t cmd )
{
    assert ( GPIO_IS_VALID_GPIO(io_num) );

    gpio_set_level(io_num, cmd);
}

/* !
 * @brief Get the status information of the busy pin
 * 
 * @note Ebyte_Port_BusyIoRead
 */
int Ebyte_Port_IoRead( int io_num )
{ 
    assert ( GPIO_IS_VALID_GPIO(io_num) );
    int result = 0 ;

    result = gpio_get_level(io_num);
    return result;
}

/* !
 * @brief Millisecond delay function
 * 
 * @param time: delay time(ms)
 */
void Ebyte_Port_DelayMs( uint16_t time )
{
    uint16_t i,n;
    
    while(time--)
    {
        for(i=900;i>0;i--)
        {
              for( n=1 ;n>0 ;n--)
              {
                  asm("nop"); 
                  asm("nop");
                  asm("nop");
                  asm("nop");
              }
        }
    }    
}



