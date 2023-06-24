

# mesh组网

## 基础概念介绍

蓝牙 Mesh 是一种基于蓝牙技术的无线网络拓扑结构，它允许多个蓝牙设备相互连接并组成一个自组织的网络。与传统的蓝牙点对点连接方式不同，蓝牙 Mesh 可以支持大规模设备互联，并提供灵活的通信和控制能力。

### 主流的几种组网方式

![image-20230612173556667](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624222907.png)

从上图可以看出，**蓝牙 Mesh 可以支持大规模设备互联，单网接入数量理论无限大**，并且由于蓝牙mesh是基于BLE（低功耗蓝牙），电池续航能力强，由于蓝牙模块的广泛应用，蓝牙拥有易于使用的配置和管理工具，所以**蓝牙mesh相对于其它组网方式具有低功耗、可扩展能力强，容易使用等优点**



功耗和mesh组网的矛盾：蓝牙 Mesh 组网为了提高传输距离以及大规模设备互联需要设备保持**通信并参与网络中的数据中继**，这可能会导致设备持续地进行通信活动，增加功耗并缩短电池寿命，所以设计mesh网络的时候需要考虑一定的设计方式比如优化网络拓扑，开启睡眠模式等等



#### 蓝牙Mesh相对于wifi组网的区别

1. 应用场景：**蓝牙 Mesh 组网适用于相对小范围的设备互联**，例如智能家居、商业建筑、工业自动化等。而 **Wi-Fi 更适用于提供大范围的互联网接入**，例如家庭、办公室、公共场所等需要高速互联网连接的场景。
2. 网络拓扑：蓝**牙 Mesh 组网采用网状拓扑结构，设备通过多跳通信实现互联，没有单点故障**。而 **Wi-Fi 使用基础设施模式，其中一个路由器充当中心控制节点，并通过单跳通信连接设备**。
3. 省电性：**蓝牙 Mesh 组网在设计上考虑了低功耗**，适合运行在电池供电的设备上，可以实现较长的电池寿命。**Wi-Fi 相对较耗电，适用于接入电源的设备**。
4. 设备数量：**蓝牙 Mesh 组网支持连接大量设备**，可以扩展到成千上万个节点。**Wi-Fi 网络通常支持更少数量的设备连接，通常不会超过50个**。
5. 配置和管理：蓝牙 Mesh 组网具有自组织和自修复的能力，设备可以自动加入网络并创建通信路径。而 Wi-Fi 网络通常需要进行手动配置和管理。
6. 安全性：蓝牙 Mesh 组网提供了多层安全性，包括消息加密、身份认证和访问控制。Wi-Fi 也提供加密和身份验证机制，但**蓝牙 Mesh 的安全性设计更加专注于物联网设备间的通信安全性**。



### 蓝牙Mesh 组网应用场景

1. 智能家居：蓝牙 Mesh 组网可用于构建智能家居系统，使各种设备（如照明、温控、安全系统等）能够互相通信和协同工作。用户可以通过智能手机或语音助手来控制和监测家居设备，提供更便捷、舒适和节能的家居体验。
2. 商业和办公空间：蓝牙 Mesh 组网可用于商业建筑、办公室和零售空间等环境中。它可以实现智能照明、能源管理、设备监测和人员定位等功能，提高能效、安全性和舒适度，并为企业提供更好的管理和控制能力。
3. 工业自动化：蓝牙 Mesh 组网在工业自动化领域具有潜力。它可以用于设备间的数据传输和控制，监测和管理工厂设备、传感器和执行器。蓝牙 Mesh 可以简化设备的布线和部署，并提供实时的设备状态和运行数据。
4. 智能城市：蓝牙 Mesh 组网可在智能城市中发挥作用。它可以用于照明系统、停车管理、环境监测、智能交通等领域。通过蓝牙 Mesh 组网，城市可以实现能源效率、交通流量优化、环境监测和城市安全等方面的改进。
5. 健康和医疗：蓝牙 Mesh 组网可以应用于医疗和健康监测领域。它可以连接和管理医疗设备、传感器和监测设备，实现远程监测、病人定位、设备管理等功能。蓝牙 Mesh 可以提供医疗设备之间的实时数据传输和通信，促进医疗服务的改进和效率提高。



### 蓝牙mesh的基本结构

蓝牙mesh包含以下几个基本结构

1. 节点（Nodes）：节点是蓝牙 Mesh 网络中的基本单元，可以是传感器、执行器、网关或中继器等设备。每个节点都具有唯一的 128 位地址（UUID），用于标识自身并与其他节点进行通信。
2. 模型（Models）：模型定义了设备的功能和行为，并提供了设备之间通信的一致方式。每个节点可以支持一个或多个模型，模型定义了设备的属性、状态和操作。例如，一个灯具节点可以支持灯光模型，其中包含控制灯光亮度、颜色等功能。
3. 元素（Elements）：节点由一个或多个元素组成，每个元素表示设备的一个逻辑部分，可以理解为设备的子模块。每个元素可以支持一个或多个模型，并与其他节点的元素进行通信。例如，一个具有多个灯泡的灯具节点可以将每个灯泡作为一个元素。
4. 发布和订阅（Publish and Subscribe）：蓝牙 Mesh 使用发布和订阅模式进行设备之间的通信。节点可以发布消息，并将其发送到一个或多个订阅该消息的节点。这种发布-订阅模式允许设备之间进行广播式的通信，实现多对多的数据传输。
5. 网络拓扑结构：蓝牙 Mesh 网络采用多跳的网络拓扑结构，其中每个节点可以与其他节点直接通信，而不依赖于中心控制器。节点之间的数据传输可以通过其他节点进行中继，从而扩展网络的范围和覆盖。其中友节点可以中继低功耗的蓝牙终端扩大传输距离



# 蓝牙mesh配网方法（例程分析）

本文档基于esp32芯片进行开发，配网的例程使用官网提供的的ble_mesh_provisioner。前面主要对示例的整体流程进行总结性质说明，后对示例代码进行具体说明

## 程序流程解析

### 函数逻辑

#### 主函数的逻辑：

1. 开启存储器
2. 蓝牙初始化
3. 获得设备的uuid 
4. ble_mesh_init



#### ble_mesh_init的逻辑：

