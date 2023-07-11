# matter环境配置方法

## 安装esp-idf

这步骤在前面已经实现了



## 基本环境准备

```
sudo apt-get install git gcc g++ pkg-config libssl-dev libdbus-1-dev \
     libglib2.0-dev libavahi-client-dev ninja-build python3-venv python3-dev \
     python3-pip unzip libgirepository1.0-dev libcairo2-dev libreadline-dev

```



## 打开idf

切换到esp-idf的目录

```
source ./export.sh
```

![image-20230710171418208](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230710171418208.png)



## 切出该目录，拉取matter

```
cd ..
git clone --recursive https://github.com/espressif/esp-matter.git
```

![image-20230710171519638](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230710171519638.png)

这个步骤需要较长的时间。省时间可以选择浅拷贝，正常步骤需要2个小时左右。

```c
git clone --depth 1 https://github.com/espressif/esp-matter.git
cd esp-matter
git submodule update --init --depth 1
cd ./connectedhomeip/connectedhomeip
./scripts/checkout_submodules.py --platform esp32 linux --shallow
cd ../..
./install.sh
cd ..   
```



## 编译并且激活matter

在esp-matter文件夹中进行操作

```
cd ./connectedhomeip/connectedhomeip
source scripts/activate.sh
```

![image-20230710183529006](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230710183529006.png)

![image-20230711103855871](../.config/Typora/typora-user-images/image-20230711103855871.png)