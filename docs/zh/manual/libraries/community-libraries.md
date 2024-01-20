# Arduino 社区库

Arduino 社区库是指由 Arduino 社区贡献的大量第三方库。目前，Arduino 社区已经包含有超过5000个库，其中包括通信、传感器驱动、数据处理、数据存储、设备控制、显示驱动、输入输出驱动、时间相关等。这里包含个人贡献的库，也有例如 Adafruit 等开源公司贡献的库，是 Arduino 社区生态的极重要组成部分。这也是设计 RTduino 的核心目的，即让 RT-Thread 可以直接兼容并运行 Arduino 库。

库和软件包其实是一个意思，只不过 RT-Thread 和 Arduino 两个社区叫法不一样：
- [软件包](https://packages.rt-thread.org)：英文为 software package，是指 RT-Thread 社区所属维护的第三方扩展，是 RT-Thread 原生生态一部分。
- [库](https://www.arduino.cc/reference/en/libraries)：英文为 library，是指 Arduino 社区所属维护的第三方扩展，是 Arduino 原生生态一部分。

## 1 通过RT-Thread软件包中心导入Arduino社区库

RT-Thread 软件包中心为 Arduino 社区库专门创建了一个分类 (Arduino libraries)，RTduino 社区会将 Arduino 社区中一些常用的、重要的第三方库（如驱动库等）或需要部分修改方能适配 RTduino 的 Arduino 库注册到 RT-Thread 软件包中心中，用户可以通过 Env 工具一键化下载使用。

RTduino/RT-Thread 官方一直在持续将优质的 Arduino 库注册到 RT-Thread 软件包中心，这样做的好处是：

- 借助 RT-Thread Env 工具完善的软件包管理机制，用户无需关心库是否依赖其他库，Env 工具会自动依赖并下载；
- RT-Thread Env 工具的软件包管理有中国大陆镜像源，可以保证中国大陆高速下载；
- 被注册到软件包中心的库都是经过验证过的。

具体使用方法和流程请参见 [Arduino 库使用实例](/zh/library-examples/README)。

## 2 手动导入Arduino社区库

已经注册到 RT-Thread 软件包中心的 Arduino 社区库只是一部分，还有很多 Arduino 社区库并没有被注册到软件包中心。如果用户想要使未被注册到 RT-Thread 软件包中心的 Arduino 库，可以采用手动导入的方式。

1. 到 [Arduino 的软件包分类中心](https://www.arduinolibraries.info)去查找想要的库，或者直接在 Github 上搜索你想要的库，一般都是 C++ 类型的。下载源码，可以是 zip 文件，也可以是非压缩的源码文件夹。

2. 将 zip 文件或源码文件夹直接拖入到工程目录下的 `packages\RTduino-xxx\libraries\user` 文件夹内，重新使用 `scons -j12` 命令编译工程即可。编译时，scons 系统会自动解压 zip 文件并将库纳入到工程并编译。

## 3 导入Arduino库的自动处理细节

用户将 Arduino 的库文件夹（或者是 zip 压缩文件）拖入 `packages\RTduino-xxx\libraries\user` 文件夹内后，scons 会自动识别到用户新加入的 Arduino 库，并将 `src`、`utility` 以及根目录下的 `.c/.cpp` 文件以及 `.h/.hpp` 文件路径加入到工程中（只有 `src` 递归添加 `.c/cpp` 文件）。其行为与 Arduino IDE/CLI 的行为一致。

此外，也可以将 Arduino 库直接注册为 RT-Thread 软件包，无需在 Arduino 库仓库中增加 Sconscript 脚本，RTduino 会自动在 Env 软件包下载文件夹的跟录下搜索，根据 `library.properties` 文件判定是否为 Arduino 库，并将其添加到工程中。

Arduino IDE/CLI 的库包含逻辑参见：

- https://arduino.github.io/arduino-cli/0.21/library-specification/#layout-of-folders-and-files
- https://arduino.github.io/arduino-cli/0.33/sketch-build-process/#dependency-resolution