1. 注册三个系统的回调函数，三个函数如下所示，这三个回调函数会根据蓝牙协议栈中的时间不定时触发，然后三个函数都是利用有限状态机来实现相关的处理逻辑，每一个回调函数的事件以及操作码在后文介绍，然后调用四个系统的API

   ```
   esp_ble_mesh_register_prov_callback(example_ble_mesh_provisioning_cb);
   配网的回调函数
   
   esp_ble_mesh_register_config_client_callback(example_ble_mesh_config_client_cb);
   客户端模型的回调函数   
   
   esp_ble_mesh_register_generic_client_callback(example_ble_mesh_generic_client_cb);
   通用模型的回调函数
   ```

2. 第１个API 　esp_ble_mesh_init  初始化BLE Mesh模块

3. 第２个API　 esp_ble_mesh_provisioner_set_dev_uuid_match　设置UUID

4. 第３个API　esp_ble_mesh_provisioner_prov_enable　开启配网功能

5. 第４个API　esp_ble_mesh_provisioner_add_local_app_key　添加本地APPkey



#### 回调函数的逻辑：　事件较多－＞不画状态机的逻辑转换图

**esp_ble_mesh_register_prov_callback的处理逻辑（只写重点）**

1. 处于下面事件的处理逻辑

   ```
   ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT
   收到未配置的设备信标事件
   
   调用recv_unprov_adv_pkt函数将未配对的设备添加到 Provisioner 的设备队列中
   ```

   

2. 处于下面事件的处理逻辑

   ```
   ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT
   Provisioner 配置完成事件
   
   　调用prov_complete函数将信息存储在nvs中
   ```

   

3. 处于下面事件的处理逻辑

   ```
   ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT
   设置节点名称完成事件
   
   调用系统API esp_ble_mesh_provisioner_get_node_name函数获取节点的相关信息
   
   ```

   　　

4. 处于下面事件的处理逻辑

   ```
   ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT
   添加本地应用程序密钥完成事件
   
   　调用系统API esp_ble_mesh_provisioner_bind_app_key_to_local_model函数将自己的模型与程序密钥绑定
   ```

   



**example_ble_mesh_config_client_cb处理逻辑**（只写重点）

1. 处于该事件时候

   ```
   ESP_BLE_MESH_CFG_CLIENT_GET_STATE_EVT
   ```

   　查看操作码　操作码为

   ```
   ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET
   
   调用example_ble_mesh_set_msg_common　　获取节点和模型基本信息　以及两个密钥
   并调用esp_ble_mesh_config_client_set_state　API函数设置服务器状态
   ```

2. 处于该事件时候

   ```
   ESP_BLE_MESH_CFG_CLIENT_SET_STATE_EVT
   ```

   查看操作码　操作码为

   ```
   ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD
   
   ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET
   调用example_ble_mesh_set_msg_common　　获取节点和模型基本信息以及单播地址等信息
   并调用esp_ble_mesh_config_client_set_state　API函数设置服务器状态
   ```

   操作码为

   ```
   ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND
   
   调用example_ble_mesh_set_msg_common　　获取节点和模型基本信息
   并调用esp_ble_mesh_config_client_set_state　API函数获取服务器状态
   
   ```

   

3. 然后就是处理超时事件的逻辑　　同上



**example_ble_mesh_generic_client_cb处理逻辑**（只写重点）

1. 处于该事件时候

   ```
   ESP_BLE_MESH_GENERIC_CLIENT_GET_STATE_EVT
   ```

   　查看操作码　操作码为

   ```
   ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET
   
   调用example_ble_mesh_set_msg_common　　获取节点和模型基本信息　以及模型开关状态
   并调用esp_ble_mesh_config_client_set_state　API函数设置服务器状态
   ```

   

2. 然后就是处理超时事件的逻辑　　同上



## 每个函数的具体说明

### 头文件

引入的相关头文件以及作用

```c
/*
esp_ble_mesh_common_api.h: 
定义了蓝牙网状网络的一般性功能和操作，例如初始化、启动、停止、发送消息等。

esp_ble_mesh_provisioning_api.h: 
提供了与蓝牙网状网络的设备入网和配置相关的函数和结构体，包括设备的配网和认证。

esp_ble_mesh_networking_api.h: 
定义了与蓝牙网状网络的网络层通信相关的函数和结构体，包括消息的传输和处理。

esp_ble_mesh_config_model_api.h: 
包含了与配置模型相关的函数和结构体，用于配置设备和网络的特定参数。

esp_ble_mesh_generic_model_api.h: 
包含了与通用模型相关的函数和结构体，用于实现蓝牙网状网络中的通用功能，如开关、调光等。


ble_mesh_example_init.h: 
包含了蓝牙网状网络示例的初始化函数和配置结构体等。该头文件定义了初始化蓝牙网状网络示例所需的函数和结构体。

#include "nvs_flash.h"
提供了与非易失性存储（NVS）相关的函数和结构体，用于在示例中保存和读取配置数据。NVS是一种用于存储持久化数据的轻量级存储解决方案。

esp_ble_mesh_defs.h
定义了用于蓝牙 Mesh 的数据结构，如节点、模型、元素、消息等。
*/
```



### 定义部分

１．定义节点信息

```
参数 	CONFIG_BLE_MESH_MAX_PROV_NODES　= 10;
```

​	所以最多可以定义并且初始化10个节点信息，可以对这十个节点进行配网

```c
typedef struct {
    uint8_t  uuid[16];　　//uuid
    uint16_t unicast;		//单播地址
    uint8_t  elem_num;		//元素数量
    uint8_t  onoff;			//服务端开关状态
} esp_ble_mesh_node_info_t;


//初始化相关节点　
static esp_ble_mesh_node_info_t nodes[CONFIG_BLE_MESH_MAX_PROV_NODES] = {
    [0 ... (CONFIG_BLE_MESH_MAX_PROV_NODES - 1)] = {
        .unicast = ESP_BLE_MESH_ADDR_UNASSIGNED,
        .elem_num = 0,
        .onoff = LED_OFF,
    }
};
```



2.定义密钥

定义相关密钥，在配置网络的时候进行交换

```c
static struct esp_ble_mesh_key {
    uint16_t net_idx;		//网络密钥地址
    uint16_t app_idx;		//应用密钥地址
    uint8_t  app_key[16];	//应用密钥
} prov_key;
```



３．配置客户端和服务端的模型

配置相关模型，该模型的模式为不开启中继，开启广播信标，开启友节点（存储低功耗节点的信息），并且开启GATT服务进行信息交互。

