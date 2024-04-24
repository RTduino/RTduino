**中文** | [English](README_en.md)

--------

<p align="center">
	<img alt="logo" src="figures/logo/RTduino-RT-Thread.png" width="600">
</p>
<h1 align="center" style="margin: 30px 0 30px; font-weight: bold;">RTduino<br/>RT-Thread操作系统的Arduino生态兼容层</h1>
<h3 align="center">RTduino<br/>Arduino Ecosystem Compatibility Layer for RT-Thread</h4>
<p align="center">
	<a href="https://gitee.com/rtduino/RTduino/stargazers"><img src="https://gitee.com/rtduino/RTduino/badge/star.svg?theme=gvp"></a>
	<a href="https://gitee.com/rtduino/RTduino/members"><img src="https://gitee.com/rtduino/RTduino/badge/fork.svg?theme=gvp"></a>
	<a href="https://github.com/RTduino/RTduino/stargazers"><img src="https://img.shields.io/github/stars/RTduino/RTduino?style=flat-square&logo=GitHub"></a>
	<a href="https://github.com/RTduino/RTduino/network/members"><img src="https://img.shields.io/github/forks/RTduino/RTduino?style=flat-square&logo=GitHub"></a>
	<a href="https://github.com/RTduino/RTduino/watchers"><img src="https://img.shields.io/github/watchers/RTduino/RTduino?style=flat-square&logo=GitHub"></a>
	<a href="https://github.com/RTduino/RTduino/issues"><img src="https://img.shields.io/github/issues/RTduino/RTduino.svg?style=flat-square&logo=GitHub"></a>
</p>

## 1 简介

