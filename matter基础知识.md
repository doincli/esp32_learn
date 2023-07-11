# matter1

您最近可能已经阅读了新推出的标准[Matter](https://medium.com/the-esp-journal/announcing-matter-previously-chip-on-esp32-84164316c0e3)。Matter 是[连接标准联盟](http://csa-iot.org/)的一项倡议，是通过物联网行业所有领导者的合作开发的。

人们对物质及其所要实现的目标感到非常兴奋。我们经常听到设备制造商和最终用户询问如何从 Matter 标准中受益。在本文中，让我们看看 Matter 对最终用户和设备制造商意味着什么。

## 对于最终用户

如果您是最终用户，Matter 将使智能家居对您来说更加自然和直观。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*fghJid9CmjlDKBcmtNmVBw.png)

**不再有孤岛：**

一家供应商的应用程序只能与自己的智能家居设备配合使用的日子已经一去不复返了。*支持 Matter 的应用程序可与任何支持 Matter 的设备配合使用。*

因此，也不再需要说：“哦，这是一个 iOS 生态系统设备，所以我的 Android 妻子无法很好地使用它”。

[亚马逊](https://developer.amazon.com/en-US/alexa/matter)、[苹果](https://developer.apple.com/videos/play/wwdc2021/10298/)、[谷歌](https://blog.google/products/google-nest/four-google-smart-home-updates-matter/)和[三星](https://news.samsung.com/global/samsung-smartthings-integrates-matter-into-ecosystem-bringing-matter-device-control-to-multiple-samsung-products)都在其最新公告中宣布集成对 Matter 的支持。因此，让我们与智能设备交互的典型设备：手机（iOS 和 Android）和语音助手/智能扬声器将支持开箱即用的基于 Matter 的设备。*您甚至可能不需要单独的手机应用程序即可使用运行 Matter 的设备。*

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*LpSUQAXVN6W_9bveO26CRA.png)

**更高的自动化程度：** Matter 可以实现有效的设备到设备通信，而无需任何其他中介参与。Matter 智能开关或传感器可以直接打开/关闭一个 Matter 灯泡（或一组灯泡），无需通过任何应用程序、云或技能。一旦设备设置完毕，控制就完全通过本地网络进行本地控制。

**不再有传输孤岛：** Matter 可与支持 Wi-Fi 以及 Thread/802.15.4 的设备配合使用。您的手机应用程序将能够同样好地控制使用这些传输方式中的任何一种的设备。

您还可以设置自动化，允许基于 802.15.4 的传感器直接打开基于 Wi-Fi 的灯泡，而无需应用程序或云进入图像。是的，你没有看错。一种称为边界路由器的设备（包括 Wi-Fi 和 802.15.4 传输）有助于桥接这两个网络，从而允许从一个网络到另一个网络的直接寻址。

此外，所有通信都通过本地 Wi-Fi/802.15.4 网络进行。因此，即使互联网中断，智能控制也能在本地网络的设备内继续工作。

对于使用 ZigBee 或 BLE Mesh 的现有产品，Matter 桥可以帮助您通过 Matter 生态系统访问这些设备。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*j0QE9Jq1JsuEebbmMh8Nuw.png)

