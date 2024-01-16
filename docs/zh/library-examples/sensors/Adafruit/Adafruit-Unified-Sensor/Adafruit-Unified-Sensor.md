# Adafruit 传感器统一驱动库

## 1 简介

Adafruit 传感器统一驱动库即 Adafruit Unified Sensor 库， 旨在为在各种 Adafruit 传感器驱动库进行交互提供一个共同的接口。

- 提供通用的传感器接口，简化了与不同类型传感器的交互过程。
- 适用于多种平台，使得开发人员可以在不同的硬件环境中轻松地使用相同的传感器代码。
- 支持包括温度传感器、湿度传感器、加速度计等多种传感器。

### 1.1 仓库地址

- 官方仓库（Github）：https://github.com/adafruit/Adafruit_Sensor
- 镜像源（Gitee）：https://gitee.com/RT-Thread-Mirror/Adafruit_Sensor

### 1.2 自动依赖Arduino库

- 无

### 1.3 BSP要求

- 无

## 2 使能Adafruit 传感器统一驱动库

一般情况下，本库不需要单独使能，在使用 Adafruit 传感器驱动库时，Env会自动依赖本库，因此用户一般情况不需要自己手动使能。用户如需手动使能，可以参考以下方法：

```Kconfig
RT-Thread online packages --->
    Arduino libraries  --->
        Sensors  --->
            [*] Enable Arduino Sensor Libraries on RT-Thread Sensor Device Framework
```

## 3 示例

```c
#include <Adafruit_Sensor.h>

// 创建传感器对象
Adafruit_Sensor *sensor = new Adafruit_Sensor();

// 初始化传感器
sensor->begin();

// 读取传感器数值
float sensorValue = sensor->readSensor();

// 在此处可以根据需要使用传感器数值进行进一步处理
// ...
```
