#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include  <stdio.h>

#define TAG "heap_test"
void *pvParameters = "liboyu";

QueueHandle_t  xQueue1;
int32_t * buf[100] = {0};

void app_main()
{
    xQueue1 = xQueueCreate(5,sizeof(int32_t));
    xQueueSend(xQueue1,5,pdMS_TO_TICKS(1000));
    xQueueReceive(xQueue1,buf,pdMS_TO_TICKS(500));
    SemaphoreHandle_t sem;
    sem = xSemaphoreCreateBinary();
    xSemaphoreGive(sem);
}