```c
static esp_ble_mesh_client_t config_client;
static esp_ble_mesh_client_t onoff_client;

static esp_ble_mesh_cfg_srv_t config_server = {
    .relay = ESP_BLE_MESH_RELAY_DISABLED,	//中继
    .beacon = ESP_BLE_MESH_BEACON_ENABLED,　//信标广播
#if defined(CONFIG_BLE_MESH_FRIEND)
    .friend_state = ESP_BLE_MESH_FRIEND_ENABLED,　	//友节点
#else
    .friend_state = ESP_BLE_MESH_FRIEND_NOT_SUPPORTED,
#endif
#if defined(CONFIG_BLE_MESH_GATT_PROXY_SERVER)
    .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_ENABLED,		//GATT
#else
    .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_NOT_SUPPORTED,
#endif
    .default_ttl = 7,							
    /* 3 transmissions with 20ms interval */
    .net_transmit = ESP_BLE_MESH_TRANSMIT(2, 20),
    .relay_retransmit = ESP_BLE_MESH_TRANSMIT(2, 20),
};

```



4.创建模型

讲配置的模型注册进mesh协议栈

```c
static esp_ble_mesh_model_t root_models[] = {
    ESP_BLE_MESH_MODEL_CFG_SRV(&config_server),
    ESP_BLE_MESH_MODEL_CFG_CLI(&config_client),
    ESP_BLE_MESH_MODEL_GEN_ONOFF_CLI(NULL, &onoff_client),
};

static esp_ble_mesh_elem_t elements[] = {
    ESP_BLE_MESH_ELEMENT(0, root_models, ESP_BLE_MESH_MODEL_NONE),
};
```



5.相关配置的结构体

定义配置mesh网络的相关信息，包括uuid,单播地址、以及元素等信息

```c
//定义了表示设备组件的相关信息
static esp_ble_mesh_comp_t composition = {
    .cid = CID_ESP,
    .elements = elements,
    .element_count = ARRAY_SIZE(elements),
};

//设备的 Provisioning（配置）信息
static esp_ble_mesh_prov_t provision = {
    .prov_uuid           = dev_uuid,
    .prov_unicast_addr   = PROV_OWN_ADDR,
    .prov_start_address  = 0x0005,
    .prov_attention      = 0x00,
    .prov_algorithm      = 0x00,
    .prov_pub_key_oob    = 0x00,
    .prov_static_oob_val = NULL,
    .prov_static_oob_len = 0x00,
    .flags               = 0x00,
    .iv_index            = 0x00,
};
```



### 函数部分

#### 节点储存的有关函数

１．节点的存储函数

该函数的主要功能说检测nvs中是否含有配置好的mesh节点数据，如果该节点配置好使用过了，则不进行配置，否则将其写入定义好的十个node节点中，定义的数据为厂商设备号，元素，以及开关状态

```c
//储存节点信息
static esp_err_t example_ble_mesh_store_node_info(const uint8_t uuid[16], uint16_t unicast,
                                                  uint8_t elem_num, uint8_t onoff_state)
{
    int i;

    if (!uuid || !ESP_BLE_MESH_ADDR_IS_UNICAST(unicast)) {
        return ESP_ERR_INVALID_ARG;
    }

    /* Judge if the device has been provisioned before */
    for (i = 0; i < ARRAY_SIZE(nodes); i++) {
        if (!memcmp(nodes[i].uuid, uuid, 16)) {
            ESP_LOGW(TAG, "%s: reprovisioned device 0x%04x", __func__, unicast);
            nodes[i].unicast = unicast;
            nodes[i].elem_num = elem_num;
            nodes[i].onoff = onoff_state;
            return ESP_OK;
        }
    }

    for (i = 0; i < ARRAY_SIZE(nodes); i++) {
        if (nodes[i].unicast == ESP_BLE_MESH_ADDR_UNASSIGNED) {
            memcpy(nodes[i].uuid, uuid, 16);
            nodes[i].unicast = unicast;
            nodes[i].elem_num = elem_num;
            nodes[i].onoff = onoff_state;
            return ESP_OK;
        }
    }

    return ESP_FAIL;
}

```



２．取用节点信息的函数

返回node的节点信息

```c
static esp_ble_mesh_node_info_t *example_ble_mesh_get_node_info(uint16_t unicast)
{
    int i;

    if (!ESP_BLE_MESH_ADDR_IS_UNICAST(unicast)) {
        return NULL;
    }

    for (i = 0; i < ARRAY_SIZE(nodes); i++) {
        if (nodes[i].unicast <= unicast &&
                nodes[i].unicast + nodes[i].elem_num > unicast) {
            return &nodes[i];
        }
    }

    return NULL;
}
```



#### 消息处理

1.mesh消息的处理函数

指向common变量的指针包含了所有需要用到的mesh信息

```c
//设置 BLE Mesh 消息的常见字段
static esp_err_t example_ble_mesh_set_msg_common(esp_ble_mesh_client_common_param_t *common,
                                                 esp_ble_mesh_node_info_t *node,
                                                 esp_ble_mesh_model_t *model, uint32_t opcode)
{
    if (!common || !node || !model) {
        return ESP_ERR_INVALID_ARG;
    }

    common->opcode = opcode;
    common->model = model;
    common->ctx.net_idx = prov_key.net_idx;
    common->ctx.app_idx = prov_key.app_idx;
    common->ctx.addr = node->unicast;
    common->ctx.send_ttl = MSG_SEND_TTL;
    common->ctx.send_rel = MSG_SEND_REL;
    common->msg_timeout = MSG_TIMEOUT;
    common->msg_role = MSG_ROLE;

    return ESP_OK;
}
```



相关的库函数结构体：

１.蓝牙 Mesh 客户端的通用参数结构体

```c
typedef struct {
    esp_ble_mesh_opcode_t opcode;   /*!<操作码 */
    esp_ble_mesh_model_t *model;    /*!< 模式指针*/
    esp_ble_mesh_msg_ctx_t ctx;     /*!< 消息的上下文（Context）。包含了发送消息的一些上下文信息，如源地址、目标地址、TTL（Time To Live）等。 */
    int32_t msg_timeout;            /*!< 发送消息的超时时间 */
    
    uint8_t msg_role;               /*!< 设备的角色 　节点或者配网器*/
} esp_ble_mesh_client_common_param_t;
```



２.Mesh 消息上下文的结构体类型,只截取了例程用到的

