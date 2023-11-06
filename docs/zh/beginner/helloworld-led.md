# 点亮一颗LED吧！

本节以 `stm32f411-st-nucleo` BSP、Windows 10操作系统环境为例。

## Arduino经典的setup和loop函数在哪里？

对于Arduino，最经典的莫过于setup和loop函数。这两个函数位于BSP目录下的 `applications` 文件夹内 `arduino_main.cpp` 文件中。以STM32F411 Nucleo板为例，这两个函数位于：`bsp/stm32/stm32f411-st-nucleo/applications/arduino_main.cpp` 文件中，在开启RTduino软件包后，你可以直接在工程的Applications组中找到它。

## 点亮LED并通过串口向外输出

将 `arduino_main.cpp` 中的内容全部用以下内容代替：

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
    Serial.println("Hello RTduino!");
    delay(100);
}
```

编译并下载到板卡中后，就可以看到，板载的LED已经开始闪烁，并且串口开始输出 "Hello RTduino!" 了。

> 注意：
> 
> 由于RT-Thread的main.c文件内，也会默认闪烁一个LED灯，如果板子上只有一个LED灯的话，两个线程会发生干涉。但是你会发现这个LED的闪烁速度明显变快了。因为main.c那边的闪烁周期是1000ms，上面这个例程是200ms。
> 
