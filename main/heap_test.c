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
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        a++;
        ESP_LOGI(TAG,"a is %d",a);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void *pvParameters = "liboyu";
void vTaskFunction( void *pvParameters )
{
    char *pcTaskName;
    TickType_t xLastWakeTime;

    /* 要打印的字符串通过参数传入。将此转换为字符指针。 */
    pcTaskName = ( char * ) pvParameters;

    /* 需要使用当前滴针计数初始化 xLastWakeTime 变量。注意，这是唯一一次显式地写入变量。
    在此之后，xLastWakeTime 将在 vTaskDelayUntil() 中自动更新。 */
    xLastWakeTime = xTaskGetTickCount();
    /* 对于大多数任务，都是在一个无限循环中实现的。 */
    for( ;; )
    {
        /* 打印出此任务的名称。 */
        ESP_LOGI(TAG,"%s",pcTaskName);

        /* 这个任务应该精确地每 250 毫秒执行一次。根据 vTaskDelay() 函数，时间是以滴答为
        单位度量的，pdMS_TO_TICKS() 宏用于将毫秒转换为滴答。xLastWakeTime 在 
        vTaskDelayUntil() 中自动更新，因此任务不会显式地更新 xLastWakeTime。 */
        vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 250 ));
    }
}


void app_main(void)
{
    xTaskCreate(vTaskFunction,"task1",5000,pvParameters,4,&task);
    xTaskCreate(task2,"task1",5000,NULL,2,NULL);
}