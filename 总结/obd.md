# OBD检测总结

## 初始化

首先初始化GPIO，也就是TWAI总线的发送和接收端，TWAI总线控制 CAN总线



然后配置好CAN总线的速率以及相关配置，通过枚举类型来实现

```c
typedef enum 
{
    ISO15765_11bit_500K=0,       /*!< Show current search mode is ISO15765_11bit,speed is 500KB */
    ISO15765_11bit_250K,         /*!< Show current search mode is ISO15765_11bit,speed is 250KB */
    ISO15765_29bit_500K,         /*!< Show current search mode is ISO15765_29bit,speed is 500KB */
    ISO15765_29bit_250K          /*!< Show current search mode is ISO15765_29bit,speed is 250KB */
} protocol;

typedef struct 
{
  uint8_t  tx_port;            /*!< IO port */
  uint8_t  rx_port;
} obd_io;

typedef obd_io * obd_io_handle;
typedef struct 
{
  uint8_t    protocol_t;          /*!< protocol can be defined as  ISO15765_11bit and ISO15765_29bit */
  uint8_t    statu ;              /*!< current search mode */
  uint8_t    speed ;              /*!< speed can be defined as  BPS_500K and BPS_250K */
  obd_io_handle io_port;
  twai_general_config_t * g_config;
  twai_timing_config_t *  t_config;
} detect_config_t;

```

通过状态机来实现对不同协议的解析

循环更改配置以及发送的命令，当返回得到正确的数据时候，认为解析到正确的协议

```c
esp_err_t obd_detect_match(obd_protocol_handle obd_handle)
{
    esp_err_t  right_protocol = obd_detect(obd_handle);
    if (right_protocol == ESP_FAIL){
        obd_twai_deinit();
        while (1){   
            uint8_t protocol_cur = obd_handle->statu;
            //State machine  polling detection
            switch (protocol_cur){
            case ISO15765_11bit_500K:
                ESP_LOGI(TAG,"ISO15765_11bit_500K start\n");
                obd_twai_modifed(obd_handle);
                //Obtain the vehicle speed, if the acquisition fails, enter the next protocol to continue detection
                right_protocol = obd_detect(obd_handle);
                if (right_protocol == ESP_FAIL) {
                    obd_handle->statu = ISO15765_11bit_250K;
                    obd_handle->protocol_t = ISO15765_11bit;
                    obd_handle->speed = BPS_250K;
                    ESP_LOGI(TAG,"next protocol is ISO15765_11bit_250K\n");
                    vTaskDelay(100);
                    obd_twai_deinit();
                    // 
                }else{
                    ESP_LOGI(TAG,"right protocol is ISO15765_11bit_500K\n");
                    return ESP_OK;
                }
                
            break;

            case ISO15765_11bit_250K:
                ESP_LOGI(TAG,"ISO15765_11bit_250K start\n");
                
                obd_twai_modifed(obd_handle);
                right_protocol = obd_detect(obd_handle);
                if (right_protocol == ESP_FAIL){
                    obd_handle->statu = ISO15765_29bit_500K;
                    obd_handle->protocol_t = ISO15765_29bit;
                    obd_handle->speed = BPS_500K;
                    ESP_LOGI(TAG,"next protocol is ISO15765_29bit_500K\n");
                    obd_twai_deinit();
                }else{
                    ESP_LOGI(TAG,"right protocol is ISO15765_11bit_250K\n");
                    return ESP_OK;
                }
                
            break;

            case ISO15765_29bit_500K:
                ESP_LOGI(TAG,"ISO15765_29bit_500K start\n");
                obd_twai_modifed(obd_handle);
                right_protocol = obd_detect(obd_handle);
                if (right_protocol == ESP_FAIL){
                    obd_handle->statu = ISO15765_29bit_250K;
                    obd_handle->protocol_t = ISO15765_29bit;
                    obd_handle->speed = BPS_250K;
                    ESP_LOGI(TAG,"next protocol is ISO15765_29bit_250K\n");
                    obd_twai_deinit();
                }else{
                    ESP_LOGI(TAG,"right protocol is ISO15765_29bit_500K\n");
                    return ESP_OK;
                }
                
            break;

            case ISO15765_29bit_250K:
                ESP_LOGI(TAG,"ISO15765_29bit_250K start\n");
                obd_twai_modifed(obd_handle);
                right_protocol = obd_detect(obd_handle);
                if (right_protocol == ESP_FAIL) {
                    obd_handle->statu = ISO15765_11bit_500K;
                    obd_handle->protocol_t = ISO15765_11bit;
                    obd_handle->speed = BPS_500K;
                    ESP_LOGI(TAG,"next protocol is ISO15765_11bit_500K\n");
                    obd_twai_deinit();
                }else{
                    ESP_LOGI(TAG,"right protocol is ISO15765_29bit_250K\n");
                    return ESP_OK;
                }
            break;

            default:
                ESP_LOGI(TAG,"event error\n");
                break;
            }
        }   
    }
    return ESP_OK;
}
```





## OBD协议详解

外部终端发送、和行车电脑返回数据的通用格式如下： 

```c
/*
终端发送： CANID 为：7DF 			报文数据为：02 01 PIDx 00 00 00 00 00 
ECU 回复： CANID 为：7E8 		 报文数据为：07 41 PIDx A  B  C D 00 
```

来直接的，如果想要从 OBD 口获取到发动机转速如何发送和接受解析命令？ 

```c
发送 SID:0x07DF 
数据字节(HEX):02 01 0C 00 00 00 00 00 
接收 SID:0x07E8 
数据字节(HEX):04 41 0C 0B 3E 00 00 00 
```

以上发送命令解析为：7DF，为 CANID，这个 CANID 是所有具有诊断功能的行车电脑都会相 应的一个 CANID。发送的数据中，02 代表数据长度，后面有效字节长度为 2，即 01 0C。01 代表服务号，也叫 SID，其中 01 是动力有关的数据，01 服务为用的最多的一个服务。0C 代表参数 ID，也叫 PID，这个 PID 具体定义在 IS015031-5 里面，此处 0C 这个是发动机转速的 PID。总结起来这条请求报文就是：外部设备要向行车电脑请求发动机转速！ 

接收到的数据为行车电脑返回的数据，其中 7E8 为行车电脑的物理 ID，也是 CANID。 

CAN 报文数据为：04，代表后续有效字节有 4 个。41 为对 01 服务的一个应答，所有行车电 脑返回的数据，都会在请求数据的基础上加 0x40 返回（0X01+0X40 即 0x41）。0C 对应请求 命令中的 0C，代表发动机转速。0B 3E 代表转速值，为十六进制的转换为十进制为： 0x0B3E=2878,但是这个里面还有一个解析算法，这个值除以 4 为实际转速，也就是实际转速 

为 2878/4=719 转。

![image-20230727174818860](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230727174818860.png)



总结：

帧协议的形式

CANID

```c
\#define MSG_ID 0x7DF            /*!< MSG_ID the head of message of protocol   */

\#define MSG_ID_EXP 0x18DB33F1 
```

发送：发送的CANID  +  字节数+服务号+PID

接收：响应的CANID + 字节数 + 响应号 +  PID + 数据



接收的数据都是单字节的，如果数据超过一个字节需要使用相关算法  比如（256*A+B)/4等