**更多生态系统：**解决方案提供商可以提供基于 Matter 的更高级别的生态系统，为最终用户提供额外的功能。Matter 规范使设备能够更轻松地与多个生态系统配合使用。因此，同一设备可以*同时*与多个异构生态系统配合使用。更重要的是，设备供应商自己[可以创建自己的生态系统](https://blog.espressif.com/matter-multi-admin-identifiers-and-fabrics-a291371af365)（不仅限于手机操作系统或语音助手）并提供创新功能作为这些生态系统的一部分。请留意后续博客文章，了解有关 Matter Ecosystems 的更多详细信息。

**更好的安全性：** Matter 规范融入了安全功能，并得到了科技行业最优秀人士的同意和审查。这保证了通过 Matter 认证的设备依赖于经过严格审查和强大的行业标准。

总而言之，Matter 应该通过提供无缝集成和新的可能性来帮助加速智能家居的普及。

## 对于设备制造商

如果您是设备制造商，首先您的客户将受益于 Matter 的上述所有优势。其次，Matter 将帮助您更快地构建和扩展智能设备，而不会造成任何阻碍。

**易于开发：**设备制造商不再需要花费工程精力来艰苦地支持，更重要的是，认证多个生态系统。这通常是工程/成本开销，也是启动时间表开销。物质使制造设备变得更加容易和更快。

**开放的力量：** Matter 实施是[开放](https://github.com/project-chip/connectedhomeip)构建的，适用于各种供应商。这使得评估和开发速度更快。

即使在今天，您也可以尝试支持 Matter 的设备，只需前往[Git 存储库](https://github.com/project-chip/connectedhomeip/tree/master/examples/all-clusters-app/esp32)即可。

**设备 — 设备自动化：**设备到设备的自动化使得在智能家居中引入智能变得更加容易：传感器与灯泡相结合，可以自行工作。您的灯泡或传感器可以与任何其他 Matter 供应商的设备配合使用。

如果您是制造传感器或开关等始终与其他设备配合使用的设备制造商，这对您来说是个好消息。这意味着您可以构建智能设备，让客户发挥想象力，他们将设备的输出连接到什么（灯泡或风扇或其他任何供应商的产品）。

**Matter 生态系统：** Matter 生态系统是一种实现智能设备生态系统的方法，这些设备可以相互通信并构建引人注目的多设备场景。Matter 规范允许供应商创建这样一个生态系统，以便这些功能可以分层在顶层。

**制造商特定的创新：** Matter 规范有一个不断发展的设备类型列表、它们将具有的典型属性以及它们将遵守的命令。如果您的设备具有某些无法通过这些功能来表达的创新功能，您可以定义自己的属性，以便充分利用这些功能。

## 乐鑫与 Matter

自 Matter 成立以来，乐鑫一直积极与 Matter 合作。Matter 存储库的上游已[提供对 Espressif SoC 的支持。](https://github.com/project-chip/connectedhomeip/tree/master/examples/all-clusters-app/esp32)[您可以按照本文](https://blog.espressif.com/announcing-matter-previously-chip-on-esp32-84164316c0e3#gettingstarted)中的说明在 ESP32 或 ESP32-C3 上试用它。

我们目前正在致力于构建工具和文档，以帮助我们的客户完成产品开发过程的每一步，从解决方案架构到认证、制造和诊断。

我们对这次旅程感到兴奋，并期待听到您如何利用 Matter 进行构建。



# matter 簇、属性、命令

在上一篇文章中，我们了解了[“物质”对您意味着什么](https://blog.espressif.com/what-does-matter-mean-to-you-fa3bb53a7793)。今天我们来谈谈Matter的一个重要部分，它的数据模型。Matter 的数据模型定义了构建典型 Matter*节点*的典型元素。作为设备开发人员，您通常会通过此数据模型来表达设备的功能。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*_9MG2aOcb5hcZUX2XzWk9A.png)

简单的数据模型树

为了更好地理解，我们假设我们正在构建一个具有 2 个灯的灯具：一个可调光，一个简单的开/关。

## 物质元素

下图显示了如何在 Matter 的数据模型中表示这一点的简单视图。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*TgD1B9vrr0m3Bc8E2fZxDA.png)

物质数据模型的简单表示

**节点：**在我们的例子中，灯具是一个节点。这是一个唯一的网络可寻址实体，公开了一些功能。这通常是用户可以识别为整个设备的物理设备。

**端点：**每个节点都有多个端点。端点可以被认为是一个虚拟设备，它提供可以逻辑分组在一起的服务。在上面的示例中，我们的灯具有 2 个独立的灯，一盏可调光，一盏可开关。其中每一个都有自己的端点。

请注意，端点 0 已被保留。其中包含适用于整个节点的某些服务。我们将在本节后面讨论其中包含的内容。

Matter 规范定义了某些常见的**设备类型**。例如，您在端点 1 和 2 中看到的开/关灯和可调光灯是 Matter 定义的标准设备类型。设备类型用于指示一组常用功能。

**集群：** 集群将常用的功能组合在一起，形成可重用的构建块。

在我们的图表中，我们的第一个灯（端点 1）显示有 2 个标准集群：开/关集群和电平控制集群。On/Off 集群提供打开或关闭某些事物的服务。级别控制集群提供了一项服务，可以配置某些事物的级别。在我们的例子中，开/关集群帮助打开或关闭灯，电平控制集群帮助配置灯的亮度。

如果我们的灯光支持控制颜色，它将有另一个标准集群，称为颜色控制集群。

我们的第二个灯（端点 2）只是一个开/关灯，因此它只包括开/关集群。

正如您可能从图中注意到的，集群包含属性和命令。

**属性：**属性表示可以读取或写入的内容。在我们的示例中，OnOff 集群具有映射到设备实际状态的 OnOff 属性。同样，在“级别控制”群集中，有一个“当前级别”属性，该属性映射到设置的当前级别。

属性可以是持久的或易失的（重新启动后丢失），也可以是只读的或读写的。

Matter 规范包括属性可能的数据类型列表。这些包括典型的布尔值、整数（有符号/无符号）、浮点数、枚举、字符串，甚至集合（列表或结构）。

**命令：**集群命令提供了在集群上调用特定行为的能力。命令可以具有与其关联的参数。在上图中，开/关集群有一个*切换*命令，用于切换集群当前的开/关属性。Level Control 集群具有*MoveToLevel、Move、Step*等命令，它们以指定的方式移动集群的当前级别。

这些是 Matter 数据模型的典型元素。Matter 规范提供了标准集群及其属性和命令的列表。您可以仔细阅读此内容以检查它如何与您的设备的功能保持一致。

现在我们已经了解了 Matter 数据模型中的常见元素，让我们了解另一个概念：集群服务器和客户端。

## 集群服务器和客户端

每个 Matter 集群都有一个集群服务器和一个对应的集群客户端。在上图中，我们的设备包括集群服务器，因为它们提供服务。与这些的交互是通过集群客户端完成的。

下图很好地证明了这一点：

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*MlRcexBBVQwNLH19olIbSw.jpeg)

- 这里，Matter Dimmer Switch 实现了 OnOff 和 Level Control Cluster**客户端**。这些客户端与灯上相应的**服务器**通信，以控制它们。
- 我们还有一个 Matter Simple Switch，它仅实现 OnOff Cluster 客户端。
- 最后，控制同一盏灯的手机应用程序也会实现相关的集群**客户端**来控制灯。
- 请注意，调光开关、简单开关、灯光以及手机应用程序都是 Matter 节点。

让我们看一些其他例子来说明这一点。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*azQptaI6VSrmjqb5ZZ0M0Q.jpeg)

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*Bo8zcn71SwpAAId52Ejwgw.png)

电源开关和电源插座

## 端点0

最后，前面我们讨论了端点 0，这是一个具有“根节点”设备类型的端点。这是一个特殊的端点，具有特定于整个 Matter 节点的集群。此端点的一些典型集群包括：

- **基本信息 Cluster Server**：提供有关节点的基本信息，如固件版本、制造商等
- **ACL 集群服务器**：允许配置该节点的访问控制列表。
- **网络调试集群服务器**：允许在节点上配置网络（Wi-Fi、以太网、Thread）。

[请注意，Matter 规范定义了某些强制集群，这些集群应该是任何端点的一部分。类似地，每个集群可以具有规范中定义的某些强制属性和命令。为了简洁起见，我们在这里省略了所有这些。]

这是对物质数据模型常用元素的简短概述。希望它可以帮助您设想设备的功能如何与物质数据模型保持一致。





# 问题：设备到设备的自动化

在上一篇文章中，我们研究了 Matter 的数据模型。今天，让我们谈谈网络中的 Matter 设备如何相互交互，从而实现有用的自动化。例如，您可能需要家里有一个电灯开关来控制一个或多个灯泡。甚至是根据占用传感器的报告打开或关闭恒温器。

这是一个非常有趣的用例，通过现有的生态系统协议不容易实现。物质很容易实现这一点。这不需要任何云或电话应用程序的中介来实现。相反，这一切都是直接通过本地网络进行的。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*B_HImQPnRCfK6RKBqWVq-g.jpeg)

