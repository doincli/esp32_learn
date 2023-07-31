#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include  <stdio.h>
TaskHandle_t task;
#define TAG "heap_test"
 void *pvParameters = "liboyu";
void vContinuousProcessingTask( void *pvParameters )
{
    char *pcTaskName;
    
    /* 要打印的字符串通过参数传入。将此转换为字符指针。 */
    pcTaskName = ( char * ) pvParameters;

    /* 对于大多数任务，都务是在一个无限循环中实现的。 */
    for( ;; )
    {
        /* 打印出此任务的名称。这个任务只是重复地做这件事，从不阻塞或延迟。 */
        ESP_LOGI(TAG,"liboyu");
    }
}

void vPeriodicTask( void *pvParameters )
{
    TickType_t xLastWakeTime;
    const TickType_t xDelay3ms = pdMS_TO_TICKS( 30 );

    /* 需要使用当前滴针计数初始化 xLastWakeTime 变量。注意，这是唯一一次显式地写入变量。
    在此之后，xLastWakeTime 由 vTaskDelayUntil() API 函数自动管理。 */
    xLastWakeTime = xTaskGetTickCount();

    /* 对于大多数任务，都务是在一个无限循环中实现的。 */
    for( ;; )
    {
        /* 打印出此任务的名称。 */
        ESP_LOGI(TAG,"Periodic task is running");
        /* 任务应该每 3 毫秒执行一次，在这个函数中可以看到 xDelay3ms 的声明。 */
        vTaskDelayUntil( &xLastWakeTime, xDelay3ms);
    }
}

void app_main(void)
{
    xTaskCreate(vContinuousProcessingTask,"task1",5000,pvParameters,1,&task);
    xTaskCreate(vPeriodicTask,"task1",5000,pvParameters,1,NULL);
}