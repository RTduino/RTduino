# RT-Thread操作系统的Arduino生态兼容层

### 如果喜欢请Star，这是对本开源项目最大的鼓励，谢谢；如果想要贡献PR，请fork

**中文** | [English](README_en.md)

--------

![RTduino](logo/RTduino-RT-Thread.png)

## 1 简介

RTduino是[RT-Thread实时操作系统](https://www.rt-thread.org)的Arduino生态兼容层，为[RT-Thread社区](https://github.com/RT-Thread/rt-thread)的子社区、Arduino开源项目的下游项目，旨在兼容Arduino社区生态来丰富RT-Thread社区软件包生态（如上千种分门别类的Arduino库，以及Arduino社区优秀的开源项目），并降低RT-Thread操作系统以及与RT-Thread适配的芯片的学习门槛。通过RTduino，可以让用户使用Arduino的函数、编程方法，轻松地将RT-Thread和BSP使用起来。用户也可以直接使用[Arduino第三方库](https://www.arduino.cc/reference/en/libraries/)（例如传感器驱动库、算法库等）直接用在RT-Thread工程中，极大地补充了RT-Thread社区生态。

本软件包可以在[RT-Thread Studio IDE](https://www.rt-thread.org/page/studio.html)和Keil-MDK编译环境下运行，但是，因为Arduino的库都是基于GCC环境开发的，因此**强烈推荐**使用[RT-Thread Studio IDE](https://www.rt-thread.org/page/studio.html)运行。

### 1.1 已经支持Arduino生态兼容层的RT-Thread BSP

| BSP名称                                                                                                                                     | BSP名称                                                                                                                                        |
| ----------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------- |
| [ES32F3696](https://github.com/RT-Thread/rt-thread/tree/master/bsp/essemi/es32f369x/applications/arduino_pinout)                          | [大疆STM32F427 RoboMaster A板](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f427-robomaster-a/applications/arduino_pinout) |
| [STM32F103 BluePill](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f103-blue-pill/applications/arduino_pinout)        | [大疆STM32F407 Robomaster C型](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f407-robomaster-c/applications/arduino_pinout) |
| [STM32F401 BlackPill](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f401-weact-blackpill/applications/arduino_pinout) | [STM32L431小熊派](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l431-BearPi/applications/arduino_pinout)                    |
| [STM32F411 BlackPill](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f411-weact-blackpill/applications/arduino_pinout) | [STM32L475潘多拉](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l475-atk-pandora/applications/arduino_pinout)               |
| [STM32F072 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f072-st-nucleo/applications/arduino_pinout)          | [CH32V307V-R1](https://github.com/RT-Thread/rt-thread/tree/master/bsp/wch/risc-v/ch32v307v-r1/applications/arduino_pinout)                   |
| [STM32F401 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f401-st-nucleo/applications/arduino_pinout)          | [CH32V208W-R0](https://github.com/RT-Thread/rt-thread/tree/master/bsp/wch/risc-v/ch32v208w-r0/applications/arduino_pinout)                   |
| [STM32F410 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f410-st-nucleo/applications/arduino_pinout)          | [NXP LPC55S69 EVK](https://github.com/RT-Thread/rt-thread/tree/master/bsp/lpc55sxx/lpc55s69_nxp_evk/applications/arduino_pinout)             |
| [STM32F411 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f411-st-nucleo/applications/arduino_pinout)          | [STM32L476 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l476-st-nucleo/applications/arduino_pinout)             |
| [STM32F412 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f412-st-nucleo/applications/arduino_pinout)          | [STM32G474 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32g474-st-nucleo/applications/arduino_pinout)             |
| [STM32F469 Discovery](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f469-st-disco/applications/arduino_pinout)        | [STM32U575 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32u575-st-nucleo/applications/arduino_pinout)             |

> 注：RTduino也可以无需适配特定BSP，直接运行在任意RT-Thread BSP上，请参考第5章-RTduino精简模式。

### 1.3 推荐初学者快速上手的板卡

| BSP名称                                                                                                                            | BSP名称                                                                                                                            |
| -------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------- |
| [STM32F401 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f401-st-nucleo/applications/arduino_pinout) | [STM32L476 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l476-st-nucleo/applications/arduino_pinout) |
| [STM32F411 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f411-st-nucleo/applications/arduino_pinout) |                                                                                                                                  |

### 1.4 术语说明

[软件包](https://packages.rt-thread.org)：英文为 software package，是指RT-Thread社区所属维护的第三方扩展，是RT-Thread原生生态一部分。

[库](https://www.arduino.cc/reference/en/libraries)：英文为library，是指Arduino社区所属维护的第三方扩展，是Arduino原生生态一部分。

>  库和软件包其实是一个意思，只不过两个社区叫法不一样。

## 2 如何使用RTduino

> RTduino 要求 RT-Thread 版本至少为4.1.1

### 2.1 参考资料

- [RTduino中文文档中心](./docs/zh/)
- [2022年RT-Thread全球开发者大会报告视频](https://www.bilibili.com/video/BV1Wa411L7B4)

### 2.2 工程的创建和导入

RTduino的工程创建分为两种：RTduino发行版 和 RTduino社区开发版本。

#### 2.2.1 RTduino社区开发版

RTduino社区开发版为社区开发的主线工程，使用最新RT-Thread和RTduino代码，支持更多的RT-Thread BSP，可以尝鲜RT-Thread和RTduino的最新功能。

有两种创建方法：

1. [使用VSCode+Env创建工程](docs/zh/6.使用VSCode+Env创建工程.md) (当前推荐这种创建方式)
2. [使用RT-Thread Studio IDE创建RTduino工程 (基于最新代码)](docs/zh/5.使用RT-Thread%20Studio%20IDE创建工程.md#51-rtduino社区开发版工程创建方法)

#### 2.2.2 RTduino发行版

**目前无RTduino发型版发布**

RTduino发行版为RT-Thread BSP经过验证，封装为RT-Thread Studio BSP包，用户可以直接通过RT-Thread Studio一键创建对应型号开发板的工程。

[使用RT-Thread Studio IDE创建RTduino工程 (基于发布版)](docs/zh/5.使用RT-Thread%20Studio%20IDE创建工程.md#52-rtduino发行版工程创建方法)

### 2.3 Arduino经典的setup和loop函数在哪里？

对于Arduino，最经典的莫过于setup和loop函数。这两个函数位于BSP目录下的applications文件夹内arduino_main.cpp文件中。以潘多拉板为例，这两个函数位于：`bsp/stm32/stm32l475-atk-pandora/applications/arduino_main.cpp`文件中，在开启RTduino软件包后，你可以直接在工程的Applications组中找到它。

![2.3-1](docs/figures/2.3-1.png)

### 2.4 点一个LED灯吧！

```c
#include <Arduino.h>

void setup(void)
{
    // put your setup code here, to run once:
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop(void)
{
    // put your main code here, to run repeatedly:
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.println("Hello Arduino!");
    delay(100);
}
```

可以看到，板载的LED灯已经开始闪烁，串口开始输出了。

> 注意：
> 
> 由于RT-Thread的main.c文件内，也会默认闪烁一个LED灯，如果板子上只有一个LED灯的话，两个线程会发生干涉。但是你会发现这个LED的闪烁速度明显变快了。因为main.c那边的闪烁周期是1000ms，上面这个例程是200ms。
> 
> 如果你用STM32L475潘多拉 BSP，main.c闪烁的是红灯，RTduino兼容层的Arduino程序默认闪烁的是绿色的灯，二者不会发生干扰。

### 2.5 RTduino文件夹目录结构

RTduino软件包包含有两个主要的文件夹：core和libraries。

![2.5-1](docs/figures/2.5-1.png)

- core文件夹主要是提供Arduino内置的所有的API函数，例如analogWrite、analogRead函数等等，这些函数可以在[Arduino官方](https://www.arduino.cc/reference/en/)找到详细的介绍。

- libraries文件夹是Arduino库所在文件夹。其中：
  
  - buildin文件夹下存放着Arduino内置的一些库，例如Servo舵机驱动库，Wire I2C驱动库等等；
  
  - user文件夹是用户文件夹，这是对用户来说很重要的一个文件夹，里边默认是空的，用户可以把下载好的Arduino库拖入到此文件夹中来，在下个章节会细说这个操作。

### 2.6 具体Arduino引脚分布信息

由于每个BSP的板子设计、以及芯片型号等，引脚分布是有区别的，因此需要到指定BSP的`applications/arduino`文件夹下的README.md文件查看详细信息。例如：

[STM32F401 Nucleo板的Arduino引脚布局的详细说明](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f401-st-nucleo/applications/arduino_pinout) | [STM32F411 Nucleo板的Arduino引脚布局的详细说明](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f411-st-nucleo/applications/arduino_pinout) | [STM32L475潘多拉板的Arduino引脚布局的详细说明](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l475-atk-pandora/applications/arduino_pinout)

## 3 Arduino库的使用

**注意：任何与Arduino（第三方）库相关的问题，请在本仓库开issue报告**

### 3.1 Arduino内置库

目前RTduino已经支持了大部分的Arduino[原生内建库](https://github.com/arduino/ArduinoCore-avr/tree/master/libraries)，存放于 `libraries/buildin` 文件夹内。具体如下表所示：

| 库名称       | 说明       | 使能宏                     | 备注                                                                                    |
| --------- | -------- | ----------------------- | ------------------------------------------------------------------------------------- |
| Servo     | 舵机库      | RTDUINO_USING_SERVO     | 所有支持PWM功能的BSP均会默认开启该库                                                                 |
| SPI       | SPI库     | RTDUINO_USING_SPI       | 所有支持SPI功能的BSP均会默认开启该库                                                                 |
| Wire      | I2C库     | RTDUINO_USING_WIRE      | 所有支持I2C功能的BSP均会默认开启该库                                                                 |
| USBSerial | USB虚拟串口库 | RTDUINO_USING_USBSERIAL | 按需求手动开启，自动依赖[TinyUSB for RT-Thread](https://github.com/RT-Thread-packages/tinyusb)软件包 |

### 3.2 通过RT-Thread软件包中心加载Arduino库到RT-Thread工程

参考文档：[通过RT-Thread软件包中心加载Arduino库到RT-Thread工程](docs/zh/7.通过RT-Thread软件包中心加载Arduino库到RT-Thread工程.md)

### 3.3 手动导入一个Arduino库到RT-Thread工程

上文提到的通过将Arduino第三方库注册到RT-Thread软件包中心，可以解决Arduino库之间的依赖问题以及中国大陆镜像加速的问题。但是，将Arduino第三方库注册到RT-Thread软件包中心是由RTduino社区维护者手动加载完成的，Arduino第三方库有将近5000个，无法都及时筛选并注册到RT-Thread软件包中心。那么，如果用户想要使用一个没有被注册到软件包中心的Arduino库该怎么办呢？RTduino也提供了可以让用户将其他Arduino库手动导入到RT-Thread工程中的方法，同样也非常方便快捷：

- 首先，你需要到Arduino官方的软件包分类中心去查找你想要的库，或者直接在Github上搜索你想要的库，一般都是C++类型的。

- 下载好之后，直接将zip压缩包（无需解压）拖进RTduino文件夹下的`libraries\user`这个目录下即可。

![3-3](docs/figures/3-3.png)

- 如果使用的是RT-Thread Studio IDE，请选择当前工程右键，并选择Sync Sconscript to project也就是让RT-Studio重新扫描并组织一遍工程目录，在扫描的过程中，RT-Studio会自动将zip压缩包解压，并按照Arduino IDE的文件添加逻辑（也就是忽略examples文件夹，并将其他文件夹的.c文件和.h路径添加到工程），将Arduino库添加到RT-Thread工程中来。然后再点一下小锤子按钮来重新编译一下工程。

- 如果使用的是Env + VSCode环境，请直接执行 `scons` 命令重新编译工程即可。

![3-4](docs/figures/3-4.png)

- 编译完成之后，你就会发现在RTduino的`libraries/user`文件夹下，就会出现你刚刚增加的Arduino库。

## 4 RTduino精简模式（快速使用，无需适配特定BSP）

### 4.1 不使用setup-loop编程模型

`setup()` 和 `loop()` 函数是Arduino编程中非常经典的函数，当你在Arduino IDE中新建一个文件时，默认就提供了这两个函数。这两个函数RTduino是完全支持的（参见第4章），但是一些较为复杂或庞大的业务逻辑如果放在setup-loop函数中就会受到一些束缚。因此，可以在Env或者RT-Thread Studio的RT-Thread Settings中取消setup-loop编程模型：

```Kconfig
RT-Thread online packages  --->
    system packages  --->
         [*] RTduino: Arduino Ecological Compatibility Layer  --->
              [*]   Don't use setup-loop structure  --->
```

选择此选项后，用户可以直接在 `core/arduino_thread.c` 中的 `arduino_entry` 线程函数中直接编程，或者在任意.cpp文件中调用Arduino API（不局限于Arduino线程，只要是.cpp文件下调用即可）。

### 4.2 如何不用定义引脚映射表，更方便的使用RTduino？

通过上文，我们可以看到，RTduino软件包并不是直接可以用的，需要RT-Thread BSP方面提供一些配套的支持，如引脚映射表（arduino_pinout）等。但是，如果用户不想使用Arduino引脚（IO）相关的API（如analogRead等），只想借助RTduino软件包，来直接兼容运行I2C芯片驱动库、纯软件算法库等和IO无关的函数和库，如何快速的使用起来呢？

用户可以直接在Env或者RT-Thread Studio的RT-Thread Settings中选择精简模式 (Enable tiny mode)。在选择精简模式后，用户就无需定义引脚映射表，直接就可以使用Arduino库中的非IO相关的函数和库了。开启精简模式后，会自动开启 5.1 章节所介绍的不使用setup-loop编程模型选项，用户可以在任意.cpp文件下使用Arduino API。

```Kconfig
RT-Thread online packages  --->
    system packages  --->
         [*] RTduino: Arduino Ecological Compatibility Layer  --->
              -*-   Don't use setup-loop structure
              [*]   Enable tiny mode  --->
```

### 4.3 常规模式（参见第4章）vs 精简模式（参见第5章）

在[Arduino API可兼容性一览表](docs/zh/1.Arduino%20API%E5%8F%AF%E5%85%BC%E5%AE%B9%E6%80%A7%E4%B8%80%E8%A7%88%E8%A1%A8.md)文档中，列举了在两种不同模式下，RTduino对Arduino API的兼容情况。

## 5 `.ino` sketch文件的编译命令与编译方法

### 5.1 以命令行方式编译运行 `.ino` 文件

在 Arduino 中，编程源码文件被称之为 "sketch" (草稿之意)，其文件后缀为 `.ino`，具体 Arduino IDE 对 sketch 文件的编译方法，参见 [Arduino CLI文档](https://arduino.github.io/arduino-cli/0.33/sketch-build-process/) 。

在RTduino中，我们也支持对 `.ino` 文件的直接编译，方法很简单，使用命令 `scons -j20 --sketch=".ino文件的绝对路径"` 即可完成编译，其中 `-j20` 表示20个CPU并发编译（取决于计算机有多少个核心）以提高编译速度，`--sketch=""` 用于指定 `.ino` 文件的绝对路径。这样RTduino就会单独创建一个新的线程运行该sketch文件内的源码。

### 5.2 RTduino sketch loader 自动初始化机制

RTduino 是基于 RT-Thread 操作系统开发，因此支持多线程并发是浑然天成的，也是 Arduino 并不具备的。使用 RTduino，我们可以通过 *sketch loader* 创建任意多个 sketch 源文件并以多线程的方式独立运行，互不干扰。

在 RTduino 中，可以使用宏 `RTDUINO_SKETCH_LOADER` 或 `RTDUINO_SKETCH_LOADER_PRIO` 或 `RTDUINO_SKETCH_LOADER_STACKSIZE` 或 `RTDUINO_SKETCH_LOADER_STACKSIZE_PRIO` 来装填到 RTduino sketch loader中，只要该源文件添加到工程中并被编译，loader在板子上电之后会自动运行 sketch 源文件。sketch源文件需要将后缀改名为 `.cpp` 文件。

以下为示例：

```cpp
#include <RTduino.h> /* 包含头文件，注意是<RTduino.h>，不是<Arduino.h> */

static void my_setup(void) /* static函数，不可命名为setup */
{
    Serial.println("Hello my sketch");
}

static void my_loop(void) /* static函数，不可命名为loop */
{

}

/* 
  以下四个自动初始化宏根据实际情况选择一个使用即可。
  四个宏的基本逻辑是一致的，即创建一个叫"my-sketch"的新容器（其实是一个新线程）
  并运行 my_setup() 和 my_loop() 函数。
  区别在于是否设置线程栈大小(STACKSIZE，默认为2048) 和 线程优先级(PRIO，默认为最低可用优先级)。
*/
RTDUINO_SKETCH_LOADER("my-sketch", my_setup, my_loop); /* 使用线程默认优先级和默认栈大小 */
RTDUINO_SKETCH_LOADER_PRIO("my-sketch", my_setup, my_loop, 10); /* 使用默认线程栈大小，并重新设置线程优先级为10 */
RTDUINO_SKETCH_LOADER_STACKSIZE("my-sketch", my_setup, my_loop, 1024); /* 使用默认线程优先级，并重新设置线程栈大小为1024 */ 
RTDUINO_SKETCH_LOADER_STACKSIZE_PRIO("my-sketch", my_setup, my_loop, 1024, 10); /* 重新设置线程栈大小为1024，线程优先级为10 */
```

参考示例：[MsgQ-C-CPP-demo](https://github.com/RTduino/MsgQ-C-CPP-demo/blob/master/arduino_producer.cpp)

## 6 需要注意的事项

### 6.1 PWM功能引脚不能调用 `pinMode` 函数，否则PWM会失效，ADC、DAC同理

```c
void setup() {
  //pinMode(led_pin, OUTPUT); //不能设置为OUTPUT，否则PWM会失效
}
void loop() {
  //Fading the LED
  for(int i=0; i<255; i++){
    analogWrite(led_pin, i);
    delay(5);
  }
  for(int i=255; i>0; i--){
    analogWrite(led_pin, i);
    delay(5);
  }
}
```

因为底层已经将对应的PWM、ADC或DAC的IO设置为模拟输入或者复用推挽，调用pinMode之后把IO模式改成了纯输入输出，原有的PWM、ADC或DAC功能将无法使用。该问题无要修正，只需要知道调用analogRead和analogWrite的时候不需要设置pinMode即可。一旦调用pinMode，该引脚将丧失analogWrite或者analogRead功能，后续只能当做普通IO使用。
Arduino [官方文档](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/)也是这么建议的：

```markdown
You do not need to call pinMode() to set the pin as an output before calling analogWrite().
The analogWrite function has nothing to do with the analog pins or the analogRead function.
```

用户如果对PWM、ADC或DAC引脚使用pinMode函数，在终端也会给出警告：

![pwmwarning](docs/figures/pwmwarning.png)

当然，如果用户已经知道这样做的后果，但是故意需要将PWM、ADC或DAC引脚通过pinMode函数转为普通IO也是完全可以的。

### 6.2 `Serial.begin()`

在很多Arduino例程中，都喜欢使用如下语句来初始化串口：

```c
  Serial.begin(9600);
```

这句话将串口默认初始化成波特率为9600. 但是在RT-Thread中，串口的初始化实际是有RT-Thread驱动框架负责的，并且默认波特率为115200. 因此如果调用`Serial.begin(9600)` 函数后，串口的波特率将会从默认的115200调整为9600。如果你的终端或者串口助手还保持在115200的波特率，那么接收数据将出现乱码。

**因此建议：** 使用`Serial.begin()`代替`Serial.begin(9600)`。`Serial.begin()`无参数方法是RTduino的扩充方法，其表示跟随使用RT-Thread串口波特率配置，不重新配置串口波特率。

### 6.3 `SPI.begin()` / `Wire.begin()`

在操作SPI和Wire(I2C)时，默认调用的RT-Thread SPI和I2C设备在arduino_pin.h中定义，用户使用SPI和Wire库时，无需指定SPI和I2C设备，和使用Arduino没有任何区别。如果使用非默认的SPI/I2C时，只需要在初始化函数中传入对应的rt-thread设备名即可，如`SPI.begin("spi1")` 或 `Wire.begin("i2c1")`。

### 6.4 PWM与SPI功能复用

在Arduino UNO R3标准引脚布局中，D10-D13引脚为SPI引脚，但同时，D10与D11引脚也是PWM引脚，引发冲突。

在RTduino支持Arduino UNO R3标准引脚布局的BSP中，D10与D11默认优先支持PWM功能，当用户调用SPI.begin时，会自动将PWM功能重定向为SPI功能，重定向后，D10、D11引脚将无法再转回到PWM功能。

在支持RTduino时，用户需要在BSP的 `pins_arduino.c` 文件中实现 `switchToSPI()` 函数，详见PR：https://github.com/RT-Thread/rt-thread/pull/7901 。

## 7 贡献与维护

### 7.1 项目仓库地址

https://github.com/RTduino/RTduino

https://gitee.com/rtduino/RTduino

### 7.2 如何为RT-Thread BSP适配RTduino

请参考文档: [如何为RT-Thread BSP适配RTduino](docs/zh/8.如何为RT-Thread%20BSP适配RTduino.md)

### 7.3 如何将某个Arduino库适配到RTduino

请参考文档：[如何将某个Arduino库适配到RTduino](docs/zh/9.如何将某个Arduino库适配到RTduino.md)

### 7.4 感谢以下小伙伴对本仓库的贡献

<a href="https://github.com/RTduino/rtduino/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=RTduino/rtduino" />
</a>
