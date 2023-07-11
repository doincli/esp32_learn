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

static const char *TAG = "i2c-example";

// #define _I2C_NUMBER(num) I2C_NUM_##num
// #define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */


#define I2C_SLAVE_SCL_IO 4               /*!< gpio number for i2c slave clock */
#define I2C_SLAVE_SDA_IO 5               /*!< gpio number for i2c slave data */
#define I2C_SLAVE_NUM 0 /*!< I2C port number for slave dev */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave rx buffer size */
#define ESP_SLAVE_ADDR 0x23             /*!< ESP32 slave address, you can set any 7bit value */


#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */

SemaphoreHandle_t print_mux = NULL;

/**
 * @brief i2c slave initialization
 */
static esp_err_t i2c_slave_init(void)
{
    int i2c_slave_port = I2C_SLAVE_NUM;
    i2c_config_t conf_slave = {
        .sda_io_num = I2C_SLAVE_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_SLAVE_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .mode = I2C_MODE_SLAVE,
        .slave.addr_10bit_en = 0,
        .slave.slave_addr = ESP_SLAVE_ADDR,
    };
    esp_err_t err = i2c_param_config(i2c_slave_port, &conf_slave);
    if (err != ESP_OK) {
        return err;
    }
    return i2c_driver_install(i2c_slave_port, conf_slave.mode, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0);
}

/**
 * @brief test function to show buffer
 */
static void disp_buf(uint8_t *buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}
//!CONFIG_IDF_TARGET_ESP32C3

static void i2c_test_task(void *arg)
{
    int ret = ESP_OK;
    int task_idx = (int)arg;

    int i = 0;
    uint8_t *data = (uint8_t *)malloc(DATA_LENGTH);
    uint8_t *data_wr = (uint8_t *)malloc(DATA_LENGTH);
    uint8_t *data_rd = (uint8_t *)malloc(DATA_LENGTH);

        for (i = 0; i < DATA_LENGTH; i++) {
            data[i] = i + 32;
        }
        xSemaphoreTake(print_mux, portMAX_DELAY);
        size_t d_size = i2c_slave_write_buffer(I2C_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_PERIOD_MS);
       
        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, "I2C Timeout");
        } else if (ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER READ FROM SLAVE\n", task_idx);
            printf("*******************\n");
            printf("====TASK[%d] Slave buffer data ====\n", task_idx);
            disp_buf(data, d_size);
            printf("====TASK[%d] Master read ====\n", task_idx);
            disp_buf(data_rd, d_size);
        } else {
            ESP_LOGW(TAG, "TASK[%d] %s: Master read slave error, IO not connected...",
                     task_idx, esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_PERIOD_MS);
        //---------------------------------------------------
        int size;
        for (i = 0; i < DATA_LENGTH; i++) {
            data_wr[i] = i + 10;
        }
        xSemaphoreTake(print_mux, portMAX_DELAY);
        //we need to fill the slave buffer so that master can read later
       // ret = i2c_master_write_slave(I2C_MASTER_NUM, data_wr, RW_TEST_LENGTH);
        if (ret == ESP_OK) {
            size = i2c_slave_read_buffer(I2C_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_PERIOD_MS);
             printf("len is %d\n",d_size);
        }
        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, "I2C Timeout");
        } else if (ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER WRITE TO SLAVE\n", task_idx);
            printf("*******************\n");
            printf("----TASK[%d] Master write ----\n", task_idx);
            disp_buf(data_wr, RW_TEST_LENGTH);
            printf("----TASK[%d] Slave read: [%d] bytes ----\n", task_idx, size);
            disp_buf(data, size);
        } else {
            ESP_LOGW(TAG, "TASK[%d] %s: Master write slave error, IO not connected....",
                     task_idx, esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux);
        vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_PERIOD_MS);
    
    vSemaphoreDelete(print_mux);
    vTaskDelete(NULL);
}

void app_main(void)
{
    print_mux = xSemaphoreCreateMutex();
    ESP_ERROR_CHECK(i2c_slave_init());
    xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL);
}
