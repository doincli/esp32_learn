/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "sdkconfig.h"
#include "ebyte_kfifo.h"
#include "ebyte.h"
#include "math.h"

#define EBYTE_HOST      SPI2_HOST

#define PIN_NUM_MISO    5
#define PIN_NUM_MOSI    6
#define PIN_NUM_CLK     7
#define PIN_NUM_CS      0

#define PIN_NUM_BUSY    2
#define PIN_NUM_RST     3

typedef struct data
{
    uint8_t data[5];
    uint8_t seq;
    uint8_t old_data[5];
    uint8_t old_seq;
}frame_data;

typedef frame_data * frame_handle;

void refresh(frame_handle frame){
   
    for (int i = 0; i < 4; i++)
    {
        frame->old_data[i] = frame->data[i];
        frame->data[i]++;
    }
    frame->old_seq = frame->seq;
    frame->old_data[4] = frame->old_seq;
    frame->seq++;
    frame->data[4] = frame->seq;
}

void app_main(void)
{   
    frame_data frame1 = {
        .data = {1,2,3,4,1},
        .seq = 1,
        .old_data = {0},
        .old_seq = 0,
    };

    Ebyte_FIFO_t my_fifo;
    uint8_t pingLength = 0;
    //uint8_t rev_data[EBYTE_MAX_SIZE];
    for(int i = 0; i < 3; ++i){
        printf("start: %d\n", i);
        ets_delay_us(10000);
    }

    Ebyte_FIFO_Init(&my_fifo);

    ebyte_status_t my_status;

    ebyte_config_t my_ebyte_config = {
        .spi_id = EBYTE_HOST,
        .radio_mode = RADIO_MODE_LORA,
        .spi_speed_hz = 10 * 1000 * 1000,
        .miso_io = PIN_NUM_MISO,
        .mosi_io = PIN_NUM_MOSI,
        .sclk_io = PIN_NUM_CLK,
        .cs_io = PIN_NUM_CS,
        .busy_io = PIN_NUM_BUSY,
        .rst_io = PIN_NUM_RST,
        .dio1_io = -1,
        .rxen_io = -1,
        .txen_io = -1,
    };

    Ebyte_Init( my_ebyte_config, &my_status );

    printf("init done\n");


    pingLength = 5;
    //uint8_t len = 0;
    Ebyte_Send( &my_status, frame1.data, pingLength, 0) ;
    for (int i = 0; i < pingLength; i++)
    {
        printf("%d, ",frame1.data[i]);
    }
    printf("\n");
    refresh(&frame1);
    uint8_t rec_seq[100] = {1,1,1,1,1};
    printf("frame seq = %d\n",frame1.seq);
    printf("data seq = %d\n",frame1.data[4]);

    while(1){
        Ebyte_Receive(&my_status, rec_seq, 1000);
        printf("seq  is %d\n",rec_seq[0]);
       // printf("frame 1 seq  is %d\n",frame1.seq);
        if (abs(rec_seq[0] - frame1.old_data[4]) > 1)
        {
            printf("step 1\n");
            frame1.seq = rec_seq[0];
            frame1.data[4] = rec_seq[0];
        }
        
        if (frame1.seq == rec_seq[0])
        {
            //接收正确
            Ebyte_Send( &my_status, frame1.data, pingLength, 0 );
            for (int i = 0; i < pingLength; i++)
            {
                printf("%d, ",frame1.data[i]);
            }
            printf("\n");
            refresh(&frame1);
            // printf("frame seq = %d\n",frame1.seq);
            // printf("data seq = %d\n",frame1.data[4]);
        }else
        {
            //接收错误
            printf("send error\n");
             for (int i = 0; i < pingLength; i++)
            {
                printf("%d, ",frame1.old_data[i]);
            }
            printf("\n");
            Ebyte_Send( &my_status, frame1.old_data, pingLength, 0 );
        }
        ets_delay_us(500);
    }
    Ebyte_DeInit( &my_status );
}


