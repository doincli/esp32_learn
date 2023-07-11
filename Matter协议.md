# Matter协议

## 1. 什么是Matter协议？

  目前的智能家居行业使用解决方案众多，相互之间隔离严重，有WiFi、蓝牙、ZigBee、蜂窝或者有线等等不同通讯协议的设备。不仅不同协议之间的设备不能互通，而且连相同通讯协议之间的设备也不能互通。例如谷歌助手控制谷歌产品，亚马逊语音设备可以用于亚马逊的设备，苹果使用Siri来驱动其产品，而国内小米、阿里等物联网平台也都有自己的生态。而Matter协议就是为了解决这些兼容性问题而诞生的， 并且在2022年十月CSA（连接标准联盟）正式发布了Matter标准协议1.0版本。

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/11d7bc6d306546d9aeac724cf28928ee.webp#pic_center)

  Matter标准规范基于互联网协议，使智能物联网设备、移动应用程序和云服务能够进行通信并定义一组特定的基于 IP 的网络技术以进行设备认证。以经典的协议栈层级做类比，最底层是物理和媒介层，包括当前主流无线标准WiFi、Thread、Zigbee、BLE等;往上是网络层，包括IP协议;再往上为传输层，有TCP和UDP协议;最上层是应用层。Matter就设计在应用层，相当于创造了一个通用语言，使不同厂家、不同协议的产品可以彼此听懂对方的表达。智能物联网设备可以通过蓝牙加入到Matter网络中，通过整合Thread、Zigbee、BLE等协议实现互联，还能通过网桥连接到其他协议的设备，大大简化智能物联网设备商的开发成本、用户的使用成本以及安装落地的服务成本。

  简单地说，Matter是一个综合了不同设备和生态系统的总体标准。这是一种开源标准，任何智能设备都可以采用它来创造更无缝的体验。通过Matter，可以购买任何兼容的智能家居设备，比如灯泡或门锁，并将其设置为可在任何平台上使用。换句话说，Matter将其他智能家居生态系统连接在一起，以便于在所有智能家居设备上使用。可以在任何智能家居生态系统中设置任何Matter支持的产品，从Apple Home Kit到谷歌Assistant、AmazonAlexa等等。

  那Matter标准协议的诞生是不是用来替代其他的协议比如WiFi、蓝牙、ZigBee等协议呢？其实不是的，Matter是一个基于IPV6的上层协议，而底层协议就是基于现有的协议。从下图Matter协议的框架图可以看到，Matter协议的底层协议目前支持的有WiFi，Thread和以太网三种协议，而蓝牙是Matter标准规定的用来配网的协议。

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/a1aacf3aadbc4ae9a7361ad1a7d75a7d.png#pic_center)

## 2. Matter标准协议的架构

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/aa14b5e54a1f4c289bcc11b8e239a5cf.png#pic_center)

Matter标准协议的分层如上图所示，最上层是应用层，最下层是基于IP的协议管理，下面分别对每一层做简单的介绍。

- **Application layer** ： 应用层 对应于设备的具体业务交互逻辑。例如一个灯的应用里面包含了开关、颜色调节等具体的特性。
- **Data Model layer**： 数据模型层 对应于 支持应用功能的一些基础数据和元素模型。当需要有设备进行交互的时候，应用就会通过操作这些数据模型来实现。

- **Interaction Model layer**： 交互模型层定义了一组在客户端和服务器设备之间执行的交互。 例如读取或写入服务器设备上的属性将对应于设备上的应用程序行为。 这些交互作用于数据模型层定义的元素。

- **Action Framing layer**： 动作框架层 一旦使用交互模型层构造了一个动作，它就会被序列化为一个二进制的数据包，以便为网络传输进行编码传输。 此过程在动作框架层中处理。
- **Security Layer**：上面编码后的数据需要经过安全层处理编码的动作帧：消息会被加密并附加消息验证码。 这些操作确保数据在消息的发送者和接收者之间传输是安全的，保证数据不会被篡改。
- **Message Framing + Routing**： 通过上面的交互之后，现在的数据应是序列化、加密和签名，消息层使用必需和可选的标头字段构造有效负载格式，这些字段指定消息的属性以及逻辑路由信息。
- **IP Framing + Transport Management**: 在消息层构建最终有效负载后，将其发送到底层传输协议（TCP 或Matter的消息可靠性协议）以对数据进行 IP 管理。

  一旦在对等设备上接收到数据，它就会沿着协议栈向上传播，在协议栈中，各个层反转发送方对数据执行的操作，最终将消息传递给应用程序以供使用。

