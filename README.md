# Arduino Ecosystem Compatibility Layer for RT-Thread

### Please Star this project if you like it. Thank you!

**English** | [中文](README_zh.md)

--------

![RTduino](logo/RTduino.png)

## 1 Introduction

RTduino is the Arduino ecosystem compatibility layer for [RT-Thread RTOS](https://www.rt-thread.io). RTduino is the sub-community of [RT-Thread community](https://github.com/RT-Thread/rt-thread) and the downstream project of Arduino. RTduino is an open source project which is compatible with Arduino APIs so that RT-Thread beginners can easily get start to use RT-Thread through Arduino APIs, which significantly reduces the difficulty of learning RT-Thread. Meanwhile, RT-Thread users also can directly run thousands of Arduino libraries on RT-Thread by using RTduino.

Using the RTduino and Arduino libraries will be very easy through the [RT-Thread Studio](https://www.rt-thread.io/studio.html) integrate development environment (IDE) with GUI configurations.

### 1.1 RT-Thread BSPs which have supported RTduino

| BSP List                                                                                                                            |
| ----------------------------------------------------------------------------------------------------------------------------------- |
| [STM32L475 Pandora](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l475-atk-pandora/applications/arduino_pinout) |
| [STM32F072 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f072-st-nucleo/applications/arduino_pinout)    |
| [STM32F401 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f401-st-nucleo/applications/arduino_pinout)    |
| [STM32F469 Discovery](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f469-st-disco/applications/arduino_pinout)  |
| [STM32F103 BluePill](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f103-blue-pill/applications/arduino_pinout)  |
| [ES32F3696](https://github.com/RT-Thread/rt-thread/tree/master/bsp/essemi/es32f369x/applications/arduino_pinout)                    |

> RTduino also allows users to directly run without a specific BSP supporting. Please see Chapter 5

## 2 How to use RTduino

We will use [STM32F103 BluePill BSP](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f103-blue-pill/applications/arduino_pinout) as an example to show how to use RTduino.

> RTduino requires the minimum version of RT-Thread is 4.1.1

### 2.1 References

- [How to import a BSP project with latest code into RT-Thread Studio](https://www.youtube.com/watch?v=fREPLuh-h8k)]

### 2.2 Import and create a project

- Please go to the [RT-Thread repository](https://github.com/RT-Thread/rt-thread) to [download](https://github.com/RT-Thread/rt-thread/archive/refs/heads/master.zip) the latest code

- Download and install [RT-Thread Studio IDE](https://www.rt-thread.io/studio.html)

- Unzip the RT-Thread source code file and open RT-Thread Studio.

![2.2-1](docs/figures/2.2-1.png)

- In RT-Thread Studio IDE, select `File` -> `Import`, and select `RT-Thread BSP Project into Workspace`

![2.2-2](docs/figures/2.2-2.png)

![2.2-3](docs/figures/2.2-3.png)
