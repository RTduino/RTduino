# Arduino Ecosystem Compatibility Layer for RT-Thread

### Please Star this project if you like it. Thank you!

**English** | [中文](README_zh.md)

--------

![RTduino](logo/RTduino.png)

## 1 Introduction

RTduino is the Arduino ecosystem compatibility layer for [RT-Thread RTOS](https://www.rt-thread.io). RTduino is the sub-community of [RT-Thread community](https://github.com/RT-Thread/rt-thread) and the downstream project of Arduino. RTduino is an open source project which is compatible with Arduino APIs so that RT-Thread beginners can easily get start to use RT-Thread through Arduino APIs, which significantly reduces the difficulty of learning RT-Thread. Meanwhile, RT-Thread users also can directly run thousands of [Arduino third party libraries](https://www.arduino.cc/reference/en/libraries/) on RT-Thread by using RTduino.

Using the RTduino and Arduino libraries will be very easy through the [RT-Thread Studio](https://www.rt-thread.io/studio.html) integrate development environment (IDE) with GUI configurations.

### 1.1 RT-Thread BSPs which have supported RTduino

| BSP List                                                                                                                            |
| ----------------------------------------------------------------------------------------------------------------------------------- |
| [STM32F103 BluePill](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f103-blue-pill/applications/arduino_pinout)  |
| [STM32F072 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f072-st-nucleo/applications/arduino_pinout)    |
| [STM32F401 Nucleo](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f401-st-nucleo/applications/arduino_pinout)    |
| [STM32F469 Discovery](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f469-st-disco/applications/arduino_pinout)  |
| [STM32L475 Pandora](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32l475-atk-pandora/applications/arduino_pinout) |
| [ES32F3696](https://github.com/RT-Thread/rt-thread/tree/master/bsp/essemi/es32f369x/applications/arduino_pinout)                    |

> RTduino also allows users to directly run without a specific BSP supporting. Please see Chapter 5

### 1.2 Terms Explanation

[Software Package](https://packages.rt-thread.org/en/index.html): is RT-Thread side third party extension, and belongs to RT-Thread ecosystem.

[Library](https://www.arduino.cc/reference/en/libraries): is Arduino side third party extension, and belongs to Arduino ecosystem.

> Software package and library are the same meaning, but different names for RT-Thread and Arduino communities.

## 2 How to use RTduino

We will use [STM32F103 BluePill BSP](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f103-blue-pill) as an example to show how to use RTduino.

> RTduino requires the minimum version of RT-Thread is 4.1.1

### 2.1 References

- [How to import a BSP project with latest code into RT-Thread Studio](https://www.youtube.com/watch?v=fREPLuh-h8k)

### 2.2 Import and create a project

- Please go to the [RT-Thread repository](https://github.com/RT-Thread/rt-thread) to [download](https://github.com/RT-Thread/rt-thread/archive/refs/heads/master.zip) the latest code

- Download and install [RT-Thread Studio IDE](https://www.rt-thread.io/studio.html)

- Unzip the RT-Thread source code file and open RT-Thread Studio.

![2.2-1](docs/figures/2.2-1.png)

- In RT-Thread Studio IDE, select `File` -> `Import`, and select `RT-Thread BSP Project into Workspace`

![2.2-2](docs/figures/2.2-2.png)

![2.2-3](docs/figures/2.2-3.png)

- Click `Browse` button and select the Blue Pill BSP folder: `rt-thread\bsp\stm32\stm32f103-blue-pill`. [This video](https://www.youtube.com/watch?v=fREPLuh-h8k) also teaches you how to import a BSP project into RT-Thread Studio. Then, click `Finish` button to let RT-Thread Studio to import the Blue Pill BSP project.

![2.2-4](docs/figures/2.2-4-bluepill.png)

- When importing finished, please click `RT-Thread Settings`. Then, click `<<` button to show the configuration details.

![2.2-5](docs/figures/2.2-5.png)

![2.2-6](docs/figures/2.2-6.png)

- Click `Hardware`, and select `Support Arduino`. Then, click the "hammer" button to compile the project. RT-Thread Studio will automatically download the RTduino and other dependency software packages and compile the whole project.

![2.2-7](docs/figures/2.2-7.png)

- Up to now, this project has supported Arduino programming.

### 2.3 Where is the setup-loop framework

Now, you have successfully create a RT-Thread Blue Pill Board project and allow you to directly use Arduino APIs to drive this board. However,  where is the setup-loop framework, which is very common seen in an Arduino sketch?

Actually, the Arduino sketch is located in `bsp/stm32/stm32f103-blue-pill/applications/arduino_main.cpp`, where is in the `application` folder.

![2.3-1](docs/figures/2.3-1.png)

### 2.4 Try to blink a LED!

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

You will see the LED is blinking and the serial terminal also shows "Hello Arduino!".

There are many other examples and demos at [example folder](https://github.com/RTduino/RTduino/tree/master/examples)

### 2.5 RTduino folder directory structure

![2.5-1](docs/figures/2.5-1.png)

There are two main folders in RTduino: core and libraries.

- [core folder](https://github.com/RTduino/RTduino/tree/master/core): contains all Arduino related APIs will be implemented in this folder, such as: digitalRead, analogWrite and so on.
- [libraries folder](https://github.com/RTduino/RTduino/tree/master/libraries)
  - [buildin folder](https://github.com/RTduino/RTduino/tree/master/libraries/buildin): contains Arduino build-in libraries such as SPI, Wire (I2C), servo and so on.
  - [user folder](https://github.com/RTduino/RTduino/tree/master/libraries/user): this folder is empty by default and it prepares for users to import the Arduino Third Parity libraries. This operation will introduce and explain in the next chapter.

### 2.6 BSP Arduino IO Pinout

You will find more information related a specific BSP Arduino pinout at: `applications/arduino` folder. For Blue Pill BSP, it is located at [here](https://github.com/RT-Thread/rt-thread/tree/master/bsp/stm32/stm32f103-blue-pill/applications/arduino_pinout).

## 3 Arduino Libraries

### 3.1 Arduino Build-in Libraries

RTduino has supported most of [Arduino build-in libraries](https://github.com/arduino/ArduinoCore-avr/tree/master/libraries), where is in the folder `libraries/buildin`. The following table will show the details:

| Library   | Enable Macro            | Note                                                                                                                                                          |
| --------- | ----------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Servo     | RTDUINO_USING_SERVO     | Enable by default if PWM is available.库                                                                                                                       |
| SPI       | RTDUINO_USING_SPI       | Enable by default if SPI is available.中                                                                                                                       |
| Wire      | RTDUINO_USING_WIRE      | Enable by default if I2C is available.库                                                                                                                       |
| USBSerial | RTDUINO_USING_USBSERIAL | Enable by default if USB is available, and reply on the [TinyUSB for RT-Thread](https://github.com/RT-Thread-packages/tinyusb) software package automatically |

### 3.2 Enable Arduino Third Party libraries through RT-Thread software center

In RT-Thread software center, there is a specific category for Arduino. RTduino community will register some important and frequent to use Arduino third party libraries to RT-Thread software package center, so that users can directly use those libraries with GUI configuration in RT-Thread Studio.

The following example will show how to enable Arduino Adafruit AHTx0 sensor driver library with a few clicks:

- Please follow the previous chapter to import a project into RT-Thread Studio and select `Support Arduino` in RT-Thread Settings.

- Please to go `Software` column, and select `Arduino libraries` category, and extend `Sensors` sub-category of Arduino library. Then, select and enable the Adafruit AHTx0 Arduino driver library. RT-Thread Studio will help you to enable other dependency libraries, such as Adafruit Unified Sensor library, Adafruit BusIO library and so on.

![3-1](docs/figures/3-1.png)

- Click hammer to compile the project. RT-Thread Studio will automatically download the software packages and libraries and compile the whole project.

![3-2](docs/figures/3-2.png)

- When the compiling finishes, you can copy the example code of AHTx0 library, where is in the example folder, and paste it into `arduino_main.cpp` file. Then, compile the project again and download the program into the board. You will see the current sensor data will show on the terminal. Now, we have successfully directly run Arduino library on RT-Thread.

### 3.3 Import Third Party Arduino Libraries Manually

You also can directly import an Arduino library manually, which has not been registered into RT-Thread software package center. This operation also is very easy.

- You need to download the library from github or other repository and directly drag the ZIP file into `libraries\user` folder, and you don't need to decompress the ZIP file.

![3-3](docs/figures/3-3.png)

- Then, go to RT-Thread Studio, right click the project, select `Sync Sconscript to project` and click hammer to compile the project again.

![3-4](docs/figures/3-4.png)

- When the compiling finishes, you will find the new library is shown on the project, where is in the `libraries\user` folder of the project group.
