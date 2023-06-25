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


enum 
{
    ISO15765_11bit_500K=1, 
    ISO15765_11bit_250K, 
    ISO15765_29bit_500K, 
    ISO15765_29bit_250K
}protocol;

uint8_t protocol_cur = ISO15765_11bit_500K; //当前协议
uint32_t now_speed; //汽车的速度


void app_main(void)
{
	
    while (1)
    {   
        vTaskDelay(100);
        
        //状态机实现轮询探测
        switch (protocol_cur)
        {
        case ISO15765_11bit_500K:
            printf("ISO15765_11bit_500K start\n");
            //配置这个协议得TWAI总线
            OBD_twai_init_500();
            //获取车速，获取失败则进入下一个协议继续探测，剩下同理
            now_speed = OBD_get_engine_speed_val_protocol_11bit();
            if (now_speed == -1)
            {
                protocol_cur = ISO15765_11bit_250K;
                printf("next protocol is ISO15765_11bit_250K\n");
              // 
            }else{
                printf("speed of car is %ld\n",now_speed);
            }
             OBD_twai_deinit();
        break;

        case ISO15765_11bit_250K:
            printf("ISO15765_11bit_250K start\n");
            OBD_twai_init_250();
            now_speed = OBD_get_engine_speed_val_protocol_11bit();
            if (now_speed == -1)
            {
                protocol_cur = ISO15765_29bit_500K;
                //
            }else{
                printf("speed of car is %ld\n",now_speed);
            }
            OBD_twai_deinit();
        break;

        case ISO15765_29bit_500K:
            printf("ISO15765_29bit_500K start\n");
            OBD_twai_init_500();
            now_speed = OBD_get_engine_speed_val_protocol_29bit();
            if (now_speed == -1)
            {
                protocol_cur = ISO15765_29bit_250K;
               // OBD_twai_deinit();
            }else{
                printf("speed of car is %ld\n",now_speed);
            }
            OBD_twai_deinit();
        break;

        case ISO15765_29bit_250K:
            printf("ISO15765_29bit_250K start\n");
            OBD_twai_init_250();
            now_speed = OBD_get_engine_speed_val_protocol_29bit();
            if (now_speed == -1)
            {
                protocol_cur = ISO15765_11bit_500K;
               // OBD_twai_deinit();
            }else{
                printf("speed of car is %ld\n",now_speed);
            }
            OBD_twai_deinit();
        break;

        default:
            printf("event error\n");
            break;
        }
    }
   
}
