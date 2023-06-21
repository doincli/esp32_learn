/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "OBD_simulator.h"

void app_main(void)
{
    uint32_t now_speed;
    OBD_twai_init();
//    OBD_twai_deinit();
    printf("初始化ok\n");
    for(int i = 0; i < 10; ++i){
         printf("获取数据\n");
        now_speed = OBD_get_engine_speed_val();
        printf("engine speed:%ld\n", now_speed);
    }

    OBD_twai_deinit();
}
