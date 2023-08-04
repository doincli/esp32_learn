#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include  <stdio.h>

#define TAG "heap_test"
void *pvParameters = "liboyu";

EventGroupHandle_t eventGroup;

// 定义事件位标志
#define EVENT_BIT_0 (1 << 0)
#define EVENT_BIT_1 (1 << 2)

// Task1函数
void Task1(void* pvParameters) {
  while (1) {
    // 做一些工作...
    // 设置事件位0以通知Task2
    xEventGroupSetBits(eventGroup, EVENT_BIT_0);
    ESP_LOGI("TAG","set 0");
    vTaskDelay(pdMS_TO_TICKS(1500));
    ESP_LOGI("TAG","set 1");
    xEventGroupSetBits(eventGroup, EVENT_BIT_1);
    // 等待一段时间后设置下一个事件
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Task2函数
void Task2(void* pvParameters) {
  while (1) {
    // 等待事件位0被设置
    ESP_LOGI("TAG","init");
    
    EventBits_t eventBits = xEventGroupWaitBits(
        eventGroup,        // 事件组句柄
        EVENT_BIT_0|EVENT_BIT_1,       // 等待的事件位标志
        pdTRUE,            // 等待后清除事件位标志
        pdTRUE,           // 等待所有指定位标志
        portMAX_DELAY      // 无限期等待
    );
    EventBits_t eventBits1 = xEventGroupGetBits(eventGroup);
    printf("wait is %ld,EVENT IS %ld",eventBits,eventBits1);
    // 检查事件位0是否被设置
    if ((eventBits & EVENT_BIT_0) != 0) {
      ESP_LOGI("TAG","rec 0");
      // 例如，切换LED，发送消息等。
    }
      if ((eventBits & EVENT_BIT_1) != 0) {
      ESP_LOGI("TAG","rec 1");
      // 例如，切换LED，发送消息等。
    }
  }
}
int app_main() {
  // 创建事件组
  eventGroup = xEventGroupCreate();
  // 创建Task1
  xTaskCreate(Task1, "Task1", 4096, NULL, 1, NULL);

  // 创建Task2
  xTaskCreate(Task2, "Task2", 4096, NULL, 2, NULL);


  // 调度器不应该返回，但为了安全起见，在此添加一个无限循环
  while (1) {
    // 什么都不做
  }

  return 0;
}






