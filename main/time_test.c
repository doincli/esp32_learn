#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include  <stdio.h>
#include  "stdlib.h"
#include  "string.h"
TaskHandle_t task;
#define TAG "heap_test"
TimerHandle_t myTimer;

void myTimerCallback(TimerHandle_t xTimer)
{
    ESP_LOGE("TAG","liboyu");
}

void task1(){
    while (1)
    {
        ESP_LOGI("TAG","task1");
    }
    

}

void app_main()
{
    myTimer = xTimerCreate("MyTimer", pdMS_TO_TICKS(1000), pdTRUE, NULL, myTimerCallback);
    xTimerStart(myTimer,0);
    xTaskCreate(task1,"task1",4096,NULL,3,NULL);
}