#include <stdio.h>
#include "ebyte_callback.h"
#include "ebyte_conf.h"
#include "esp32c6/rom/ets_sys.h"

/* !
 * @brief 
 *
 * @param state 
 *
 * @note E22x 
 *         IRQ_TX_DONE                             = 0x0001,
 *         IRQ_RX_DONE                             = 0x0002,
 *         IRQ_PREAMBLE_DETECTED                   = 0x0004,
 *         IRQ_SYNCWORD_VALID                      = 0x0008,
 *         IRQ_HEADER_VALID                        = 0x0010,
 *         IRQ_HEADER_ERROR                        = 0x0020,
 *         IRQ_CRC_ERROR                           = 0x0040,
 *         IRQ_CAD_DONE                            = 0x0080,
 *         IRQ_CAD_ACTIVITY_DETECTED               = 0x0100,
 *         IRQ_RX_TX_TIMEOUT                       = 0x0200,
 */
void Ebyte_Port_TransmitCallback( uint16_t state )
{
#if EBYTE_PRINTF_IRQ_STATUS
    if((state & 0x0001) ==  0x0001 ) {
        ets_printf("tx done!\n");
    }
    else if((state & 0x0200) ==  0x0200) {
        ets_printf("tx timeout!\n");
    }
    else{
        ets_printf("tx error!\n");
    }
#endif
}

/* !
 * @brief 
 *
 * @param state 
 *
 * @note E22x 
 *         IRQ_TX_DONE                             = 0x0001,
 *         IRQ_RX_DONE                             = 0x0002,
 *         IRQ_PREAMBLE_DETECTED                   = 0x0004,
 *         IRQ_SYNCWORD_VALID                      = 0x0008,
 *         IRQ_HEADER_VALID                        = 0x0010,
 *         IRQ_HEADER_ERROR                        = 0x0020,
 *         IRQ_CRC_ERROR                           = 0x0040,
 *         IRQ_CAD_DONE                            = 0x0080,
 *         IRQ_CAD_ACTIVITY_DETECTED               = 0x0100,
 *         IRQ_RX_TX_TIMEOUT                       = 0x0200,
 */
void Ebyte_Port_ReceiveCallback( uint16_t state, uint8_t *ebyte_buffer, uint8_t *buffer, uint8_t len )
{
    if((state & 0x0002) ==  0x0002 ) {
        // ets_printf("rx done!\n");
        for(int i = 0; i < len; i++ )
        {
            buffer[i] = ebyte_buffer[i];
        }
    }
#if EBYTE_PRINTF_IRQ_STATUS
    else if((state & 0x0200) ==  0x0200) {
        ets_printf("rx timeout!\n");
    }
    else{
        ets_printf("rx error!\n");
    }
#endif
}