# RTduino精简模式

## 1 不使用setup-loop编程模型

`setup()` 和 `loop()` 函数是Arduino编程中非常经典的函数，当你在Arduino IDE中新建一个文件时，默认就提供了这两个函数。这两个函数RTduino是完全支持的，但是一些较为复杂或庞大的业务逻辑如果放在setup-loop函数中就会受到一些束缚。因此，可以在 Env 的 `menuconfig` 配置中取消setup-loop编程模型：

```Kconfig
RT-Thread online packages  --->
    system packages  --->
         [*] RTduino: Arduino Ecological Compatibility Layer  --->
              [*]   Don't use setup-loop structure  --->
```

选择此选项后，用户可以直接在 `core/arduino_thread.c` 中的 `arduino_entry` 线程函数中直接编程，或者在任意.cpp文件中调用Arduino API（不局限于Arduino线程，只要是.cpp文件下调用即可）。

## 2 RTduino 精简模式

RTduino软件包并不是直接可以用的，需要RT-Thread BSP方面提供一些配套的支持，如引脚映射表（arduino_pinout）等（参见[如何为 RT-Thread BSP 适配 RTduino](/zh/manual/adapt/bsp/bsp.md)章节）。但是，如果用户不想使用Arduino引脚（IO）相关的API（如analogRead等），只想借助RTduino软件包，来直接兼容运行I2C芯片驱动库、纯软件算法库等和IO无关的函数和库，如何快速的使用起来呢？

用户可以直接在 Env 的 `menuconfig` 配置中选择精简模式 (Enable tiny mode)。在选择精简模式后，用户就无需定义引脚映射表，直接就可以使用Arduino库中的非IO相关的函数和库了。开启精简模式后，会自动开启 5.1 章节所介绍的不使用setup-loop编程模型选项，用户可以在任意.cpp文件下使用Arduino API。

```Kconfig
RT-Thread online packages  --->
    system packages  --->
         [*] RTduino: Arduino Ecological Compatibility Layer  --->
              -*-   Don't use setup-loop structure
              [*]   Enable tiny mode  --->
```

## 3 常规模式 vs 精简模式

在[Arduino API可兼容性一览表](/zh/manual/api/api-compatibility)文档中，列举了在两种不同模式下，RTduino对Arduino API的兼容情况。
