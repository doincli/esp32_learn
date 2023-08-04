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

TaskHandle_t task1Handle, task2Handle;

// 任务1：发送任务通知
void Task1(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  uint32_t notificationValue = 42;

  while (1) {
    // 发送任务通知给任务2，并将通知值设置为 notificationValue
    xTaskNotify(task2Handle, notificationValue, eSetValueWithOverwrite);

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
  }
}

configTIMER_TASK_PRIORITY
// 任务2：接收任务通知
void Task2(void *pvParameters) {
    while (1) {
        // 等待任务通知，等待时间为 1 秒
        uint32_t ulReceivedValue = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000));

        if (ulReceivedValue != 0) {
            // 接收到任务通知，处理通知值 ulReceivedValue
            // ...
        } else {
            // 等待超时或未接收到任务通知
            // ...
        }
    }
}

void app_main()
{

}