```c
typedef struct {
    /** NetKey Index of the subnet through which to send the message. */
    uint16_t net_idx;

    /** AppKey Index for message encryption. */
    uint16_t app_idx;

    /** Remote address. */
    uint16_t addr;
 
    /** Force sending reliably by using segment acknowledgement */
    uint8_t  send_rel: 1;

    /** TTL, or ESP_BLE_MESH_TTL_DEFAULT for default TTL. */
    uint8_t  send_ttl;

} esp_ble_mesh_msg_ctx_t;
```



2.配网函数

设置节点设备的名称，并且把它存储到nvs中。然后根据单播地址找到相关的节点信息，通过调用消息处理函数写入到common，然后

```c
static esp_err_t prov_complete(int node_idx, const esp_ble_mesh_octet16_t uuid,
                               uint16_t unicast, uint8_t elem_num, uint16_t net_idx)
{
    esp_ble_mesh_client_common_param_t common = {0};
    esp_ble_mesh_cfg_client_get_state_t get_state = {0};
    esp_ble_mesh_node_info_t *node = NULL;
    char name[11] = {0};
    int err;

    ESP_LOGI(TAG, "node index: 0x%x, unicast address: 0x%02x, element num: %d, netkey index: 0x%02x",
             node_idx, unicast, elem_num, net_idx);
    ESP_LOGI(TAG, "device uuid: %s", bt_hex(uuid, 16));

    sprintf(name, "%s%d", "NODE-", node_idx);
    
    //调用系统函数
    err = esp_ble_mesh_provisioner_set_node_name(node_idx, name);
    if (err) {
        ESP_LOGE(TAG, "%s: Set node name failed", __func__);
        return ESP_FAIL;
    }
	
    //调用存储函数
    err = example_ble_mesh_store_node_info(uuid, unicast, elem_num, LED_OFF);
    if (err) {
        ESP_LOGE(TAG, "%s: Store node info failed", __func__);
        return ESP_FAIL;
    }
	
    //调用取用函数	去掉也不影响
    node = example_ble_mesh_get_node_info(unicast);
    if (!node) {
        ESP_LOGE(TAG, "%s: Get node info failed", __func__);
        return ESP_FAIL;
    }
	
    //调用消息处理函数	去掉也不影响
    example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET);
    get_state.comp_data_get.page = COMP_DATA_PAGE_0;
    
    //看服务端的状态　　去掉也不影响
    err = esp_ble_mesh_config_client_get_state(&common, &get_state);
    if (err) {
        ESP_LOGE(TAG, "%s: Send config comp data get failed", __func__);
        return ESP_FAIL;
    }

    return ESP_OK;
}
```



相关的库函数

![image-20230614104239701](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624222916.png)

![image-20230614104513423](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624222915.png)





3.链路函数

很简单

```c
static void prov_link_open(esp_ble_mesh_prov_bearer_t bearer)
{
    ESP_LOGI(TAG, "%s link open", bearer == ESP_BLE_MESH_PROV_ADV ? "PB-ADV" : "PB-GATT");
}

static void prov_link_close(esp_ble_mesh_prov_bearer_t bearer, uint8_t reason)
{
    ESP_LOGI(TAG, "%s link close, reason 0x%02x",
             bearer == ESP_BLE_MESH_PROV_ADV ? "PB-ADV" : "PB-GATT", reason);
}
```





4.处理没有配置的广播包

核心功能就是将未配对的设备添加到 Provisioner 的设备队列中 ，让协议栈完成自动配网

```c
static void recv_unprov_adv_pkt(uint8_t dev_uuid[16], uint8_t addr[BD_ADDR_LEN],
                                esp_ble_mesh_addr_type_t addr_type, uint16_t oob_info,
                                uint8_t adv_type, esp_ble_mesh_prov_bearer_t bearer)
{
    esp_ble_mesh_unprov_dev_add_t add_dev = {0};
    int err;

    /* Due to the API esp_ble_mesh_provisioner_set_dev_uuid_match, Provisioner will only
     * use this callback to report the devices, whose device UUID starts with 0xdd & 0xdd,
     * to the application layer.
     */

    ESP_LOGI(TAG, "address: %s, address type: %d, adv type: %d", bt_hex(addr, BD_ADDR_LEN), addr_type, adv_type);
    ESP_LOGI(TAG, "device uuid: %s", bt_hex(dev_uuid, 16));
    ESP_LOGI(TAG, "oob info: %d, bearer: %s", oob_info, (bearer & ESP_BLE_MESH_PROV_ADV) ? "PB-ADV" : "PB-GATT");

    memcpy(add_dev.addr, addr, BD_ADDR_LEN);
    add_dev.addr_type = (uint8_t)addr_type;
    memcpy(add_dev.uuid, dev_uuid, 16);
    add_dev.oob_info = oob_info;
    add_dev.bearer = (uint8_t)bearer;
    /* Note: If unprovisioned device adv packets have not been received, we should not add
             device with ADD_DEV_START_PROV_NOW_FLAG set. */
    
    //将未配对的设备添加到 Provisioner 的设备队列中 
    //ADD_DEV_RM_AFTER_PROV_FLAG：在设备配对完成后从设备队列中移除该设备。
    //ADD_DEV_START_PROV_NOW_FLAG：立即开始对设备进行配对过程。
    //ADD_DEV_FLUSHABLE_DEV_FLAG：将设备标记为可刷写的设备。
    err = esp_ble_mesh_provisioner_add_unprov_dev(&add_dev,
            ADD_DEV_RM_AFTER_PROV_FLAG | ADD_DEV_START_PROV_NOW_FLAG | ADD_DEV_FLUSHABLE_DEV_FLAG);
    
    if (err) {
        ESP_LOGE(TAG, "%s: Add unprovisioned device into queue failed", __func__);
    }

    return;
}
```



```c
typedef struct {
    esp_ble_mesh_bd_addr_t addr;                 /*!< Device address */
    esp_ble_mesh_addr_type_t addr_type;     	 /*!< Device address type */
    uint8_t  uuid[16];                  		/*!< Device UUID */
    uint16_t oob_info;                  		/*!< Device OOB Info */
    esp_ble_mesh_prov_bearer_t bearer;  		/*!< Provisioning Bearer */
} esp_ble_mesh_unprov_dev_add_t;
```



库函数：

![image-20230614154740697](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624222921.png)





#### 回调函数

１．配网的回调函数

不加注释的就是单出打印看信息的状态机，没啥用，重要的事加注释的时间

