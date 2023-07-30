#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include  <stdio.h>
TaskHandle_t task;
#define TAG "heap_test"
void task1(){
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        UBaseType_t  aa = uxTaskGetStackHighWaterMark(task);
        ESP_LOGW(TAG,"task heap is %d",aa);
    }
}

void task2(){
    int a = 20;
    while (1)
    {
        a++;
        ESP_LOGI(TAG,"a is %d",a);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
void app_main(void)
{
    xTaskCreate(task2,"task1",5000,NULL,2,&task);
    xTaskCreate(task1,"task1",5000,NULL,4,NULL);
    
}