正如数据模型文章中提到的，每个 Matter 集群都有一个集群服务器和一个对应的集群客户端。通信发生在同一集群的客户端和服务器之间。从上面可以看出，交换机上的 OnOff 集群客户端可以与灯上的 OnOff 集群服务器通信来打开或关闭它。最终用户可以配置哪个设备可以与家里的哪个设备通信。

为了发生这种交互，开关应该知道有关灯光的详细信息。这是通过**设备绑定**来实现的。绑定表示一种持久关系，它向客户端端点通知一个或多个潜在交互的目标端点。用户（通过 Matter 手机应用程序）可以在设备之间建立绑定，无论它们来自哪个供应商。

设备与设备之间的交互可以通过两种方式完成：

## **1. 同步控制**

让我们回到上面显示的控制灯的开关示例。为此，交换机还需要有一个提供绑定服务的Binding集群服务器。用户将灯绑定到开关后，开关上的动作（开或关）会导致灯上的相应动作。这种情况如下图所示：

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*U854xZnv59FRNcbzHAoAqA.jpeg)

同样，调光开关需要有一个 OnOff 客户端、一个 Level Control 客户端和一个 Binding 服务器来控制可调光灯。

## **2. 异步通知（订阅-报告）**

该方法有利于从发布者向订阅者接收数据报告。订阅者可以**订阅**发布者上的属性和/或事件。