RTduino是[RT-Thread实时操作系统](https://www.rt-thread.org)的Arduino生态兼容层，为[RT-Thread社区](https://github.com/RT-Thread/rt-thread)的子社区，旨在兼容Arduino社区生态来丰富RT-Thread社区软件包生态（如上千种分门别类的Arduino库，以及Arduino社区优秀的开源项目），并降低RT-Thread操作系统以及与RT-Thread适配的芯片的学习门槛。通过RTduino，可以让用户使用Arduino的函数、编程方法，轻松地将RT-Thread和BSP使用起来。用户也可以直接使用[Arduino社区第三方库](https://www.arduino.cc/reference/en/libraries/)（例如传感器驱动库、算法库等）直接用在RT-Thread工程中，极大地补充了RT-Thread社区生态。

![framework-zh](./figures/rtduino-framework-zh.png)

## 2 已经适配RTduino的RT-Thread BSP

| BSP名称                     | 源码链接                                                                                                                                                                                                                                                         | DigitalWrite & Read | AnalogWrite / PWM | AnalogRead / ADC | 串口  | I2C | SPI |
| ------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ------------------- | ----------------- | ---------------- | --- | --- | --- |
| 瑞萨 RA6M3 HMI              | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/renesas/ra6m3-hmi-board/board/rtduino/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/renesas/ra6m3-hmi-board/board/rtduino/arduino_pinout)                   | √                   | √                 | √                | √   | √   | √   |
| NXP LPC55S69 EVK          | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/nxp/lpc/lpc55sxx/lpc55s69_nxp_evk/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/nxp/lpc/lpc55sxx/lpc55s69_nxp_evk/applications/arduino_pinout) | √                   | √                 | √                | √   | √   | ×   |
| STM32F407星火一号             | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f407-rt-spark/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f407-rt-spark/applications/arduino_pinout)                   | √                   | √                 | √                | √   | √   | √   |
| Raspberry Pi Pico         | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/raspberry-pico/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/raspberry-pico/applications/arduino_pinout)                                       | √                   | √                 | √                | √   | √   | √   |
| STM32F072 Nucleo          | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f072-st-nucleo/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f072-st-nucleo/applications/arduino_pinout)                 | √                   | √                 | √                | √   | √   | √   |
| STM32F401 Nucleo          | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f401-st-nucleo/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f401-st-nucleo/applications/arduino_pinout)                 | √                   | √                 | √                | √   | √   | √   |
| STM32F410 Nucleo          | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f410-st-nucleo/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f410-st-nucleo/applications/arduino_pinout)                 | √                   | √                 | √                | √   | √   | ×   |
| STM32F411 Nucleo          | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f411-st-nucleo/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f411-st-nucleo/applications/arduino_pinout)                 | √                   | √                 | √                | √   | √   | √   |
| STM32F412 Nucleo          | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f412-st-nucleo/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f412-st-nucleo/applications/arduino_pinout)                 | √                   | √                 | √                | √   | √   | √   |
| STM32L476 Nucleo          | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l476-st-nucleo/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32l476-st-nucleo/applications/arduino_pinout)                 | √                   | √                 | √                | √   | √   | √   |
| STM32G474 Nucleo          | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32g474-st-nucleo/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32g474-st-nucleo/applications/arduino_pinout)                 | √                   | √                 | √                | √   | √   | √   |
| STM32U575 Nucleo          | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32u575-st-nucleo/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32u575-st-nucleo/applications/arduino_pinout)                 | √                   | √                 | √                | √   | √   | ×   |
| STM32F469 Discovery       | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f469-st-disco/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f469-st-disco/applications/arduino_pinout)                   | √                   | √                 | √                | √   | √   | √   |
| STM32F103 BluePill        | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f103-blue-pill/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f103-blue-pill/applications/arduino_pinout)                 | √                   | √                 | √                | √   | √   | √   |
| STM32F401 BlackPill       | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f401-weact-blackpill/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f401-weact-blackpill/applications/arduino_pinout)     | √                   | √                 | √                | √   | √   | √   |
| STM32F411 BlackPill       | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f411-weact-blackpill/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f411-weact-blackpill/applications/arduino_pinout)     | √                   | √                 | √                | √   | √   | √   |
| STM32L475潘多拉              | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l475-atk-pandora/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32l475-atk-pandora/applications/arduino_pinout)             | √                   | √                 | √                | √   | √   | √   |
| STM32L431小熊派              | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l431-BearPi/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32l431-BearPi/applications/arduino_pinout)                       | √                   | √                 | √                | √   | √   | √   |
| 大疆STM32F427 RoboMaster A板 | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f427-robomaster-a/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f427-robomaster-a/applications/arduino_pinout)           | √                   | √                 | √                | √   | √   | ×   |
| 大疆STM32F407 Robomaster C型 | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f407-robomaster-c/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/stm32/stm32f407-robomaster-c/applications/arduino_pinout)           | √                   | √                 | √                | √   | √   | ×   |
| CH32V307V-R1              | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/wch/risc-v/ch32v307v-r1/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/wch/risc-v/ch32v307v-r1/applications/arduino_pinou)                      | √                   | √                 | √                | √   | √   | ×   |
| CH32V208W-R0              | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/wch/risc-v/ch32v208w-r0/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/wch/risc-v/ch32v208w-r0/applications/arduino_pinout)                     | √                   | √                 | √                | √   | √   | ×   |
| ES32F3696                 | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/essemi/es32f369x/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/essemi/es32f369x/applications/arduino_pinou)                                    | √                   | √                 | √                | √   | √   | √   |
| ES32VF2264                | [Github](https://github.com/RT-Thread/rt-thread/tree/master/bsp/essemi/es32vf2264/applications/arduino_pinout) / [Gitee](https://gitee.com/rtthread/rt-thread/tree/master/bsp/essemi/es32vf2264/applications/arduino_pinout)                                 | √                   | √                 | √                | √   | √   | √   |

## 3 开发环境

RTduino 支持以下开发环境:

- GCC
- VSCode
- RT-Thread Studio
- Keil MDK5
- Keil MDK6

> 注：
> RTduino作为RT-Thread软件包，其本身支持GCC工具链以及Keil AC5、AC6集成开发环境（IDE），但由于Arduino社区第三方库均为GCC工具链下编写，因此建议使用基于GCC工具链的开发环境。

## 4 官网与代码仓库

- 官网: http://www.rtduino.com/
- Github代码仓库: https://github.com/RTduino/RTduino
- Gitee代码仓库: https://gitee.com/rtduino/RTduino
- 文档中心（全球）：https://docs.rtduino.com
- 文档中心（国内）：https://rtduino.gitee.io/docs

## 5 感谢以下小伙伴对本仓库的贡献

<a href="https://github.com/RTduino/rtduino/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=RTduino/rtduino" />
</a>
