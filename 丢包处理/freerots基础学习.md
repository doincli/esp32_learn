# freerots 基础知识

## 多线程

### **xTaskCreate()**：

```c
BaseType_t xTaskCreate(TaskFunction_t pxTaskCode, const char* pcName, uint16_t usStackDepth, void* pvParameters, UBaseType_t uxPriority, TaskHandle_t* pxCreatedTask);
```

该函数用于创建一个新的任务。

参数 `pxTaskCode` 是任务函数的指针，

参数 `pcName` 是任务名称，

参数 `usStackDepth` 是任务堆栈大小，

参数 `pvParameters` 是传递给任务函数的参数，

参数 `uxPriority` 是任务优先级，

参数 `pxCreatedTask` 是任务句柄的指针（可选）。

函数返回 `pdPASS` 表示任务创建成功。



### **vTaskDelete()**：

```c
void vTaskDelete(TaskHandle_t xTask);

vTaskDelete(NULL);  //删除当前任务：
vTaskDelete(xTask);//删除其他任务
```

该函数用于删除一个任务。参数 `xTask` 是要删除的任务的句柄。被删除的任务将被终止并回收相关资源





### **vTaskDelay()**：

```c
void vTaskDelay(TickType_t xTicksToDelay);
```

该函数用于使当前任务延迟一定的时间。参数 `xTicksToDelay` 是延迟的时间，以 FreeRTOS 的时钟节拍为单位。



### **vTaskSuspend()**：

```c
void vTaskSuspend(TaskHandle_t xTaskToSuspend);
```

该函数用于暂停（挂起）一个任务。参数 `xTaskToSuspend` 是要挂起的任务的句柄。被挂起的任务将停止执行，直到被恢复。



### **vTaskResume()**：

```c
void vTaskResume(TaskHandle_t xTaskToResume);
```

该函数用于恢复（继续执行）一个被挂起的任务。参数 `xTaskToResume` 是要恢复的任务的句柄。





## 互斥量

在 FreeRTOS 中，互斥量是通过互斥量控制块（Mutex Control Block）来实现的，提供了一系列函数用于创建、获取、释放和删除互斥量。



### **xSemaphoreCreateMutex()**：

```c
SemaphoreHandle_t xSemaphoreCreateMutex(void);
```

该函数用于创建一个互斥量。返回值是互斥量的句柄（handle）。如果创建成功，返回非空句柄；否则返回空句柄。



### **xSemaphoreTake()**：

```c
BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait);
```

该函数用于获取（占用）互斥量。参数 `xSemaphore` 是互斥量的句柄，参数 `xTicksToWait` 是获取互斥量的超时时间。函数返回 `pdPASS` 表示成功获取互斥量，返回 `pdFAIL` 表示超时或获取失败。



### **xSemaphoreGive()**：

```c
BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore);
```

该函数用于释放（归还）互斥量。参数 `xSemaphore` 是互斥量的句柄。函数返回 `pdPASS` 表示成功释放互斥量，返回 `pdFAIL` 表示失败。



### **vSemaphoreDelete()**：

```c
void vSemaphoreDelete(SemaphoreHandle_t xSemaphore);
```

该函数用于删除互斥量。参数 `xSemaphore` 是互斥量的句柄。被删除的互斥量将被释放，不再可用。





## 消息队列

### **xQueueCreate()**：

```c
QueueHandle_t xQueueCreate(UBaseType_t uxQueueLength, UBaseType_t uxItemSize);
```

该函数用于创建一个消息队列，参数 `uxQueueLength` 表示队列中可以容纳的元素数量，参数 `uxItemSize` 表示队列中每个元素的大小（以字节为单位）。函数返回一个指向新创建队列的句柄，如果创建失败则返回 `NULL`。



### **vQueueDelete()**：

```c
void vQueueDelete(QueueHandle_t xQueue);
```

该函数用于删除一个已创建的消息队列，参数 `xQueue` 是要删除的队列的句柄。



### **xQueueSend()**：