**上述恒温器和占用传感器用例可以在恒温器订阅传感器的属性**后实现。首先，用户将恒温器绑定到占用传感器。完成后，恒温器可以订阅传感器属性并定期以及在有活动（传感器属性发生变化）时接收数据。这种情况如下图所示：

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*-3jfMtoyO2kIyCpOolNw1A.jpeg)

这是订阅属性的示例。设备还可以**订阅事件**。在数据模型文章中，我们讨论了具有属性和命令的集群。在这里，我们介绍也是集群一部分的事件。事件捕获每一个更改并将其传达给订阅的实体。事件的一些示例包括按下开关、长按和门打开。





# 非matter设备

几个大型物联网生态系统已宣布集成支持 Matter，例如亚马逊、苹果、谷歌和三星。令人兴奋的是，越来越多来自多个品牌的设备在更大的 Matter 生态系统下本地协同工作。

同时，人们可能会有一个疑问：消费者家里已经有很多物联网产品，这些可以是基于Zigbee、Z-Wave、BLE Mesh等的设备。这些非Matter设备能否与Matter生态系统协同工作？答案是肯定的。

今天，我们来谈谈Matter 中的Bridge 功能。

## **物质桥简介**

Bridge 允许在 Matter 生态系统 (Matter Fabric) 中使用非 Matter IoT 设备。它使消费者能够继续将这些非 Matter 设备与 Matter 设备一起使用。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*IOfjWhJZ9QvSS_glb5DGcw.png)

物质桥

非 Matter 设备作为桥接设备暴露给 Matter 生态系统中的节点。Bridge设备执行Matter和其他协议之间的转换，以便Matter节点可以与Bridged设备进行通信。

下面是 Matter-Zigbee 桥的示例，它将两个 Zigbee 灯桥接至 Matter 生态系统：

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*JMliH3B14JTFUaGCZFaGjA.png)

Matter-Zigbee 桥

## **桥接数据模型**

