# RTduino线程间通信示例

## 1 简介

MsgQ-C-CPP-demo（RTduino线程间通信示例）软件包作为一个示例展示了如何使用 RT-Thread 消息队列 (message queue) 进行线程间通信，将基于 RTduino 运行的 AHT10 温湿度传感器驱动库获取的温湿度数据数据发给 RT-Thread 另一个线程，进而在另一个线程中对温湿度数据做进一步处理（例如，将数据打印到串口终端）。

### 1.1 仓库地址

- 官方（Github）：https://github.com/RTduino-libraries/MsgQ-C-CPP-demo
- 镜像源（Gitee）：https://gitee.com/RT-Thread-Mirror/MsgQ-C-CPP-demo

### 1.2 自动依赖Arduino库

- [Adafruit AHTX0 温湿度传感器驱动库](/zh/library-examples/sensors/Adafruit/Adafruit-AHTX0/Adafruit-AHTX0)

> 注：
>
> 是否依赖取决于是否选择 `Use Adafruit AHT10/AHT20 sensor data in the demo` 选项。

### 1.3 BSP要求

- 已对接 RT-Thread I2C 设备驱动框架
- 已对接 RTduino Wire(I2C)

> 注：
>
> 是有有上述 BSP 要求取决于是否选择 `Use Adafruit AHT10/AHT20 sensor data in the demo` 选项。

## 2 如何运行本示例

本节以**具有 Arduino UNO 标准引脚布局**的BSP为例，讲解如何运行 Adafruit AHTx0 驱动库。

### 3.1 开启RTduino

在 BSP 根目录下目录下，进入 `menuconfig` 后，先选择 `Compatible with Arduino Ecosystem (RTduino)`，开启 RTduino，让 BSP 具备兼容 Arduino 生态的能力：

```Kconfig
Hardware Drivers Config --->
    Onboard Peripheral Drivers --->
        [*] Compatible with Arduino Ecosystem (RTduino)
```

### 3.2 开启

RTduino 线程间通信示例软件包已经注册到 RT-Thread 软件包中心：

```Kconfig
RT-Thread online packages --->
    Arduino libraries  --->
        Projects and Demos  --->
            [*] Message Queue demo: communicate between Arduino sketches and RT-Thread typcial C threads  --->
                [*] Use Adafruit AHT10/AHT20 sensor data in the demo
```

其中 `Use Adafruit AHT10/AHT20 sensor data in the demo` 是否选定取决于你是否希望获取真实的温湿度数据。如果你没有 AHT10 传感器，可以不使能，线程之间将传送一个假的温湿度数据。但是依然可以直接体验并观察 RTduino 是如何使用消息队列来进行线程间通信的。

### 3.3 硬件连接

参考 [Adafruit AHTX0 温湿度传感器驱动库](/zh/library-examples/sensors/Adafruit/Adafruit-AHTX0/Adafruit-AHTX0)文档进行硬件连接。


### 3.4 编译运行

用 `scons -j12` 命令编译，并将 `.bin` 或 `.elf` 文件烧录到板卡中。

板卡上电后打开串口终端，调整接收波特率为115200（RT-Thread 默认波特率），可以看到，RTduino 获取的 AHT10 温湿度数据在另一个线程中持续打印输出到串口终端上。
