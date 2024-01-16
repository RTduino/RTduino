# 电容性触摸传感库

## 1 简介

本节介绍如何使用 CapacitiveSensor 电容性触摸传感库实现触摸开关的设计。

### 1.1 仓库地址

- 官方（Github）：https://github.com/PaulStoffregen/CapacitiveSensor
- 镜像源（Gitee）：https://gitee.com/RT-Thread-Mirror/CapacitiveSensor

### 1.2 自动依赖Arduino库

- 无

### 1.3 BSP要求

- 已对接RT-Thread ADC设备驱动框架
- 已对接RT-Thread PIN设备驱动框架
- 已对接RTduino基本引脚分布表(pinout)数字部分，支持函数DigitalRead()/DigitalWrite()
- 已对接RTduino基本引脚分布表(pinout)模拟输入部分，支持函数AnalogRead()

## 2 CapacitiveSensor库工作原理


## 3 如何运行CapacitiveSensor库

### 3.1 开启RTduino

使用Env进入 `menuconfig` 后，先选择 `Compatible with Arduino Ecosystem (RTduino)`，让BSP具备兼容Arduino生态的能力：

```Kconfig
Hardware Drivers Config --->
    Onboard Peripheral Drivers --->
        [*] Compatible with Arduino Ecosystem (RTduino)
```

### 3.2 开启CapacitiveSensor库

CapacitiveSensor库已经注册到RT-Thread软件包中心：

```Kconfig
RT-Thread online packages --->
    Arduino libraries  --->
        Sensors  --->
            [*] Capacitive Sensor: detect touch or proximity
```

### 3.3 硬件连接

### 3.4 编写程序

### 3.5 编译运行

