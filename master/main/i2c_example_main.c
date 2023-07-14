/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* i2c - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include <string.h>
static const char *TAG = "i2c-example";

#define SLAVE_ADDR  0x23
#define MASTER_BUFF_LEN 16
#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM CONFIG_I2C_MASTER_PORT_NUM /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY        /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */


SemaphoreHandle_t print_mux = NULL;

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK) {
        return err;
    }
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}


static void i2c_test_task(void *arg)
{
    esp_err_t ret = ESP_OK;
    int task_idx = (int)arg;

    uint8_t sensor_data[20] = {0};
    uint8_t read_buf[1024] = {0};
    memset(read_buf,0,1024);
    for (int i = 0; i < 20; i++)
    {
        sensor_data[i] = i + 50;
    }
    

    while (1) {
        ret = i2c_master_write_to_device(I2C_MASTER_NUM,SLAVE_ADDR,sensor_data,20,portTICK_PERIOD_MS);

        //&read_buf[1]
        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, "I2C Timeout");
        } else if (ret == ESP_OK) {
            ret = i2c_master_read_from_device(I2C_MASTER_NUM,SLAVE_ADDR,read_buf,MASTER_BUFF_LEN,portTICK_PERIOD_MS);
            if (ret == ESP_OK)
            {   
                xSemaphoreTake(print_mux, portMAX_DELAY);
                //printf("master rec len is %d\n",len);
                printf("idx %d,the rec data is \n", task_idx);
                for (int i = 0; i < MASTER_BUFF_LEN; i++)
                {
                    printf("%x,",read_buf[i]);
                }
                printf("\n");
                printf("*******************\n");
            }                      
        } else {
            ESP_LOGW(TAG, "%s: No ack, sensor not connected...skip...", esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay(300);
        //---------------------------------------------------
    }
    vSemaphoreDelete(print_mux);
    vTaskDelete(500);
}

void app_main(void)
{
    print_mux = xSemaphoreCreateMutex();
    ESP_ERROR_CHECK(i2c_master_init());
    xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 10, (void *)0, 10, NULL);
}