## 3. Matter 网络结构

   Matter 网络是基于IPV6的链路上架设的一种局域网协议，目前支持的链路层有Wifi、Threa、以太网这三种网络链路。Matter将网络视为一种开放的网络资源，所以Matter中并没有规定专有的网络所有权或访问权限。 因此，可以在同一组 IP 网络上覆盖多个 Matter 网络。

Matter底层不同通讯协议的设备之间如果需要互相通讯，则需要使用一个边界路由器来桥接。 Matter通过边界路由器实现网络内不同链路设备之间的互相通讯，例如WiFi设备和Thead设备之间的通讯。

### 3.1 Matter - Thread 网络结构

如果Matter网络内的设备都是基于Thread链路，则Matter网络结构如下图：

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/f9f0d467176e493183763f3c7f11cab0.png#pic_center)

### 3.2 Matter - WiFi 网络结构

如果Matter网络内的设备都是基于WiFi链路，则Matter网络结构如下图：

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/f8a42407537540ec95edce43d2f0c60f.png#pic_center)

### 3.3 Matter - Cross网络结构

如果Matter网络内的设备有基于WiFi链路的也有基于Thread链路的，则需要使用一个边界路由器来桥接两种类型的设备。设备通过边界路由器来加入到网络中，通过边界路由器节点可以Thread设备可以与WiFi设备互相通讯以及进行设备之间的联动。Matter网络结构如下图： BR表示的就是边界路由器

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/d75179e8b4d841febda66a5e530b30a0.png#pic_center)

## 4. Matter的现状及前景

  Matter的愿景是很好的，希望打破目前智能家居不同设备之间割裂的局面，并且Matter智能家居协议在成立之初就承诺，使用Matter协议的设备将可以兼容Alexa、Google Home和HomeKit生态， 这也是各个巨头愿意大力支持Matter协议的原因之一。目前Matter的标准协议1.0已经发布，谷歌、亚马逊、苹果等巨头本身就是其中的重要成员并且已经率先开始支持。谷歌的安卓系统、苹果的IOS系统，后面都会支持Matter协议。 亚马逊的Alexa音箱也已经支持了Matter设备。所以目前各个巨头对Matter的支持和推动力度还是很大的，Matter的生态还是比较容易起来的。

当然，Matter目前也有一些缺点，其中最重要的一点就是价格较高。Matter产品价格较高的原因，主要有两个方面：

1. Matter对芯片资源占用较高： 由于Matter下层是基于原有的链路WiFi或者Thread协议，上层又搭建了庞大的Matter协议，并且在安全等方面机制比较复杂，导致Matter协议非常吃资源，很多芯片厂家都是在性能最高的芯片上来适配Matter协议。并且Matter产品目前还没有形成规模，所以芯片和模组的价格也是比较高的。
2. 认证成本较高： 不仅要过原有的WiFi或Tread认证，还需要给CSA（连接标准联盟）交钱过Matter认证，并且Matter规定需要使用蓝牙来配网，所以还要过蓝牙技术联盟的BQB认证，这三个认证下来费用和时间的成本也是比较高的。

  总体上讲，Matter的发展还是要依赖各个巨头的推动，巨头大力推动，积极接入，Matter前期的生态就比较容易起来，这时候其他的中小厂商就愿意跟进，毕竟做的设备可以直接接入Alexa、Google Home和HomeKit生态还是非常吸引人的。当然如果巨头不再推动Matter标准，那Matter的互联互通就基本没有意义了，厂商也不会为这个而花费更高的成本来开发Matter产品了。目前巨头对Matter的支持力度还是很大的，非常期待Matter能够打破智能家居的碎片化，实现真正的互联互通。





## 5. Matter框架简介

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/c4ed04d495eb40319ebdff2b6cc77dcd.png#pic_center)

Matter是一个基于IPV6的运行在TCP/IP上的应用层协议，Matter的底层可以工作在WiFi、Thread或者以太网协议上，而蓝牙是用来完成Matter的入网配置的。

因为TCP/IP是一个网络结构，所以Matter也是一个网状结构的网络，如下图所示：

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/7ceea6b5629e443a8695a44282e584d9.png#pic_center)

