#include "app.h"
#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "product_config.h"
#include "rfm300_driver.h"
#include "cmt2300_regs.h"
#include "esp_log.h"

#define USE_HRF_MODE 1

void* subg_handle = NULL;

uint8_t rec_seq[100] = {0};


#if 0 //ESP32
#define CMT2300_CSB  18
#define CMT2300_FCSB 15
#define CMT2300_SCLK 14
#define CM2300_SDIO  17

#define EBYTE_MISO 17
#define EBYTE_MOSI 18
#define EBYTE_SCLK 15
#define EBYTE_CS   14
#define EBYTE_BUSY 13
#define EBYTE_RST   2
#elif 1 //ESP32C3
#define CMT2300_CSB  5
#define CMT2300_FCSB 7
#define CMT2300_SCLK 6
#define CM2300_SDIO  4

#define EBYTE_MISO 4
#define EBYTE_MOSI 5
#define EBYTE_SCLK 7
#define EBYTE_CS   6
#define EBYTE_BUSY 1
#define EBYTE_RST  0
#endif


void app_subg_init(){

    #if USE_HRF_MODE
    // cmt2300_handle_t dev
	subg_handle = cmt2300_init(
						CMT2300_CSB, //int csb, 
						CMT2300_FCSB, //int fcsb, 
						CMT2300_SCLK, //int sclk, 
						CM2300_SDIO, //int sdio, 
						-1, //int intr1, 
						-1, //int intr2, 
						-1, //int intr3
						#ifdef CONFIG_IDF_TARGET_ESP32
						VSPI_HOST // int spi_hw_if
						#else
						SPI2_HOST // int spi_hw_if
						#endif					
						);
    #else
    ebyte_config_t my_ebyte_config = {
        .spi_id = EBYTE_HOST,
        .radio_mode = RADIO_MODE_LORA,
        .spi_speed_hz = 5 * 1000 * 1000,
        .miso_io = EBYTE_MISO,
        .mosi_io = EBYTE_MOSI,
        .sclk_io = EBYTE_SCLK,
        .cs_io = EBYTE_CS,
        .busy_io = EBYTE_BUSY,
        .rst_io =  EBYTE_RST,
        .dio1_io = -1,
        .rxen_io = -1,
        .txen_io = -1,
    };
    subg_handle = Ebyte_Init(my_ebyte_config);
    #endif
}

void app_subg_send_and_recv(uint32_t ticks_to_wait, uint16_t data, uint8_t retry)
{
    uint8_t send_buf[3] = {0};
    uint8_t seq = rand() % 256;
    for (int i = 0; i < Frame_len - 1; i++)
    {
        send_buf[Frame_len-i - 2] = (uint8_t) data;
        data = (data>>8);
    }
    send_buf[Frame_len-1] = seq;

    int times = 0;

    #if USE_HRF_MODE
    cmt2300_send_data(subg_handle, send_buf, Frame_len);
    #else
    Ebyte_Send( subg_handle, send_buf, Frame_len, ticks_to_wait );
    #endif

    // printf("send size is %d\n",size1);
    while (1)
    {   
        int rssi = 0;
        #if USE_HRF_MODE
		int size = cmt2300_recv_data(subg_handle, rec_seq, &rssi, ticks_to_wait);
        #else
        int size = Ebyte_Receive(subg_handle, rec_seq, ticks_to_wait);
        #endif

        printf("rec seq is %d,size is %d, rssi: %d\n",rec_seq[0], size, rssi);
        if (rec_seq[0] == seq)
        {
            printf("rec right\n");
            return;
        } else {
            printf("rec error\n");

            #if USE_HRF_MODE
            cmt2300_send_data(subg_handle, send_buf, Frame_len);
            #else
            Ebyte_Send( subg_handle, send_buf, Frame_len, 0 );
            #endif
            times++;
            if (times == retry)
            {
                printf("retry error\n");
                return;
            }
        }
    }   
}

void app_subg_tx_test()
{
    while (1) {
        app_subg_send_and_recv(3000, 0x1234, 3);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_subg_rx_test(void* arg)
{
    while (1) {
        esp_rom_printf("check recv\n");
        #if USE_HRF_MODE
		int rssi = 0;
		int size = cmt2300_recv_data(subg_handle, rec_seq, &rssi, 5000 / portTICK_PERIOD_MS);
        #else
        int size = Ebyte_Receive(subg_handle, rec_seq, 5000);
        #endif
        esp_rom_printf("check recv len: %d\n", size);
        uint8_t seq = rec_seq[2];

        #if USE_HRF_MODE
        cmt2300_send_data(subg_handle, &seq, 1);
        #else
        Ebyte_Send( subg_handle, &seq, 1, 2000 );
        #endif
        // vTaskDelay(20);
    }   
}

void app_subg_rx_test_task()
{
    xTaskCreate(app_subg_rx_test, "app_subg_rx_test", 4096, NULL, 5, NULL);

}