```c
static void example_ble_mesh_provisioning_cb(esp_ble_mesh_prov_cb_event_t event,
                                             esp_ble_mesh_prov_cb_param_t *param)
{
    switch (event) {
    case ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT:
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT, err_code %d", param->provisioner_prov_enable_comp.err_code);
        break;
    case ESP_BLE_MESH_PROVISIONER_PROV_DISABLE_COMP_EVT:
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_PROV_DISABLE_COMP_EVT, err_code %d", param->provisioner_prov_disable_comp.err_code);
        break;
            
    	//接收没有配置的广播包　　调用配置函数,配置函数需要的参数是蓝牙栈内部的结构体提供　直接调用就好了
    case ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT:
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT");
        recv_unprov_adv_pkt(param->provisioner_recv_unprov_adv_pkt.dev_uuid, param->provisioner_recv_unprov_adv_pkt.addr,
                            param->provisioner_recv_unprov_adv_pkt.addr_type, param->provisioner_recv_unprov_adv_pkt.oob_info,
                            param->provisioner_recv_unprov_adv_pkt.adv_type, param->provisioner_recv_unprov_adv_pkt.bearer);
        break;
    case ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT:
        prov_link_open(param->provisioner_prov_link_open.bearer);
        break;
    case ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSE_EVT:
        prov_link_close(param->provisioner_prov_link_close.bearer, param->provisioner_prov_link_close.reason);
        break;
            
            //配置完成　调用配网函数，存储在nvs里面
    case ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT:
        prov_complete(param->provisioner_prov_complete.node_idx, param->provisioner_prov_complete.device_uuid,
                      param->provisioner_prov_complete.unicast_addr, param->provisioner_prov_complete.element_num,
                      param->provisioner_prov_complete.netkey_idx);
        break;
    case ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT:
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, err_code %d", param->provisioner_add_unprov_dev_comp.err_code);
        break;
    case ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT:
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT, err_code %d", param->provisioner_set_dev_uuid_match_comp.err_code);
        break;
            
            //名字设置完毕
    case ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT: {
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT, err_code %d", param->provisioner_set_node_name_comp.err_code);
        if (param->provisioner_set_node_name_comp.err_code == ESP_OK) {
            const char *name = NULL;
            name = esp_ble_mesh_provisioner_get_node_name(param->provisioner_set_node_name_comp.node_index);
            if (!name) {
                ESP_LOGE(TAG, "Get node name failed");
                return;
            }
            ESP_LOGI(TAG, "Node %d name is: %s", param->provisioner_set_node_name_comp.node_index, name);
        }
        break;
    }
            
            //应用密钥设置
    case ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT: {
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT, err_code %d", param->provisioner_add_app_key_comp.err_code);
        if (param->provisioner_add_app_key_comp.err_code == ESP_OK) {
            esp_err_t err = 0;
            prov_key.app_idx = param->provisioner_add_app_key_comp.app_idx;
            //绑定应用密钥
            err = esp_ble_mesh_provisioner_bind_app_key_to_local_model(PROV_OWN_ADDR, prov_key.app_idx,
                    ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_CLI, ESP_BLE_MESH_CID_NVAL);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Provisioner bind local model appkey failed");
                return;
            }
        }
        break;
    }
            
    case ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT:
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT, err_code %d", param->provisioner_bind_app_key_to_model_comp.err_code);
        break;
    default:
        break;
    }

    return;
}

```



相关的结构体：

```c
    struct ble_mesh_provisioner_recv_unprov_adv_pkt_param {
        uint8_t  dev_uuid[16];                  /*!< Device UUID of the unprovisioned device */
        esp_ble_mesh_bd_addr_t addr;            /*!< Device address of the unprovisioned device */
        esp_ble_mesh_addr_type_t addr_type;     /*!< Device address type */
        uint16_t oob_info;                      /*!< OOB Info of the unprovisioned device */
        uint8_t  adv_type;                      /*!< Avertising type of the unprovisioned device */
        esp_ble_mesh_prov_bearer_t bearer;      /*!< Bearer of the unprovisioned device */
        int8_t   rssi;                          /*!< RSSI of the received advertising packet */
    } provisioner_recv_unprov_adv_pkt;   
```



```c
struct ble_mesh_provisioner_set_node_name_comp_param {
        int err_code;                           /*!< Indicate the result of setting provisioned device name by the Provisioner */
        uint16_t node_index;                    /*!< Index of the provisioned device */
    } provisioner_set_node_name_comp;           /*!< Event parameter of ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT 
```

![image-20230614104846326](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624222943.png)

![image-20230614104941165](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624222936.png)



2.客户端模型的回调函数

