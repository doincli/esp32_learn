# Apptrace

## 功能:

主要功能一共有四个,目前探索了两个

1. 收集来自特定应用程序的数据。具体请参阅 [特定应用程序的跟踪](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/app_trace.html#app-trace-application-specific-tracing)。
2. 记录到主机的轻量级日志。具体请参阅 [记录日志到主机](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/app_trace.html#app-trace-logging-to-host)。
3. 系统行为分析。具体请参阅 [基于 SEGGER SystemView 的系统行为分析](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/app_trace.html#app-trace-system-behaviour-analysis-with-segger-systemview)。(同时连jtag还有uart)
4. 获取源代码覆盖率。具体请参阅 [Gcov（源代码覆盖）](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-guides/app_trace.html#app-trace-gcov-source-code-coverage)。



为了方便没有使用命令行 接下来都是在vscode下面进行的



**官网的解释的作用:**

ESP-IDF 中提供了应用层跟踪功能，用于分析应用程序的行为。这一功能在相应的库中实现，可以通过 menuconfig 开启。**此功能允许用户在程序运行开销很小的前提下，通过 JTAG、UART 或 USB 接口在主机和 ESP32 之间传输任意数据。**用户也可同时使用 JTAG 和 UART 接口。UART 接口主要用于连接 SEGGER SystemView 工具（参见 [SystemView](https://www.segger.com/products/development-tools/systemview/))。





## 功能一（特定应用程序的跟踪）:

主要用于主机和目标机之间传递数据,但是主机接收到的数据需要自己处理,要自己写python,官网说的在~/esp-idf/tools/esp_app_trace里面的apptrace_proc.py` 和 `logtrace_proc.py有示例,我看了一下,不是特别懂。

**我个人理解是和打印差不多,都是用来观察变量的**



#### **问题1:**

**主机接收到的数据如何处理**？我明天具体测试一下



## 功能二（记录日志到主机）:

#### 场景:

记录日志到主机是 ESP-IDF 中一个非常实用的功能：通过应用层跟踪库将日志保存到主机端。某种程度上，这也算是一种半主机 (semihosting) 机制，相较于调用 `ESP_LOGx` 将待打印的字符串发送到 UART 的日志记录方式，此功能将大部分工作转移到了主机端，从而减少了本地工作量。

#### 方法:

以例程blink举例:

1.创建例程文件
![image-20230608174209159](apptrace.assets/image-20230608174209159.png)

2.如果想要使用apptrace功能,需要更改设置,打开menuconfig,进行如下修改

![image-20230608174344901](apptrace.assets/image-20230608174344901.png)

3.添加相应的api函数,将main函数修改如下:

```c
//添加头文件       
#include "esp_app_trace.h"

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    configure_led();
    int i = 1;
    while (1) {
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led();
        esp_log_set_vprintf(esp_apptrace_vprintf);
        ESP_LOGI(TAG,"log data : %d",i);
        esp_apptrace_flush(ESP_APPTRACE_DEST_TRAX, 1000); 
        i++;
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
```

**代码解释:**

具体函数的API链接：https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/app_trace.html

```c
 //将日志数据发送给主机
 esp_log_set_vprintf(esp_apptrace_vprintf);
 //如需将日志数据再次重定向给 UART，请使用esp_log_set_vprintf(vprintf);
 
 //打印需要观测的数据
 ESP_LOGI(TAG,"log data : %d",i);
 
 //将跟踪缓冲区中的剩余数据刷新到主机。
  esp_apptrace_flush(ESP_APPTRACE_DEST_TRAX, 1000); 
  
  esp_err_t esp_apptrace_flush ( esp_apptrace_dest_t dest , uint32_t tmo )
    – dest 指示刷新数据的硬件接口。
        ESP_APPTRACE_DEST_JTAG 或者ESP_APPTRACE_DEST_TRAX  是JTAG 
        ESP_APPTRACE_DEST_UART  是UART
    – tmo 设置超时时间。使用 ESP_APPTRACE_TMO_INFINITE 无限期等待。
```

4.正常编译下载

5.点击apptrace,等待下面出现日志文件

![image-20230608174659218](apptrace.assets/image-20230608174659218.png)

6.打开日志文件,观察检测的内容

![image-20230608174726888](apptrace.assets/image-20230608174726888.png)

这样即可跟踪想要观察的数据以及变量.



#### **问题2：**

和直接打印没有什么区别,**官网给的是效率高,因为通过应用层跟踪库将日志保存到主机端。某种程度上，这也算是一种半主机 (semihosting) 机制，相较于调用 `ESP_LOGx` 将待打印的字符串发送到 UART 的日志记录方式，此功能将大部分工作转移到了主机端，从而减少了本地工作量**。

明天探索一下内在原理。



## 功能三（系统行为分析）：

### 场景：

1. 调试和故障排除：SystemView可以提供实时的系统行为信息，帮助开发人员快速定位和解决嵌入式系统中的问题。通过可视化系统事件和任务活动，可以更容易地发现代码执行的瓶颈、资源竞争、死锁等问题。
2. 系统性能优化：SystemView可以用于分析系统的性能瓶颈，并找到改进的机会。通过查看任务执行时间、中断频率、资源利用率等指标，可以确定需要优化的部分，并采取相应的措施提高系统的响应性能和效率。
3. 电源优化：对于嵌入式系统来说，电源管理是至关重要的。SystemView可以提供有关系统中各个任务和外设的功耗信息，帮助开发人员识别和优化高功耗的部分，以延长系统的电池寿命。
4. 面向实时系统的调度分析：SystemView可以捕获和显示任务调度的信息，包括任务的执行顺序、切换时间等。这有助于分析系统的实时行为，验证调度算法的正确性，以及调整任务的优先级和时间片分配，以满足实时性要求。
5. 系统性能评估：SystemView可以用于对嵌入式系统的整体性能进行评估。通过收集和分析系统的运行数据，可以获取系统的平均响应时间、最大响应时间、任务执行情况等指标，从而评估系统在各种负载情况下的性能表现。



### 使用方法：

这个vscode的图形化界面没有提供这个功能，我明白测试一下命令行怎么处理





## 功能四（获取源代码覆盖率）：

### 场景：

1. 源代码覆盖率是指示程序运行时内每个程序执行路径的计数和频率的数据
2. 源代码覆盖率可以帮助评估测试用例的质量和覆盖程度。通过分析覆盖率报告，开发人员可以了解哪些部分的代码被测试覆盖，哪些部分被忽略，从而评估测试的全面性和有效性。
3. 覆盖率工具可以帮助发现潜在的代码缺陷和逻辑错误。通过分析覆盖率报告，开发人员可以识别出未执行的代码分支、未处理的异常情况等，从而发现潜在的问题，并对代码进行修复和改进。



### 使用方法：

1.新建一个例程

![image-20230608181840646](apptrace.assets/image-20230608181840646.png)

2.开启三个配置

![image-20230608182045675](apptrace.assets/image-20230608182045675.png)

![image-20230608182114387](apptrace.assets/image-20230608182114387.png)

![image-20230608182126849](apptrace.assets/image-20230608182126849.png)

3.正常编译下载

4.打开openocd

5.打开一个新的终端  输入telnet localhost 4444  然后输入esp gcov dump

![image-20230608192027048](apptrace.assets/image-20230608192027048.png)

6.打开命令面板输入**ESP-IDF: Add Editor coverage**

### 问题3：没有显示颜色

我的：

![image-20230608192112917](apptrace.assets/image-20230608192112917.png)

正常的：

![image-20230608192128891](apptrace.assets/image-20230608192128891.png)

7.输入**ESP-IDF: Get HTML Coverage Report** 

此处结果正确，上一处结果错误

![image-20230608192155177](apptrace.assets/image-20230608192155177.png)