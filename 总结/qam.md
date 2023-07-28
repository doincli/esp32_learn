# QAM I2C总结

## 初始化

初始化I2C包括引脚以及master引脚

```c
esp_err_t qma7981_master_init(void)
 {
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK){
        return err;
    }
    read_mux = xSemaphoreCreateMutex();
    if (read_mux == NULL){
        ESP_LOGE(TAG, "mux init fail\n");
        return ESP_FAIL;
    }
    
    return i2c_driver_install(i2c_master_port, conf.mode, 1, I2C_MASTER_TX_BUF_DISABLE, 0);
}
```

![image-20230728120730565](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230728120730565.png)





## 读命令

```c
  return i2c_master_write_read_device(I2C_MASTER_NUM, QMA7981_ADDR, &reg_addr, 1, data, len, 1000 / portTICK_PERIOD_MS);
```

![image-20230728115733707](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230728115733707.png)

```c
esp_err_t i2c_master_write_read_device(i2c_port_t i2c_num, uint8_t device_address,
                                       const uint8_t* write_buffer, size_t write_size,
                                       uint8_t* read_buffer, size_t read_size,
                                       TickType_t ticks_to_wait)
{
    esp_err_t err = ESP_OK;
    uint8_t buffer[I2C_TRANS_BUF_MINIMUM_SIZE] = { 0 };

    i2c_cmd_handle_t handle = i2c_cmd_link_create_static(buffer, sizeof(buffer));
    assert (handle != NULL);

    err = i2c_master_start(handle);
    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_write_byte(handle, device_address << 1 | I2C_MASTER_WRITE, true);
    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_write(handle, write_buffer, write_size, true);
    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_start(handle);
    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_write_byte(handle, device_address << 1 | I2C_MASTER_READ, true);
    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_read(handle, read_buffer, read_size, I2C_MASTER_LAST_NACK);
    if (err != ESP_OK) {
        goto end;
    }

    i2c_master_stop(handle);
    err = i2c_master_cmd_begin(i2c_num, handle, ticks_to_wait);

end:
    i2c_cmd_link_delete_static(handle);
    return err;
}
```





## 写命令

```c
 ret = i2c_master_write_to_device(I2C_MASTER_NUM, QMA7981_ADDR, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS);
```

![image-20230728115828490](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230728115828490.png)

```c
esp_err_t i2c_master_write_to_device(i2c_port_t i2c_num, uint8_t device_address,
                                     const uint8_t* write_buffer, size_t write_size,
                                     TickType_t ticks_to_wait)
{
    esp_err_t err = ESP_OK;
    uint8_t buffer[I2C_TRANS_BUF_MINIMUM_SIZE] = { 0 };

    i2c_cmd_handle_t handle = i2c_cmd_link_create_static(buffer, sizeof(buffer));
    assert (handle != NULL);

    err = i2c_master_start(handle);
    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_write_byte(handle, device_address << 1 | I2C_MASTER_WRITE, true);
    if (err != ESP_OK) {
        goto end;
    }

    err = i2c_master_write(handle, write_buffer, write_size, true);
    if (err != ESP_OK) {
        goto end;
    }

    i2c_master_stop(handle);
    err = i2c_master_cmd_begin(i2c_num, handle, ticks_to_wait);

end:
    i2c_cmd_link_delete_static(handle);
    return err;
}
```



## 其它API

```c
float qma7981_read_DXM()
{
    uint8_t data[2];
    int16_t XA;
    float X_AXIS_A;
    BaseType_t esp_status = xSemaphoreTake(read_mux, 1000/portTICK_PERIOD_MS);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG, "mux take fail\n");
    }
    qma7981_register_read(QMA7981_DXM_ADDR, data, 2);
    esp_status = xSemaphoreGive(read_mux);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG, "mux give fail\n");
    }
    XA = (data[0] & 0xFC) | (data[1]<< 8);
    X_AXIS_A = (float) XA / QMA7981_MAX_VALUE ;
    return X_AXIS_A;
}

float qma7981_read_DYM()
{
    uint8_t data[2];
    int16_t YA;
    float Y_AXIS_A;
    BaseType_t esp_status = xSemaphoreTake(read_mux, 1000/portTICK_PERIOD_MS);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG,  "mux take fail\n");
    }
    qma7981_register_read(QMA7981_DYM_ADDR, data, 2);
    esp_status =xSemaphoreGive(read_mux);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG,  "mux give fail\n");
    }
    YA = (data[0] & 0xFC) | (data[1]<< 8);
    Y_AXIS_A = (float) YA / QMA7981_MAX_VALUE ;
    return Y_AXIS_A;
}

float qma7981_read_DZM()
{
    uint8_t data[2];
    int16_t ZA;
    float Z_AXIS_A;
    BaseType_t esp_status = xSemaphoreTake(read_mux, 1000/portTICK_PERIOD_MS);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG, "mux take fail\n");
    }
    qma7981_register_read(QMA7981_DZM_ADDR, data, 2);
    esp_status = xSemaphoreGive(read_mux);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG, "mux give fail\n");
    }
    ZA = (data[0] & 0xFC) | (data[1]<< 8);
    Z_AXIS_A = (float) ZA / QMA7981_MAX_VALUE ;
    return Z_AXIS_A;
}

esp_err_t qma7981_setMode(uint8_t cmd)
{
    return  qma7981_register_write_byte(QMA7981_MODE_ADDR, cmd);
}

esp_err_t qma7981_setAcc(uint8_t cmd)
{
    return  qma7981_register_write_byte(QMA7981_RANGE_ADDR, cmd);
}

esp_err_t qma7981_driver_delete()
{
   return  i2c_driver_delete(I2C_MASTER_NUM);  
}
```

# 基础知识补充

## IIC协议简介

