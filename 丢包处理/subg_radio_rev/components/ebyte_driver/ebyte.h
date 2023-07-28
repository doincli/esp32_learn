#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "ebyte_core.h"
#include "esp32c6/rom/ets_sys.h"

void Ebyte_Init( ebyte_config_t ebyte_config, ebyte_status_t *ebyte_status );
int Ebyte_Send( ebyte_status_t *ebyte_status, uint8_t *payload, uint8_t size, uint32_t timeout_ms );
int Ebyte_Receive( ebyte_status_t *ebyte_status, uint8_t *payload, uint32_t timeout_ms );
void Ebyte_DeInit( ebyte_status_t *ebyte_status );