```c
BaseType_t xQueueSend(QueueHandle_t xQueue, const void *pvItemToQueue, TickType_t xTicksToWait);
```

该函数用于向队列发送数据，参数 `xQueue` 是目标队列的句柄，参数 `pvItemToQueue` 是指向要发送数据的指针，参数 `xTicksToWait` 是等待超时时间。

函数返回值：

返回 `pdPASS` 表示数据成功发送，

返回 `errQUEUE_FULL` 表示队列已满，

返回 `errQUEUE_YIELD` 表示发送数据后触发了任务切换。



### **xQueueReceive()**：

```c
BaseType_t xQueueReceive(QueueHandle_t xQueue, void *pvBuffer, TickType_t xTicksToWait);
```

该函数用于从队列接收数据，参数 `xQueue` 是目标队列的句柄，参数 `pvBuffer` 是用于存储接收数据的缓冲区指针，参数 `xTicksToWait` 是等待超时时间。

函数返回 `pdPASS` 表示成功接收到数据，

返回 `errQUEUE_EMPTY` 表示队列为空，

返回 `errQUEUE_YIELD` 表示接收数据后触发了任务切换。





### **uxQueueMessagesWaiting()**：

```c
UBaseType_t uxQueueMessagesWaiting(const QueueHandle_t xQueue);

```

该函数用于获取队列中当前等待处理的消息数量，参数 `xQueue` 是目标队列的句柄。

函数返回队列中等待处理的消息数量。



### **uxQueueSpacesAvailable()**：

```c
UBaseType_t uxQueueSpacesAvailable(const QueueHandle_t xQueue);
```

该函数用于获取队列中当前可用的空闲空间数量，参数 `xQueue` 是目标队列的句柄。函数返回队列中可用的空闲空间数量。



### **xQueueReset()；**

```c
BaseType_t xQueueReset(QueueHandle_t xQueue);
```

该函数用于重置队列，将队列中的数据清空。参数 `xQueue` 是目标队列

- 如果成功重置了队列，返回 `pdPASS`。这表示队列已被成功清空，并且现在可以重新使用。
- 如果队列句柄无效或函数调用失败，返回 `errQUEUE_HANDLE`。这表示队列句柄无效，或者队列不存在。
- 如果函数调用被中断并触发了任务切换，返回 `errQUEUE_YIELD`。这表示在重置队列后，发生了任务切换。





## 信号量

### **xSemaphoreCreateBinary()**：

```c
SemaphoreHandle_t xSemaphoreCreateBinary(void);
```

该函数用于创建一个二值信号量。返回值是一个 `SemaphoreHandle_t` 类型的句柄，用于引用新创建的信号量。



### **xSemaphoreCreateCounting()**：

```c
SemaphoreHandle_t xSemaphoreCreateCounting(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount);
```

该函数用于创建一个计数信号量。参数 `uxMaxCount` 表示计数信号量的最大计数值，参数 `uxInitialCount` 表示计数信号量的初始计数值。返回值是一个 `SemaphoreHandle_t` 类型的句柄。



### **vSemaphoreDelete()**：

```c
void vSemaphoreDelete(SemaphoreHandle_t xSemaphore);
```

该函数用于删除一个已创建的信号量，释放相应的资源。参数 `xSemaphore` 是要删除的信号量的句柄。



### **xSemaphoreTake()**：

```c
BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait);
```

该函数用于获取（获取）一个信号量。参数 `xSemaphore` 是目标信号量的句柄，参数 `xTicksToWait` 是等待超时时间。如果信号量可用，则函数立即返回；如果信号量不可用，则会等待指定的超时时间。

函数返回 `pdPASS` 表示成功获取到信号量，返回 `errQUEUE_EMPTY` 表示等待超时。



### **xSemaphoreGive()**：

```c
BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore);
```

该函数用于释放（放回）一个信号量。参数 `xSemaphore` 是要释放的信号量的句柄。函数返回 `pdPASS` 表示成功释放信号量。