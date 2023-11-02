# RT-Thread Env 工具

Env 是 RT-Thread 推出的开发辅助工具，针对基于 RT-Thread 操作系统的项目工程，提供编译构建环境、图形化系统配置及软件包管理功能。

其内置的 menuconfig 图形化配置界面系统以及 scons 命令行系统，提供了简单易用的配置剪裁工具，可对内核、组件和软件包进行自由裁剪，使系统以搭积木的方式进行构建。

## 主要特性

- menuconfig 图形化配置界面，交互性好，操作逻辑强；
- 丰富的文字帮助说明，配置无需查阅文档；
- 使用灵活，自动处理依赖，功能开关彻底；
- 自动生成 rtconfig.h，无需手动修改；
- 使用 scons 工具生成工程，提供编译环境，操作简单；
- 提供多种软件包，模块化软件包耦合关联少，可维护性好；
- 软件包可在线下载，软件包持续集成，包可靠性高；

## Windows 10安装Env

对于Windows 10及以上的操作系统，可以直接利用其内置的Powershell终端作为平台。

#### 下载于安装

需要以**管理员身份**运行 PowerShell 来执行如下三行命令即可自动下载并安装Env。

对于身在中国大陆的用户，请使用如下命令（使用[Gitee镜像源](https://gitee.com/RT-Thread-Mirror/env)下载）：

``` shell
wget https://gitee.com/RT-Thread-Mirror/env/raw/master/install_windows.ps1 -O install_windows.ps1
set-executionpolicy remotesigned
.\install_windows.ps1 --gitee
```

对于身在中国大陆之外的用户，请使用如下命令（使用[Github开发源](https://github.com/RT-Thread/env)下载）：

``` shell
wget https://raw.githubusercontent.com/RT-Thread/env/master/install_windows.ps1 -O install_windows.ps1
set-executionpolicy remotesigned
.\install_windows.ps1
```

#### 环境变量设置

打开 `C:\Users\user\Documents\WindowsPowerShell`，如果没有`WindowsPowerShell`则新建该文件夹。新建文件 `Microsoft.PowerShell_profile.ps1`，然后写入 `~/.env/env.ps1` 命令保存并重启 Powershell，`Microsoft.PowerShell_profile.ps1` 文件内的命令将会在每次启动 Powershell 终端时，均会自动初始化Env环境变量，无需手动初始化。

在Windows Powershell终端下默认不安装任何工具链，因此如果想要用 `scons` 命令编译工程以及能使用 QEMU，则需要再添加工具链路径和 QEMU 路径，例如：

``` shell
~/.env/env.ps1
$env:RTT_EXEC_PATH="your toolchain path (e.g. C:\xxx\bin)"
$env:path="your QEMU path;$env:path"
```

> 注：
> Windows Powershell平台在执行上述Env下载、安装命令时，不会自动安装工具链，因此要根据板卡的型号自行下载、安装并向Env指定工具链的位置。
> 例如，`arm-none-eabi-gcc` 的[下载地址](https://developer.arm.com/downloads/-/gnu-rm)

#### 注意

1. Powershell 要以管理员身份打开；
2. 将其设置为 remotesigned 后，您可以作为普通用户运行 PowerShell；
3. 一定要关闭杀毒软件，否则安装过程可能会被杀毒软件强退；


## Windows 7安装Env

Windows 7操作系统使用第三方终端ConEmu作为平台。RT-Thread基于ConEmu终端集成了所有Env功能，下载最新发布版本（.7z压缩包）并解压即可使用。

下载地址：https://github.com/RT-Thread/env-windows/releases

具体安装方法参见[RT-Thread文档中心](https://www.rt-thread.org/document/site/#/development-tools/env/env?id=env-%e7%9a%84%e4%bd%bf%e7%94%a8%e6%96%b9%e6%b3%95)。

> 注：
> Env-ConEmu 版本会内置QEMU以及 `arm-none-eabi-gcc`，编译ARM平台的BSP时可以直接 `scons` 编译。无需额外设置工具链的环境变量。


## Ubuntu安装Env

对于Ubuntu操作系统，可以直接利用其内置的终端作为平台。

#### 下载与安装

打开终端，并执行如下三行命令即可自动下载并安装Env。

对于身在中国大陆的用户，请使用如下命令（使用[Gitee镜像源](https://gitee.com/RT-Thread-Mirror/env)拉取）：

``` shell
wget https://gitee.com/RT-Thread-Mirror/env/raw/master/install_ubuntu.sh
chmod 777 install_ubuntu.sh
./install_ubuntu.sh --gitee
```

对于身在中国大陆之外的用户，请使用如下命令（使用[Github开发源](https://github.com/RT-Thread/env)拉取）：

``` shell
wget https://raw.githubusercontent.com/RT-Thread/env/master/install_ubuntu.sh
chmod 777 install_ubuntu.sh
./install_ubuntu.sh
```

#### 环境变量设置

打开 `~/.bashrc` 文件，将 `source ~/.env/env.sh` 这行命令附加在改文件末尾，保存并退出。这样，每次启动终端时，均会自动初始化Env环境变量，无需手动初始化。

> 注：
> Ubuntu终端窗口下的Env在执行上述下载、安装命令时会自动安装 `arm-none-eabi-gcc` 工具链。