IIC（inter-integrated Circuit集成电路总线)总线支持设备之间的短距离通信，用于处理器和一些外围设备之间的接口，它需要两根信号线来完成信息交换。IIC的一个特殊工艺优势是微控制器只需要两个通用I/O引脚和软件即可控制芯片网络。IIC最早是飞利浦在1982年开发设计并用于自己的芯片上，一开始只允许100Khz、7-bit标准地址，1992年，IIC的第一个公共规范发行，增加了400Khz的快速模式以及10bit地址扩展。

## IIC协议内容

IIC协议把传输的消息分为两种类型的帧： 

地址帧–用于master指明消息发往哪搁slave；

 数据帧（单个或者连续）– 由master发往slave的数据或者是接收到来自于slave的数据。一次读写单位为8bit,高位先发 

注：IIC读写是相对于master来说的。

 **IIC总线的数据传送**

 **1、数据位有效性的规定：**

 IIC总线进行数据传送时，时钟信号为高电平期间，数据线上的数据必须保持稳定，只有在时钟电平为低电平期间，数据线上的高电平或者低电平状态才允许变化。 

即数据在时钟线SCL的上升沿到来之前就需要准备好，并在下降沿到来之前必须保持稳定。

 **2、空闲状态：**

 当IIC总线的数据线SDA和时钟线同时处于高电平时，规定为总线的空闲状态。此时各个器件输出级场效应管均处在截止状态，即释放总线，由两条信号线的上拉电阻把电平拉高。

 **3、起始和终止信号：** 

SCL为高电平期间，SDA信号线由高电平向低电平的变化表示起始信号；SCL为高电平期间，SDA由低电平向高电平的变化表示终止信号。

 SDA：由低到高 – 起始信号 

SDA：由高到低 – 终止信号 

起始信号和终止信号都是由主机发送的，起始信号产生之后，总线处于被占用的状态，在终止信号产生之后，总线就处于空闲状态。 

![在这里插入图片描述](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/86c385cacb57ffc40d91badb6ffd1042.jpeg)

**4、应答信号：** 

发送器每发送一个字节（8个bit），就在时钟脉冲9期间释放数据线，由接收器反馈一个应答信号。

应答信号为低电平时，规定为有效应答（ACK，简称应答位），表示接收器已经成功地接受了该字节 

应答位为高电平时，规定为非应答信号（NACK），一般表示接收器接收该字节没有成功。

对于反馈的有效应答信号ACK的要求是：接收器在第9个时钟脉冲之前的低电平期间将数据线SDA拉低，并且确保在该时钟的高电平期间为稳定的低电平。如果接受器是主控器，则在它收到最后一个字节后，发送一个NACK信号，以通知被控发送器结束数据发送，并释放数据线SDA，以便主控接收器发送一个停止信号P。

 **5、数据传送格式：** 

（1）字节传输与应答：每一个字节必须保证是8bit长度。数据传送时，先传送最高位（MSB），每一个被传送的字节后面都必须跟随1bit的应答位（即每一帧数据一共有9bit） 

（2）在IIC总线上传送的每一位数据都有一个时钟脉冲相对应（或同步控制），即在SCL串行时钟的配合下，在SDA上逐位地串行传送每一位数据。数据位的传输是边沿触发。 

**6、总线寻址：** 

（1）IIC协议规定采用7bit的寻址字节（寻址字节是起始信号后的第一个字节）其位定义如下：

 | bit | 7 | 6 | 5 | 4 ｜3 ｜2｜1｜ 0｜ 

D7 ～ D1位组成从机地址。 

D0位是数据传送的方向，0:表示主机向从机写数据，1:表示主机由从机读数据。 

（2）主设备往从设备中写寄存器。数据传输格式如下： 

![在这里插入图片描述](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/aacce0aa18c36cd1a6da21a790288a74.png)

 1）首先由由主机发送一个开始信号， 

2）然后主机在数据线上广播从机地址 

3）确定数据的传输方向（是读还是写） 

4）“A”代表应答，表示主机找到了从机。 

5）主机发送一个字节数据，从机接收到后，会返回给主机一个应答信号，直到主机发送完数据，或者从机返回一个NACK信号，表示从机不再接收主机发送的数据。主机发送一个stop信号。此次数据传输完成。 



（2）主机读取从机寄存器值： 

![在这里插入图片描述](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/ee03512728d0d03f09e767ea61436a6b.png)

 1）首先由主机发送一个开始信号 

2）然后主机在数据总线上广播地址 

3）确定数据的传输方向（读1） 

4）从机返回一个“A”代表应答，表示主机找到了从机。 

5）从机返回读的数据

6）主机返回一个应答信号，表示成功接收从机发出的一个数据 

7）当主机接收数据完毕，返回一个非应答信号“NACK”，从机释放数据总线，接着主机发送一个“P”信号，表示此次数据传输结束。 

![在这里插入图片描述](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/fea4b6f87f41cfd026ee5994241b25ec.jpeg)

 1）主机首先产生START信号 

2）然后紧跟着发送一个从机地址，注意此时该地址的第8位为0，表明是向从机写命令，

 3）主机等待从机的应答信号(ACK) 

4）当主机收到应答信号时，发送要访问的地址，继续等待从机的应答信号， 

5）当主机收到应答信号后，主机要改变通信模式(主机将由发送变为接收，从机将由接收变为发送)所以主机重新发送一个开始start信号，然后紧跟着发送一个从机地址，注意此时该地址的第8位为1，表明将主机设 置成接收模式开始读取数据，

 6）主机等待从机的应答信号，当主机收到应答信号时，就可以接收1个字节的数据，当接收完成后，主机发送非应答信号，表示不在接收数据 

7）主机进而产生停止信号，结束传送过程。



# 