```c
// 配置客户端的回调函数，用于处理配置客户端的事件和响应，执行相应的操作并处理操作结果
static void example_ble_mesh_config_client_cb(esp_ble_mesh_cfg_client_cb_event_t event,
                                              esp_ble_mesh_cfg_client_cb_param_t *param)
{
    esp_ble_mesh_client_common_param_t common = {0};
    esp_ble_mesh_node_info_t *node = NULL;
    uint32_t opcode;
    uint16_t addr;
    int err;

    opcode = param->params->opcode;
    addr = param->params->ctx.addr;

    ESP_LOGI(TAG, "%s, error_code = 0x%02x, event = 0x%02x, addr: 0x%04x, opcode: 0x%04" PRIx32,
             __func__, param->error_code, event, param->params->ctx.addr, opcode);

    if (param->error_code) {
        ESP_LOGE(TAG, "Send config client message failed, opcode 0x%04" PRIx32, opcode);
        return;
    }

    node = example_ble_mesh_get_node_info(addr);
    if (!node) {
        ESP_LOGE(TAG, "%s: Get node info failed", __func__);
        return;
    }

    	/*
ESP_BLE_MESH_CFG_CLIENT_GET_STATE_EVT：表示配置客户端接收到获取状态请求的事件。
ESP_BLE_MESH_CFG_CLIENT_SET_STATE_EVT：表示配置客户端接收到设置状态请求的事件。
ESP_BLE_MESH_CFG_CLIENT_PUBLISH_EVT：表示配置客户端接收到发布事件的事件。
ESP_BLE_MESH_CFG_CLIENT_TIMEOUT_EVT：表示配置客户端操作超时的事件。
ESP_BLE_MESH_CFG_CLIENT_EVT_MAX：表示配置客户端事件的最大值，用于遍历事件类型时的边界检查。
    	*/
    
    switch (event) {
            //ESP_BLE_MESH_CFG_CLIENT_GET_STATE_EVT：表示配置客户端接收到获取状态请求的事件。
    case ESP_BLE_MESH_CFG_CLIENT_GET_STATE_EVT:
        switch (opcode) {
            /*
ESP_BLE_MESH_MODEL_OP_BEACON_GET：获取 Beacon 状态。
ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET：获取设备的组成数据。
ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_GET：获取默认 TTL（Time To Live）值。
ESP_BLE_MESH_MODEL_OP_GATT_PROXY_GET：获取 GATT 代理状态。
ESP_BLE_MESH_MODEL_OP_RELAY_GET：获取 Relay 状态。
ESP_BLE_MESH_MODEL_OP_MODEL_PUB_GET：获取模型发布状态。
ESP_BLE_MESH_MODEL_OP_FRIEND_GET：获取 Friend 状态。
ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_GET：获取心跳发布状态。
ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_GET：获取心跳订阅状态。    
ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD mesh 网络中添加应用密钥
          */
        case ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET: {
            ESP_LOGI(TAG, "composition data %s", bt_hex(param->status_cb.comp_data_status.composition_data->data,
                     param->status_cb.comp_data_status.composition_data->len));
            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
            set_state.app_key_add.net_idx = prov_key.net_idx;
            set_state.app_key_add.app_idx = prov_key.app_idx;
            memcpy(set_state.app_key_add.app_key, prov_key.app_key, 16);
            err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
                return;
            }
            break;
        }
        default:
            break;
        }
        break;
    case ESP_BLE_MESH_CFG_CLIENT_SET_STATE_EVT:
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD: {
            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND);
            set_state.model_app_bind.element_addr = node->unicast;
            set_state.model_app_bind.model_app_idx = prov_key.app_idx;
            set_state.model_app_bind.model_id = ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_SRV;
            set_state.model_app_bind.company_id = ESP_BLE_MESH_CID_NVAL;
            err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config Model App Bind failed", __func__);
                return;
            }
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND: {
            esp_ble_mesh_generic_client_get_state_t get_state = {0};
            example_ble_mesh_set_msg_common(&common, node, onoff_client.model, ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET);
            err = esp_ble_mesh_generic_client_get_state(&common, &get_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Generic OnOff Get failed", __func__);
                return;
            }
            break;
        }
        default:
            break;
        }
        break;
    case ESP_BLE_MESH_CFG_CLIENT_PUBLISH_EVT:
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_STATUS:
            ESP_LOG_BUFFER_HEX("composition data %s", param->status_cb.comp_data_status.composition_data->data,
                               param->status_cb.comp_data_status.composition_data->len);
            break;
        case ESP_BLE_MESH_MODEL_OP_APP_KEY_STATUS:
            break;
        default:
            break;
        }
        break;
    case ESP_BLE_MESH_CFG_CLIENT_TIMEOUT_EVT:
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET: {
            esp_ble_mesh_cfg_client_get_state_t get_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET);
            get_state.comp_data_get.page = COMP_DATA_PAGE_0;
            err = esp_ble_mesh_config_client_get_state(&common, &get_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config Composition Data Get failed", __func__);
                return;
            }
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD: {
            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
            set_state.app_key_add.net_idx = prov_key.net_idx;
            set_state.app_key_add.app_idx = prov_key.app_idx;
            memcpy(set_state.app_key_add.app_key, prov_key.app_key, 16);
            err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
                return;
            }
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND: {
            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND);
            set_state.model_app_bind.element_addr = node->unicast;
            set_state.model_app_bind.model_app_idx = prov_key.app_idx;
            set_state.model_app_bind.model_id = ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_SRV;
            set_state.model_app_bind.company_id = ESP_BLE_MESH_CID_NVAL;
            err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config Model App Bind failed", __func__);
                return;
            }
            break;
        }
        default:
            break;
        }
        break;
    default:
        ESP_LOGE(TAG, "Not a config client status message event");
        break;
    }
}
//同理分析如下代码
```



```c
/** Configuration Client Model callback parameters */
typedef struct {
    int error_code;                                         /*!< Appropriate error code */
    esp_ble_mesh_client_common_param_t       *params;       /*!< The client common parameters */
    esp_ble_mesh_cfg_client_common_cb_param_t status_cb;    /*!< The config status message callback values */
} esp_ble_mesh_cfg_client_cb_param_t;
```



```c
typedef struct {
    esp_ble_mesh_opcode_t opcode;   /*!< Message opcode */
    esp_ble_mesh_model_t *model;    /*!< Pointer to the client model structure */
    esp_ble_mesh_msg_ctx_t ctx;     /*!< The context used to send message */
    int32_t msg_timeout;            /*!< Timeout value (ms) to get response to the sent message */
    /*!< Note: if using default timeout value in menuconfig, make sure to set this value to 0 */
    uint8_t msg_role;               /*!< Role of the device - Node/Provisioner */
} esp_ble_mesh_client_common_param_t;
```

![image-20230614105452075](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223004.png)





3.通用客户端模型的处理函数

开关灯，目前用不到，甚至可以注释掉

