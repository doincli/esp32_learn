# freertos

## 堆内存管理

五种分配方式

### Heap_1  不允许free

`heap_1.c` 实现了 `pvPortMalloc()` 的一个非常基本的版本，并没有实现 `vPortFree()`。 永远不会删除任务或其他内核对象的应用程序可能会使用 `heap_1`。

[![图5. 每次创建任务时从 heap_1 阵列分配的 RAM](https://github.com/doincli/Mastering-the-FreeRTOS-Real-Time-Kernel-CN/raw/master/.gitbook/assets/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20190904140941.png)](https://github.com/doincli/Mastering-the-FreeRTOS-Real-Time-Kernel-CN/blob/master/.gitbook/assets/微信截图_20190904140941.png)



### Heap_2 不能合并

`heap_2.c` 也可以通过细分由 `configTOTAL_HEAP_SIZE` 标注的数组来工作。 它使用最合适的算法来分配内存，与 `heap_1` 不同，它允许释放内存。 同样，数组是静态声明的，因此即使在分配了阵列中的任何内存之前，也会使应用程序看起来消耗大量 RAM。

最合适的算法确保 `pvPortMalloc()` 使用与所请求的字节数最接近的空闲内存块。 例如，考虑场景：

- 堆包含三个可用内存块，分别为 5 个字节，25 个字节和 100 个字节。
- 调用 `pvPortMalloc()` 来请求 20 个字节的 RAM。

所请求的字节数适合的最小空闲 RAM 块是 25 字节块，因此 `pvPortMalloc()` 将 25 字节块拆分为一个 20 字节的块和一个 5 字节的块，然后返回指针 20 字节的块。 新的 5 字节块仍可用于将来调用 `pvPortMalloc()`。

[![图 6. 在创建和删除任务时，正在分配和释放来自 heap_2 数组的 RAM](https://github.com/doincli/Mastering-the-FreeRTOS-Real-Time-Kernel-CN/raw/master/.gitbook/assets/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20190904142156.png)](https://github.com/doincli/Mastering-the-FreeRTOS-Real-Time-Kernel-CN/blob/master/.gitbook/assets/微信截图_20190904142156.png)



### Heap_3

`heap_3.c` 使用标准库 `malloc()` 和 `free()` 函数



### Heap_4 可以合并

`heap_4` 使用首次拟合算法来分配内存。 与 `heap_2` 不同，`heap_4` 将相邻的空闲内存块（合并）组合成一个更大的块，从而最大限度地降低了内存碎片的风险。

第一适合算法确保 `pvPortMalloc()`使用足够大的第一个空闲内存块来保存请求的字节数。 例如，考虑以下情况：

- 堆包含三个可用内存块，它们按照它们在数组中出现的顺序分别为 5 个字节，200 个字节和 100 个字节。
- 调用 `pvPortMalloc()` 来请求 20 个字节的 RAM。

请求的字节数适合的第一个RAM块是 200 字节块，因此 `pvPortMalloc()` 将 200 字节块拆分为一个 20 字节的块，以及一个 180 字节的块，然后返回指向 20 的字节块。 新的 180 字节块仍可用于将来调用 `pvPortMalloc()`。

`heap_4` 将相邻空闲块的（合并）组合成一个更大的块，最大限度地降低了碎片的风险，并使其适用于重复分配和释放不同大小的 RAM 块的应用程序。

[![图 7. 正在分配并从 heap_4 数组中释放的 RAM](https://github.com/doincli/Mastering-the-FreeRTOS-Real-Time-Kernel-CN/raw/master/.gitbook/assets/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20190904144813.png)](https://github.com/doincli/Mastering-the-FreeRTOS-Real-Time-Kernel-CN/blob/master/.gitbook/assets/微信截图_20190904144813.png)



### Heap_5

`heap_5` 用于分配和释放内存的算法与 `heap_4` 使用的算法相同。 与 `heap_4` 不同的是，`heap_5` 不限于从单个静态声明的数组中分配内存; `heap_5` 可以从多个独立的内存空间分配内存。 当运行 FreeRTOS 的系统提供的 RAM 在系统的内存映射中不显示为单个连续（无空间）块时，`heap_5` 非常有用。



### 其他的API

#### xPortGetFreeHeapSize() API函数

```c
size_t xPortGetFreeHeapSize( void );
xPortGetFreeHeapSize() API 函数在调用函数时返回堆中的空闲字节数。它可用于优化堆大小。 例如，如果 xPortGetFreeHeapSize() 在创建了所有内核对象后返回 2000，那么 configTOTAL_HEAP_SIZE 的值可以减少 2000。
```

#### `xPortGetMinimumEverFreeHeapSize()`API 函数

```c
xPortGetMinimumEverFreeHeapSize()
返回自 FreeRTOS 应用程序开始执行以来堆中已存在的最小未分配字节数。
```

####  uxTaskGetStackHighWaterMark(task)函数

```c
UBaseType_t  aa = uxTaskGetStackHighWaterMark(task);
返回用了多少内存
```



## 任务管理

### 任务创建

```c
BaseType_t xTaskCreate( TaskFunction_t pvTaskCode, 
                        const char* const pcName, 
                        uint16_t usStackDepth, 
                        void *pvParameters, 
                        UBaseType_t uxPriority, 
                        TaskHandle_t *pxCreatedTask );
```



### API

#### `vTaskDelay()` 的函数

| 参数名称        | 描述                                                         |
| --------------- | ------------------------------------------------------------ |
| `xTicksToDelay` | 调用任务在转换回就绪状态之前将保持在阻塞状态的滴答中断数。例如，如果一个名为 `vTaskDelay(100)` 的任务在滴答计数为 `10,000` 时立即进入阻塞状态，并保持阻塞状态，直到滴答计数达到 `10,100`。可以使用宏 `pdMS_TO_TICKS()` 将以毫秒为单位的时间转换为以 int 为单位的时间。例如，调用 `vTaskDelay(pdMS_TO_TICKS(100))` 将导致调用任务保持阻塞状态 100 毫秒。 |

#### `vTaskDelayUntil()` API 函数

| 参数名称           | 描述                                                         |
| ------------------ | ------------------------------------------------------------ |
| pxPreviousWakeTime | 此参数的命名是基于 `vTaskDelayUntil()` 用于实现定期执行且具有固定频率的任务。 在这种情况下，`pxPreviousWakeTime` 保持任务最后一次离开阻塞状态的时间（被 '唤醒'）。 此时间用作参考点，用于计算任务下次离开阻塞状态的时间。`pxPreviousWakeTime` 指向的变量在 `vTaskDelayUntil()`函数中自动更新；它通常不会被应用程序代码修改，但必须在第一次使用之前初始化为当前的滴答计数。 清单 25 演示了如何执行初始化。 |
| xTimeIncrement     | 此参数的命名也是基于 `vTaskDelayUntil()` 用于实现定期执行且具有固定频率的任务，频率由 `xTimeIncrement` 值设置。`xTimeIncrement` 在 'ticks' 中指定。 `pdMS_TO_TICKS()`宏可用于将毫秒指定的时间转换为刻度中指定的时间。 |

==区别：==

- `vTaskDelayUntil()`：主要用于实现周期性任务。你可以指定任务在上次唤醒时间的基础上，以固定的时间间隔运行。这在周期性任务或实时任务中非常有用。
- `vTaskDelay()`：主要用于实现非周期性的任务延迟。当任务执行完毕后，你可以使用该函数使任务进入阻塞状态，并延迟一段时间再继续执行。

​				**多了一个唤醒时间，可以控制的更加精确**





#### vTaskDelete() API 函数

| 参数名称/返回值  | 描述                                                         |
| ---------------- | ------------------------------------------------------------ |
| `pxTaskToDelete` | 要删除的任务的句柄（主题任务）—— 请参阅 `xTaskCreate()`API 函数的`pxCreatedTask` 参数，以获取有关获取任务句柄的信息。任务可以通过在有效的任务句柄处传递 `NULL` 来删除自己。 |

### 空闲函数

#### void vApplicationIdleHook( void );

1. 一个空闲的任务钩子函数决不能试图阻塞或挂起。
2. 如果应用程序使用了 `vTaskDelete()` API函数，那么空闲任务钩子必须总是在合理的时间段内返回给调用者。这是因为空闲任务负责在删除任务之后清理内核资源。如果空闲任务永久地保留在空闲钩子函数中，则无法进行清理。



他是抢占式的操作系统，与正常Linux时间片不太一致。





## 队列管理

“队列” 提供了一个**任务到任务**，**任务到中断**和**中断到任务**的通讯机制。