# sketch文件的编译命令与编译方法

## 1 以命令行方式编译运行sketch文件

在 Arduino 中，编程源码文件被称之为 "sketch" (草稿之意)，其文件后缀为 `.ino`，具体 Arduino IDE 对 sketch 文件的编译方法，参见 [Arduino CLI文档](https://arduino.github.io/arduino-cli/0.33/sketch-build-process/) 。

RTduino支持对 `.ino` 文件的直接编译，方法很简单，使用命令 `scons -j20 --sketch=".ino文件的绝对路径"` 即可完成编译，其中 `-j20` 表示20个CPU并发编译（取决于计算机有多少个核心）以提高编译速度，`--sketch=""` 用于指定 `.ino` 文件的绝对路径。这样RTduino就会单独创建一个新的线程运行该sketch文件内的源码。

## 2 RTduino sketch loader 自动初始化机制

RTduino 是基于 RT-Thread 操作系统开发，因此支持多线程并发是浑然天成的，也是 Arduino 并不具备的。使用 RTduino，我们可以通过 **sketch loader** 创建**任意多个** sketch 源文件并以多线程的方式独立运行，互不干扰。

在 RTduino 中，可以使用以下4个宏其中之一来装填到 RTduino sketch loader中，只要该源文件添加到工程中并被编译，loader在板子上电之后会自动运行 sketch 源文件。sketch源文件需要将后缀改名为 `.cpp` 文件。

-  `RTDUINO_SKETCH_LOADER`：使用线程默认优先级（最低可用优先级）和默认栈大小（2048字节）
-  `RTDUINO_SKETCH_LOADER_PRIO`：使用默认线程栈大小，并重新设置线程优先级
-  `RTDUINO_SKETCH_LOADER_STACKSIZE`：使用默认线程优先级，并重新设置线程栈大小
-  `RTDUINO_SKETCH_LOADER_STACKSIZE_PRIO`：重新设置线程栈大小以及线程优先级

以下为示例：

```c
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
