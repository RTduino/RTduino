# Arduino 社区库

Arduino 社区库是指由Arduino社区贡献的大量第三方库。目前，Arduino社区已经包含有超过5000个库，其中包括通信、传感器驱动、数据处理、数据存储、设备控制、显示驱动、输入输出驱动、时间相关等。这里包含个人贡献的库，也有例如Adafruit等开源公司贡献的库，这些库占Arduino库的主题，是Arduino社区生态的极重要组成部分。这也是设计RTduino的核心目的，即让RT-Thread可以直接兼容并运行Arduino库。

库和软件包其实是一个意思，只不过RT-Thread和Arduino两个社区叫法不一样：
- [软件包](https://packages.rt-thread.org)：英文为 software package，是指RT-Thread社区所属维护的第三方扩展，是RT-Thread原生生态一部分。
- [库](https://www.arduino.cc/reference/en/libraries)：英文为library，是指Arduino社区所属维护的第三方扩展，是Arduino原生生态一部分。


## 1 通过RT-Thread软件包中心导入Arduino社区库

RT-Thread软件包中心为Arduino社区库专门创建了一个分类（Arduino libraries），RTduino社区会将Arduino社区中一些常用的、重要的第三方库（如驱动库等）或需要部分修改方能适配RTduino的Arduino库注册到RT-Thread软件包中心中，用户可以通过Env工具一键化下载使用。

RTduino/RT-Thread官方一直在持续将优质的Arduino库注册到RT-Thread软件包中心，这样做的好处是：

- 借助RT-Thread Env完善的软件包管理机制，用户无需关心库是否依赖其他库，Env工具会自动依赖并下载；
- RT-Thread Env软件包管理有中国大陆镜像源，可以保证国内高速下载；
- 被注册到软件包中心的库都是经过验证过的。

下面AHT10温湿度传感器为例，讲解如何快速使用Arduino的AHT10温湿度传感器驱动：

1. 使用Env进入 `menuconfig` 后，先选择 `Compatible with Arduino Ecosystem (RTduino)`，让BSP具备兼容Arduino生态的能力：

```Kconfig
Hardware Drivers Config --->
    Onboard Peripheral Drivers --->
        [*] Compatible with Arduino Ecosystem (RTduino)
```

2. 点击Package栏，选择Arduino软件包分类，找到Sensor分类（Arduino传感器库分类），找到并选择Adafruit AHTx0驱动。此时，Env会自动选择该库依赖的其他Arduino库，例如Adafruit Unified Sensor库以及Adafruit BusIO库等：

``` Kconfig
RT-Thread online packages  --->
    Arduino libraries  --->
        Sensors  --->
            [*] Adafruit AHT10 & AHT20: Humidity and Temperature Sensor  ---->
```

3. 保存并退出 `menuconfig`，此时Env工具会自动下载一些列软件包，包括RTduino本身以及Adafruit AHTx0库及其依赖库。

4. 软件包和库下载完毕后，可以将Adafruit AHTx0库的例程（位于该库文件夹下的examples文件夹）直接复制到 `arduino_main.cpp` 文件下运行，你可以看到，串口会输出当前的温湿度，Arduino库的例程是直接可以在RTduino/RT-Thread环境上运行起来的。

## 2 手动导入Arduino社区库

已经注册到RT-Thread软件包中心的Arduino社区库只是一部分，还有很多Arduino社区库并没有被注册到软件包中心。如果用户想要使未被注册到RT-Thread软件包中心的Arduino库，可以采用手动导入的方式。

1. 到[Arduino的软件包分类中心](https://www.arduinolibraries.info)去查找想要的库，或者直接在Github上搜索你想要的库，一般都是C++类型的。下载源码，可以是zip文件，也可以是非压缩的源码文件夹。

2. 将zip文件或源码文件夹直接拖入到工程目录下的 `packages\RTduino-xxx\libraries\user` 文件夹内，重新使用 `scons -j12` 命令编译工程即可。编译时，scons系统会自动解压zip文件并将库纳入到工程并编译。
