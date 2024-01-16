# Adafruit BusIO库

## 1 简介

Adafruit BusIO 库是一个用于处理嵌入式系统通信协议的轻量级库，主要支持I2C和SPI协议。该库提供了简单而灵活的抽象层，旨在与Adafruit的传感器、显示器等设备紧密配合使用。

### 1.1 仓库地址

Adafruit BusIO库是专门针对RTduino/RT-Thread适配过的，RTduino对官方上游仓库有部分修改。

- 官方上游仓库（Github）：https://github.com/adafruit/Adafruit_BusIO
- RTduino适配仓库（Github）：https://github.com/RTduino-libraries/Adafruit_BusIO
- 镜像源（Gitee）：https://gitee.com/RT-Thread-Mirror/Adafruit_BusIO

### 1.2 自动依赖Arduino库

- 无

### 1.3 BSP要求

- 已对接RT-Thread I2C设备驱动框架
- 已对接RTduino Wire(I2C)
- 已对接RT-Thread SPI设备驱动框架
- 已对接RTduino SPI

> 注：
>
> 如果没有对接SPI或I2C，Adafruit BusIO 库对应的SPI或I2C功能将不可使用，但是不会影响编译。

## 2 使能Adafruit BusIO库

一般情况下，Adafruit BusIO库不需要单独使能，在使用 Adafruit 传感器驱动库时（如 Adafruit AHTX0 库），Env会自动依赖Adafruit BusIO库，因此用户一般情况不需要自己手动使能。用户如需手动使能，可以参考以下方法：

```Kconfig
RT-Thread online packages --->
    Arduino libraries  --->
        Signal IO  --->
            [*] Adafruit BusIO: abstract away I2C & SPI transactions and registers
```

## 3 功能与API介绍

### 3.1 I2C总线

Adafruit BusIO库通过 `Adafruit_I2CDevice` 类提供了对I2C通信的支持。通过实例化一个I2C设备对象，您可以轻松进行读写操作。例如：

```c
#include <Wire.h>
#include <Adafruit_BusIO.h>

Adafruit_I2CDevice myDevice(0x42, &Wire);

void setup() {
  Wire.begin();
}

void loop() {
  uint8_t data;
  myDevice.readByte(0x01, &data);
}
```

#### 3.1.1 创建对象

创建 `Adafruit_I2CDevice` 对象时，需要提供以下参数：

- I2C地址 (`address`)：指定I2C设备的地址。这是一个7位或8位的地址，取决于设备规范。
- Wire对象 (`wire`)：指定I2C通信所使用的`Wire`对象，负责底层的I2C通信。例如，`&Wire`表示使用Arduino的Wire库。

#### 3.1.2 方法

`Adafruit_I2CDevice` 类提供了一系列方法，用于进行I2C通信：

- `readBytes(reg, buffer, size)`：从I2C设备的指定寄存器读取指定数量的字节到缓冲区。
- `writeBytes(reg, buffer, size)`：向I2C设备的指定寄存器写入缓冲区中的指定数量的字节。
- `readByte(reg, value)`：从I2C设备的指定寄存器读取一个字节的数据。
- `writeByte(reg, value)`：向I2C设备的指定寄存器写入一个字节的数据。

### 3.2 SPI总线

Adafruit BusIO库通过 `Adafruit_SPIDevice` 类提供了对SPI通信的支持。通过创建一个SPI设备对象，您可以进行SPI通信。以下是一个简单的示例代码：

```c
#include <SPI.h>
#include <Adafruit_BusIO.h>

// 创建一个SPI设备对象，参数分别为CS引脚、SPI模式、时钟速率
Adafruit_SPIDevice myDevice(10, SPI_MODE0, 1000000);

void setup() {
  // 初始化SPI总线
  SPI.begin();
}

void loop() {
  // 读取一个字节的数据
  uint8_t data;
  myDevice.readBytes(&data, 1);
}
```

#### 3.2.1 创建对象

创建 `Adafruit_SPIDevice` 对象时，需要提供以下参数：

- CS引脚 (`csPin`)：指定SPI设备的片选引脚。这是一个数字引脚，用于选择要与通信的特定设备。

- SPI模式 (`spiMode`)：定义SPI通信的模式，影响数据传输的时序和极性。可选的模式有：
  - `SPI_MODE0`：时钟悬空低，数据采样在时钟的第一个边沿。
  - `SPI_MODE1`：时钟悬空低，数据采样在时钟的第二个边沿。
  - `SPI_MODE2`：时钟悬空高，数据采样在时钟的第一个边沿。
  - `SPI_MODE3`：时钟悬空高，数据采样在时钟的第二个边沿。

- 时钟速率 (`bitRate`)：设置SPI总线的时钟速率，以赫兹为单位。这决定了每秒钟传输的位数。例如，`1000000`表示1兆赫兹。

#### 3.2.2 方法

`Adafruit_SPIDevice` 类提供了一系列方法，用于进行SPI通信：

- `readBytes(buffer, size)`：从SPI设备读取指定数量的字节到缓冲区。
- `writeBytes(buffer, size)`：向SPI设备写入缓冲区中的指定数量的字节。
