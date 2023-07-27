# 项目总结

## 蓝牙服务端

### 初始化流程：

```c
nvs_flash_init();
esp_bt_controller_mem_release();
esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
esp_bt_controller_init();
```



### 注册事件的顺序 GAP

gap_event_handler

```
ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT 当扫描参数设置完成后，事件到来
ESP_GAP_BLE_SCAN_START_COMPLETE_EVT	当开始扫描完成时，事件发生
ESP_GAP_BLE_SCAN_RESULT_EVT 当一个扫描结果准备好时，每次都会发生该事件
当满足想要读取的名字后  停止扫描广播
```



### gattc_event_handler/gatts_event_handler

这个函数的主要作用：导入 GATT 的 profiles。

这个函数注册完成之后，就会在ble 协议任务函数中运行，将程序前面定义的 profiles 导入：

在这两个 profiles 中，每一个都有自己对于的回调函数：gatts_profile_a_event_handler 和 gatts_profile_b_event_handler， 这也是我们本示例的关键函数，下面我们会来分析。




### gattc_profile_event_handler 注册

```
ESP_GATTC_REG_EVT    当GATT客户端注册时，事件发生 初始化之后开始扫描
ESP_GATTC_CONNECT_EVT 当ble物理连接建立后，事件来了
ESP_GATTC_OPEN_EVT  当GATT虚拟连接建立时，事件到来
ESP_GATTC_DIS_SRVC_CMPL_EVT  当 ble discovery 服务完成时，事件到来
	esp_ble_gattc_search_service 调用此函数从本地缓存获取服务。该函数通过回调事件报告服务搜索结果，然后是服务搜索完成事件。 
ESP_GATTC_SEARCH_RES_EVT 	当获得 GATT 服务发现结果时，事件发生  这里记录蓝牙的特征句柄
ESP_GATTC_SEARCH_CMPL_EVT  当GATT服务发现完成后，该事件到来
ESP_GATTC_REG_FOR_NOTIFY_EVT 
ESP_GATTC_CFG_MTU_EVT
ESP_GATTC_WRITE_DESCR_EVT   esp_ble_gattc_write_char 写入特征
ESP_GATTC_WRITE_CHAR_EVT     写入字符
```



- **gattc_if** – **[in]** Gatt 客户端访问接口。
- **conn_id** – **[in]**标识服务器的连接 ID。
- **start_handle** – **[in]**属性启动句柄

主要功能：注册profile





### gatts_profile_a_event_handle

```
在没有连接之前：注册->创建->启动->添加特征->添加特征描述：

ESP_GATTS_REG_EVT—>
ESP_GATTS_CREATE_EVT—>
ESP_GATTS_START_EVT—>
ESP_GATTS_ADD_CHAR_EVT—>
ESP_GATTS_ADD_CHAR_DESCR_EVT

流程说明：

在 Demo 的ESP_GATTS_REG_EVT事件中，调用esp_ble_gap_set_device_name(char *)来设置蓝牙设备名字；调用esp_ble_gap_config_adv_data()来配置广播数据；

最后调用esp_ble_gatts_create_service()指定 gatts_if 和 service_id 来创建服务<实际调用 btc_transfer_context() 来完成服务的创建和调用回调函数>。

服务创建完成就会触发回调函数向profile报告状态和服务ID。Service_id对于后面添加included serivces 和 characteristics 和 descriptor 都要用到。触发ESP_GATTS_CREATE_EVT事件

在Demo的ESP_GATTS_CREATE_EVT中调用esp_ble_gatts_start_service(uint16_t service_handle)来启动服务；

再调用 esp_ble_gatts_add_char() 来添加特性(特征的UUID， 特征值描述符属性权限， 特征属性、特征值、属性响应控制字节)。

触发ESP_GATTS_START_EVT和ESP_GATTS_ADD_CHAR_EVT事件，在ESP_GATTS_ADD_CHAR_EVT事件中，获取特征值调用esp_err_tesp_ble_gatts_add_char_descr()来添加特征描述符。

在连接之后：

CONNECT_EVT—>
ESP_GATTS_MTU_EVT—>
GATT_WRITE_EVT—>
ESP_GATTS_CONF_EVT—>
GATT_READ_EVT
```



















![image-20230727145143306](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230727145143306.png)

![image-20230727153030860](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230727153030860.png)

![image-20230727153109353](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230727153109353.png)

![image-20230727153120885](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230727153120885.png)

![image-20230727155006131](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230727155006131.png)

![image-20230727155101203](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230727155101203.png)