网络有WiFi设备，Thread设备，还有其他非Matter类型的设备。其他典型的智能家居技术比如ZigBee和Zwave也可以通过网关来接入到Matter网络中，Matter对桥接设备也有相应的规范。

目前CSA规定Matter支持的设备类型有如下几中类型设备：

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/c3ad3f133296488480f94902a84aca19.png#pic_center)

- **照明电工类型**： 比如灯泡、开关、插座
- **HVAC设备**：比如空调、暖气
- **安防类设备**：报警器
- **接入控制类设备**： 比如智能门锁
- **TV类**： 电视
- **窗帘类**： 窗帘、百叶窗
- **桥接类设备**：将非Matter网络内的设备加入到Matter网络中的桥接设备。

## 6. Matter关键概念及特性介绍

Matter网络更像是一个虚拟的网络，他可以跨越不同的底层通讯协议，让不同协议的设备之间可以互相通讯。

- **Fabric**：在Matter协议里将这个虚拟的网络称为Fabric，Fabric是一群共享相同Trusted Root的设备的集合，每个Fabric有着自己的Fabric ID，这个ID是一个64-bit的整数。
- **Node**： 在Fabric中每个物理的设备称为一个Node，每个Node有自己的Node_ID，每个Node_ID也是64-bit的整数，在同一个Fabric网络内，该Node_ID是唯一的。
- **EndPoint**：每个Node可以划分为一个或多个逻辑实体，每个逻辑实体都是一个EndPoint，比如一个排查有四个插孔，那么每个插孔都可以用一个单独的EndPoint来表示。 EndPoint有三种类型，分别是Root node EndPoint、Composed Endpoint ， Leaf Endpoint。

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/79c79d7796d74821a2a6a61d16c247fd.png#pic_center)

Root node EndPoint： 是每个设备都必须具有的，用来描述设备的能力集合。

composed endpoint ：用来实现设备的一些复杂组合的功能

- **Culsters**： Culsters定义了设备的每个属性，以及控制该属性的命令，用来实现一个具体功能的模型。比如开关灯和插座可以使用onoff Culsters，比如设置灯的亮度、风扇的速度调节等可以使用一个Level Culsters来实现。 Culsters是一个client-server的通讯模式。Culsters的数据类型有attribute、Commands、Events三种。 Attribute主要是在服务端，例如On/Off Culsters和Level Culsters 的Attribute就是指的灯的开关和亮度状态。 Commands指的是客户端发送给服务端的Culsters状态的命令， Events指的是服务端状态变化后，主动通知给客户端。

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/69ed59ee1585496aaca14e7425fc19b7.png#pic_center)

  Matter设备实现互联互通，主要是靠在上层定义了一套统一的 Data Model（数据模型）， 在这套设备模型里面，设备内的逻辑功能单元是用EndPoint来表示的。每个EndPoint的功能有若干个Culsters来描述。 通讯的过程是有本地的EndPoint和远端的EndPoint来交互完成的，这个交互过程称之为interaction。

- Commissioning

  : 指的是Matter的配网过程，这个概念是从Thread继承过来的。Matter标准规定需要使用蓝牙来配网，配网流程比较复杂，我们会在后面的章节详细介绍。

  ![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/0e1e0ec71db4465397e25d8381a7c01d.png#pic_center)

  上图的Matter网络是一个thread和一个WiFi网络组成，灯泡和音箱是在Fabric A中，门锁和手机是在Fabric B中，那是否可以将灯泡同时加入到Fabric A和Fabric B中？

**答案是肯定的**，这就是Matter中的Multi-Admin特性。

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/180ff25ecad24149acd66cd9a7c74ee4.png#pic_center)

- **Multi-Admin**： Matter的Multi-Admin特性指的是可以让一个Matter设备同时加入到两个不同的Fabric网络中，并且同时被两个不同的Fabric管理员管理。比如上图的灯泡，即加入了Fabric A也加入了Fabric B中，正是由于Fabric的特性Matter设备才可以同时连接到多个生态系统，并且可以根据自己爱好切换不同的生态系统。

## 7. Matter关键概念总结

Matter中的概念和术语都不是新定义的。整体上看，Matter协议参考了之前很多通讯协议的概念和定义，本文上面只列出了一些关键的概念，还有一些其他的概念，大家了参考下表，也可以在CSA官网下载Matter标准协议文档学习。

![在这里插入图片描述](https://ucc.alicdn.com/images/user-upload-01/6bf4ae58629b470a869eb8f33a17ceaa.png#pic_center)