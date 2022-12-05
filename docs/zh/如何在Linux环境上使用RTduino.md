# 如何在Linux环境上使用RTduino

## 1 rt-thread 环境搭建

这里大家可以参考RT-Thread官方env仓库的README.md文件安装开发环境：[Linux开发环境搭建教程](https://github.com/RT-Thread/env#install-env)。

这里以中国国内用户为例：
```sh
wget https://gitee.com/RT-Thread-Mirror/env/raw/master/install_ubuntu.sh
chmod 777 install_ubuntu.sh
./install_ubuntu.sh --gitee
```
通过这三行命令，我们就可以一键化配置RT-Thread开发环境，并且会为我们获取一份rt-thread的最新源码，位于`~/`目录下。

## 2 stm32f103-blue-pill开发板RTduino使用示例

### 2.1 生成一份dist文件

```sh
cd rt-thread/bsp/stm32/stm32f103-blue-pill
scons --dist
```
这份dist里面的文件是可以移动到电脑的任何一个目录的，很方便。我这里把它移动到桌面继续下面的流程。

### 2.2 编译工程

#### 2.2.1 配置编译环境

- 导出编译器的路径到环境变量，scons工具才能正常使用。
- 配置env的环境，才能使用`pkgs --update`命令
```sh
export RTT_EXEC_PATH=/usr/bin
source ~/.env/env.sh
```
这两条命令大家也可以根据自己的情况添加到`~/.bashrc`这个文件里面。

#### 2.2.2 配置menuconfig

```sh
cd ~/Desktop/stm32f103-blue-pill
scons --menuconfig
```
在menuconfig界面根据以下路径选择
```Kconfig
Hardware Drivers Config --->
    Onboard Peripheral Drivers --->
        [*] Compatible with Arduino Ecosystem (RTduino)
```
> 注意，由于`RTduino`软件包会使用到`perf_counter`软件包，但是scons工具在编译的时候，会因为这个软件包而发生一些错误，原因不详。所以这里我介绍两种成功编译的方法。

#### 2.2.3 方法一：让工程不包含 perf_counter 软件包

menuconfig 取消勾选 `micros() function with high precision`
```Kconfig
RT-Thread online packages --->
    Arduino libraries --->
        RTduino: Arduino Ecological Compatibility Layer --->
            [ ]   micros() function with high precision
```
执行下列两条命令编译项目：
```sh
pkgs --update
scons
```
这样，就可以编译成功，并且生成可烧写的二进制文件。
```sh
LINK rt-thread.elf
arm-none-eabi-objcopy -O binary rt-thread.elf rtthread.bin
arm-none-eabi-size rt-thread.elf
   text	   data	    bss	    dec	    hex	filename
  82212	   1852	   5688	  89752	  15e98	rt-thread.elf
scons: done building targets.
```

#### 2.2.4 方法二：生成cmake工程进行编译

```sh
pkgs --update
scons --target=cmake
cd build
cmake ..
make
```
这样，就可以编译成功，并且生成可烧写的二进制文件。
```sh
[100%] Linking CXX executable rtthread.elf
   text	   data	    bss	    dec	    hex	filename
  83032	   1860	   5728	  90620	  161fc	rtthread.elf
[100%] Built target rtthread.elf
```

### 2.3 下载工程

我们已经编译出二进制文件，接下来就是烧写进单片机了，这里介绍一下st-link驱动的安装。如果是其他烧写方式，大家自行查阅资料。

#### 2.3.1 安装st-link驱动
```sh
cd ~
sudo apt-get install libusb-1.0
sudo apt-get install cmake
sudo apt-get install libgtk-3-dev
sudo apt-get install autoconf automake libtool
git clone https://github.com/texane/stlink stlink.git
cd stlink.git
make 
make release 
make debug
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cd Release
sudo make install
sudo ldconfig
sudo cp ~/stlink.git/config/udev/rules.d/49-stlinkv2.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
st-info –version
```
如果出现版本号，就表示安装成功了。

#### 2.3.2 烧写bin文件

进入rtthread.bin所在文件夹
```sh
st-flash write rtthread.bin  0x8000000
```
出现下面内容，表示下载成功：
```sh
2022-12-05T13:45:59 INFO flashloader.c: Starting Flash write for F2/F4/F7/L4
2022-12-05T13:45:59 INFO flash_loader.c: Successfully loaded flash loader in sram
2022-12-05T13:45:59 INFO flash_loader.c: Clear DFSR
2022-12-05T13:46:01 INFO common_flash.c: Starting verification of write complete
2022-12-05T13:46:02 INFO common_flash.c: Flash written and verified! jolly good!
```