```c
//处理通用客户端的事件和响应。
static void example_ble_mesh_generic_client_cb(esp_ble_mesh_generic_client_cb_event_t event,
                                               esp_ble_mesh_generic_client_cb_param_t *param)
{
    esp_ble_mesh_client_common_param_t common = {0};
    esp_ble_mesh_node_info_t *node = NULL;
    uint32_t opcode;
    uint16_t addr;
    int err;

    opcode = param->params->opcode;
    addr = param->params->ctx.addr;

    ESP_LOGI(TAG, "%s, error_code = 0x%02x, event = 0x%02x, addr: 0x%04x, opcode: 0x%04" PRIx32,
             __func__, param->error_code, event, param->params->ctx.addr, opcode);

    if (param->error_code) {
        ESP_LOGE(TAG, "Send generic client message failed, opcode 0x%04" PRIx32, opcode);
        return;
    }

    node = example_ble_mesh_get_node_info(addr);
    if (!node) {
        ESP_LOGE(TAG, "%s: Get node info failed", __func__);
        return;
    }

    switch (event) {
    case ESP_BLE_MESH_GENERIC_CLIENT_GET_STATE_EVT:
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET: {
            esp_ble_mesh_generic_client_set_state_t set_state = {0};
            node->onoff = param->status_cb.onoff_status.present_onoff;
            ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET onoff: 0x%02x", node->onoff);
            /* After Generic OnOff Status for Generic OnOff Get is received, Generic OnOff Set will be sent */
            example_ble_mesh_set_msg_common(&common, node, onoff_client.model, ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET);
            set_state.onoff_set.op_en = false;
            set_state.onoff_set.onoff = !node->onoff;
            set_state.onoff_set.tid = 0;
            err = esp_ble_mesh_generic_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Generic OnOff Set failed", __func__);
                return;
            }
            break;
        }
        default:
            break;
        }
        break;
    case ESP_BLE_MESH_GENERIC_CLIENT_SET_STATE_EVT:
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET:
            node->onoff = param->status_cb.onoff_status.present_onoff;
            ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET onoff: 0x%02x", node->onoff);
            break;
        default:
            break;
        }
        break;
    case ESP_BLE_MESH_GENERIC_CLIENT_PUBLISH_EVT:
        break;
    case ESP_BLE_MESH_GENERIC_CLIENT_TIMEOUT_EVT:
        /* If failed to receive the responses, these messages will be resend */
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET: {
            esp_ble_mesh_generic_client_get_state_t get_state = {0};
            example_ble_mesh_set_msg_common(&common, node, onoff_client.model, ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET);
            err = esp_ble_mesh_generic_client_get_state(&common, &get_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Generic OnOff Get failed", __func__);
                return;
            }
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET: {
            esp_ble_mesh_generic_client_set_state_t set_state = {0};
            node->onoff = param->status_cb.onoff_status.present_onoff;
            ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET onoff: 0x%02x", node->onoff);
            example_ble_mesh_set_msg_common(&common, node, onoff_client.model, ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET);
            set_state.onoff_set.op_en = false;
            set_state.onoff_set.onoff = !node->onoff;
            set_state.onoff_set.tid = 0;
            err = esp_ble_mesh_generic_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Generic OnOff Set failed", __func__);
                return;
            }
            break;
        }
        default:
            break;
        }
        break;
    default:
        ESP_LOGE(TAG, "Not a generic client status message event");
        break;
    }
}

```



### 主函数和初始化函数

#### 主函数

```c
void app_main(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing...");
	//开启存储器
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
	
    //蓝牙初始化
    err = bluetooth_init();
    if (err) {
        ESP_LOGE(TAG, "esp32_bluetooth_init failed (err %d)", err);
        return;
    }
	
    //获得ｕｕｉｄ
    ble_mesh_get_dev_uuid(dev_uuid);

    /* Initialize the Bluetooth Mesh Subsystem */
    err = ble_mesh_init();
    if (err) {
        ESP_LOGE(TAG, "Bluetooth mesh init failed (err %d)", err);
    }
}

```



#### 初始化函数

初始化三个回调函数，每一个回调函数的参数都是协议栈里面的

```c
static esp_err_t ble_mesh_init(void)
{
    uint8_t match[2] = {0xdd, 0xdd};
    esp_err_t err = ESP_OK;

    prov_key.net_idx = ESP_BLE_MESH_KEY_PRIMARY;
    prov_key.app_idx = APP_KEY_IDX;
    memset(prov_key.app_key, APP_KEY_OCTET, sizeof(prov_key.app_key));

   //三个回调函数
esp_ble_mesh_register_prov_callback(example_ble_mesh_provisioning_cb);
   esp_ble_mesh_register_config_client_callback(example_ble_mesh_config_client_cb);
   esp_ble_mesh_register_generic_client_callback(example_ble_mesh_generic_client_cb);

    //初始化ｍｅｓｈ
    err = esp_ble_mesh_init(&provision, &composition);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize mesh stack (err %d)", err);
        return err;
    }

    err = esp_ble_mesh_provisioner_set_dev_uuid_match(match, sizeof(match), 0x0, false);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set matching device uuid (err %d)", err);
        return err;
    }

    err = esp_ble_mesh_provisioner_prov_enable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable mesh provisioner (err %d)", err);
        return err;
    }

    err = esp_ble_mesh_provisioner_add_local_app_key(prov_key.app_key, prov_key.net_idx, prov_key.app_idx);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add local AppKey (err %d)", err);
        return err;
    }

    ESP_LOGI(TAG, "BLE Mesh Provisioner initialized");

    return err;
}

```

![image-20230614111556442](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223007.png)

![image-20230614111626962](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223011.png)

![image-20230614111651643](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223013.png)

![image-20230614111716568](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223015.png)





## 事件统计

#### １.provisioning_cb事件

```
ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT
Provisioner 启用供应功能完成事件

ESP_BLE_MESH_PROVISIONER_PROV_DISABLE_COMP_EVT
Provisioner 禁用供应功能完成事件

ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT
Provisioner 收到未配置的设备信标事件

ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT
Provisioner 建立 BLE Mesh 链接事件

ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSE_EVT
Provisioner 关闭 BLE Mesh 链接事件

ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT
Provisioner 配置完成事件

ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT
Provisioner 将设备添加到列表中，其中包含正在等待/将要配置完成事件的设备

ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT
Provisioner 设置要与部分未配置设备 UUID 完成事件进行比较的值

ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT
Provisioner 设置节点名称完成事件

ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT
Provisioner 添加本地应用程序密钥完成事件

ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT
Provisioner 将本地模型与本地应用程序密钥完成事件绑定
```



#### ２.config_client_cb事件

```
ESP_BLE_MESH_CFG_CLIENT_GET_STATE_EVT
配置客户端收到了获取状态的响应

ESP_BLE_MESH_CFG_CLIENT_SET_STATE_EVT
配置客户端收到了设置状态的响应

ESP_BLE_MESH_CFG_CLIENT_PUBLISH_EVT
配置客户端成功地发布了配置消息

ESP_BLE_MESH_CFG_CLIENT_TIMEOUT_EVT
配置客户端在特定的时间段内未能接收到预期的响应或确认
```

操作码

```
ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET
配置成分数据获取

ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD
配置 AppKey 添加

ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND
配置模型应用程序绑定

ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_STATUS
获取组成数据状态的操作
```



#### ３.generic_client_cb事件

```
ESP_BLE_MESH_GENERIC_CLIENT_GET_STATE_EVT
通用客户端收到了获取状态的响应

ESP_BLE_MESH_GENERIC_CLIENT_SET_STATE_EVT
通用客户端收到了设置状态的响应

ESP_BLE_MESH_GENERIC_CLIENT_PUBLISH_EVT
通用客户端成功地发布了配置消息

ESP_BLE_MESH_GENERIC_CLIENT_TIMEOUT_EVT
通用客户端在特定的时间段内未能接收到预期的响应或确认
```



