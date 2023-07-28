#include "ebyte_port.h"

void Ebyte_Port_TransmitCallback( uint16_t state );
void Ebyte_Port_ReceiveCallback( uint16_t state, uint8_t *ebyte_buffer, uint8_t *buffer, uint8_t len );