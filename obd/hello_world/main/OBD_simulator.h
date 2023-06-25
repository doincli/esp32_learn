// Copyright 2015-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdbool.h>
#include "esp_err.h"
#include "driver/twai.h"
#include "esp32c3/rom/ets_sys.h"

/**
 * @brief Initialize TWAI peripheral
 *
 * puts TWAI peripheral into reset mode, then sets some
 * registers with default values.
 * 
 */
void OBD_twai_init_500(void);
void OBD_twai_init_250(void);
/**
 * @brief Deinitialize the TWAI peripheral
 *
 * Clears any unhandled interrupts and unsets peripheral
 *
 */
void OBD_twai_deinit(void);

/**
 * @brief Get engine speed from OBD simulator
 *
 * Send instructions according to the specified format 
 * to obtain the transmitter speed information
 *
 * @return engine speed value.
 */
uint32_t OBD_get_engine_speed_val_protocol_11bit(void);
uint32_t OBD_get_engine_speed_val_protocol_29bit(void);