在上一篇文章[Matter: Clusters, Attributes, Commands](https://blog.espressif.com/matter-clusters-attributes-commands-82b8ec1640a0)中，我们讨论了 Matter 数据模型，下面是 Matter Bridge 设备的数据模型示例。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*xUIoURpXd_10wZIvw7tZYQ.png)

物质桥设备数据模型

- 在端点 0 上，设备类型定义为桥接器。PartsList 字段列出了桥接设备的所有端点，每个端点代表桥接非 Matter 侧的一个设备。
- 每个端点上的描述符簇提供有关特定桥接设备的信息。

Bridge 还可能包含本机 Matter 功能，例如，它本身可能是具有 Wi-Fi 和 Zigbee 连接的智能恒温器。恒温器是 Matter 的原生功能，它能够向加热/冷却装置发送加热和/或冷却需求通知。而其他端点代表桥接设备。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*oRuvnusPkJDFLqmJCkbSZQ.png)

具有 Native Matter 功能的 Matter Bridge 设备

现在，让我们看看上一节提到的 Matter-Zigbee 桥示例。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*N0DUQ69Ed1-tH3YVgdsIVA.png)

Matter-Zigbee 桥工作流程

以下是使用 Matter 协议在手机上控制 Zigbee 设备的工作流程：

**步骤1。**Bridge 是 Matter 中定义的一种设备类型，应遵循标准 Matter 调试流程来加入 Matter 结构。

**第2步。**Matter-Zigbee 桥接设备也应加入 Zigbee 网络。与 Matter 有点不同的是，Zigbee 规范不强制要求任何标准的调试过程，而是由设备供应商决定分配链接密钥的工作流程。安装代码是自 Zigbee 3.0 以来最常用的方法。

**步骤3。**一旦桥接设备加入 Zigbee 网络，它将通过广播**匹配描述符请求**命令来发现 Zigbee 网络中支持的设备。该命令包括所需的配置文件、集群内和集群外。在此示例中，它会询问诸如“谁拥有带有 OnOff Cluster 的开关灯？”。相应的 Zigbee 设备将回复包含其网络地址的**匹配描述符响应。**对于每个匹配的 Zigbee Light，桥接器将在 Matter 中添加一个动态端点，它代表桥接 Zigbee 设备。

**步骤4。**Bridge 将所有桥接设备暴露给 Matter 结构，该结构遵循Matter 规范定义的**操作发现方法（***敬请关注即将推出的有关 Matter 中发现的系列*）。

**步骤5。**现在，Matter 结构中的控制器可以在 Bridge 的帮助下控制 Zigbee 网络中的灯光。

一些注意事项：

- 注1：Step-2和Step-3中的交互方法是由设备供应商和协议本身定义的，不在Matter范围内。
- 注2：桥接设备可以根据非Matter侧网络中的保活机制动态添加或删除。

这是桥接到 Zigbee 网络的典型工作流程。类似的概念将适用于我们要桥接的其他网络。

## **乐鑫 Matter Bridge 解决方案**

乐鑫作为从一开始就支持 Matter 的活跃成员，可以为具有 Wi-Fi 或 Thread 连接的终端产品以及使用 SoC 组合的 Thread 边界路由器和桥接解决方案提供全方位的 Matter 协议解决方案。

我们提供 Matter-Zigbee 和 Matter-BLE Mesh 网桥解决方案，并提供全功能软件 SDK 支持。Matter-Zigbee 桥使用两个 SoC (Wi-Fi + 802.15.4)，它们通过 UART 或 SPI 等串行接口连接，而 Matter-BLE 网桥可以在具有 Wi-Fi 和 BLE 的单个 SoC 上完成接口。

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*6TO13fzMLPs2Ia8b0Sil3g.png)

Espressif Matter-Zigbee 网桥解决方案

![img](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/1*r_5x4LnVjtckBmAiQbZzSg.png)

Espressif Matter-BLE Mesh 网桥解决方案

# 