操作码：

```
ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET
获取通用开关状态的操作

ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET
设置通用开关状态的操作

```





## 配网实验及现象

共计需要三个板子

第一个板子：打开配网的ble_mesh_provisioner例程　作为配网器

其它两个板子使用ble_mesh_client例程			作为未配网的目标节点



修改例程，在每一个事件下面打印相关日志，获得的日志如下

![image-20230614181744813](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223025.png)

![image-20230614181829970](https://ayu-990121-1302263000.cos.ap-nanjing.myqcloud.com/makedown/20230624223027.png)



### 实验中系统的运行顺序：

1. 在全局变量中提前配置好相关的模型和元素
2. 协议栈先进入*ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT*事件，调用*recv_unprov_adv_pk*t函数将未配对的设备添加到 Provisioner 的设备队列中
3. 进入*ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT*　事件
4. 进入*ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT*事件　将设备添加到列表　协议栈自己进行配置　不需要调用
5. 进入*ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT*事件　添加密钥
6. 进入*ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT*事件　调用*prov_complete*函数将节点信息存储在nvs中，防止重复连接
7. 进入*ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSED_EVT*事件　　配网介绍

## 附录：

### 具体日志

```
I (435) EXAMPLE: Initializing...
I (445) BT_INIT: BT controller compile version [80abacd]
I (445) phy_init: phy_version 540,a5d905b,Oct 20 2022,19:36:11
I (485) system_api: Base MAC address is not set
I (495) system_api: read default base MAC address from EFUSE
I (495) BT_INIT: Bluetooth MAC: 84:f7:03:80:02:7a

I (585) EXAMPLE: ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT, err_code 0
I (585) EXAMPLE: ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT, finish 

I (595) EXAMPLE: ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT, err_code 0
I (595) EXAMPLE: ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT, finish 

I (605) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT, err_code 0
prov_key refresh  app_idx : 0
I (615) EXAMPLE: ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT, err_code 0
I (625) EXAMPLE: BLE Mesh Provisioner initialized
node_0  unicast :  0,elements: 0
node_1  unicast :  0,elements: 0
I (4495) EXAMPLE: ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT
I (4495) EXAMPLE: address: 4c11aef5505e, address type: 0, adv type: 3
I (4495) EXAMPLE: device uuid: dddd4c11aef5505e0000000000000000
I (4505) EXAMPLE: oob info: 0, bearer: PB-ADV
recv_unprov_adv_pkt  finish
I (4515) EXAMPLE: ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT, finish 

I (4525) EXAMPLE: PB-ADV link open
I (4525) EXAMPLE: ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT, finish 

I (4535) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, err_code 0
I (4535) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, finish 

node_0  unicast :  0,elements: 0
node_1  unicast :  0,elements: 0
I (6525) EXAMPLE: node index: 0x0, unicast address: 0x05, element num: 3, netkey index: 0x00
I (6525) EXAMPLE: device uuid: dddd4c11aef5505e0000000000000000
prov_complete  finish
I (6535) EXAMPLE: ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT, finish 

I (6545) EXAMPLE: ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT, err_code 0
I (6555) EXAMPLE: finished config Node 0 name is: NODE-0
I (6555) EXAMPLE: ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT, finish 

I (8425) EXAMPLE: PB-ADV link close, reason 0x00
I (8425) EXAMPLE: ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSE_EVT, finish 

node_0  unicast :  5,elements: 3
node_1  unicast :  0,elements: 0
I (14655) EXAMPLE: ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT
I (14655) EXAMPLE: address: 7cdfa186da02, address type: 0, adv type: 3
I (14655) EXAMPLE: device uuid: dddd7cdfa186da020000000000000000
I (14665) EXAMPLE: oob info: 0, bearer: PB-ADV
recv_unprov_adv_pkt  finish
I (14675) EXAMPLE: ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT, finish 

I (14685) EXAMPLE: ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT
I (14685) EXAMPLE: address: 7cdfa186da02, address type: 0, adv type: 3
I (14695) EXAMPLE: device uuid: dddd7cdfa186da020000000000000000
I (14705) EXAMPLE: oob info: 0, bearer: PB-ADV
recv_unprov_adv_pkt  finish
I (14705) EXAMPLE: ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT, finish 

I (14715) EXAMPLE: PB-ADV link open
I (14725) EXAMPLE: ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT, finish 

I (14725) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, err_code 0
I (14735) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, finish 

W (14745) BLE_MESH: Device already exists in queue
I (14745) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, err_code -120
I (14755) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, finish 

node_0  unicast :  5,elements: 3
node_1  unicast :  0,elements: 0
I (16595) EXAMPLE: node index: 0x1, unicast address: 0x08, element num: 3, netkey index: 0x00
I (16605) EXAMPLE: device uuid: dddd7cdfa186da020000000000000000
prov_complete  finish
I (16605) EXAMPLE: ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT, finish 

I (16615) EXAMPLE: ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT, err_code 0
I (16625) EXAMPLE: finished config Node 1 name is: NODE-1
I (16625) EXAMPLE: ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT, finish 

I (18495) EXAMPLE: PB-ADV link close, reason 0x00
I (18495) EXAMPLE: ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSE_EVT, finish 

I (20405) EXAMPLE: ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT
I (20405) EXAMPLE: address: 7cdfa186da02, address type: 0, adv type: 3
I (20415) EXAMPLE: device uuid: dddd7cdfa186da020000000000000000
I (20415) EXAMPLE: oob info: 0, bearer: PB-ADV
recv_unprov_adv_pkt  finish
I (20425) EXAMPLE: ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT, finish 

I (20435) EXAMPLE: ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT
I (20435) EXAMPLE: address: 7cdfa186da02, address type: 0, adv type: 3
I (20445) EXAMPLE: device uuid: dddd7cdfa186da020000000000000000
I (20455) EXAMPLE: oob info: 0, bearer: PB-ADV
recv_unprov_adv_pkt  finish
I (20465) EXAMPLE: ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT, finish 

I (20465) EXAMPLE: PB-ADV link open
I (20475) EXAMPLE: ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT, finish 

I (20485) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, err_code 0
I (20485) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, finish 

W (20495) BLE_MESH: Device already exists in queue
I (20505) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, err_code -120
I (20515) EXAMPLE: ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, finish 

node_0  unicast :  5,elements: 3
node_1  unicast :  8,elements: 3
```





