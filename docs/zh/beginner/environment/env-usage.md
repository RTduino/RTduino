# Env工具的使用

## 编译BSP

到RT-Thread 代码托管平台Github或Gitee下载源代码。

这里以`stm32f411-st-nucleo`这个BSP、Windows 10操作系统环境为例，需要进入到 `rt-thread\bsp\stm32\stm32f411-st-nucleo` 文件夹下，按住Shift键+鼠标邮件，点击“在此处打开PowerShell窗口”。

### menuconfig -s 预设配置界面

如果这是你第一次使用Env，建议先键入命令 `menuconfig -s`，用于检查和设置软件包的下载行为：

``` Kconfig
Env config --->
    [*] Auto update pkgs config
        Select download server (Auto)  --->
    [ ] Auto create a Keil-MDK or IAR project
    [*] Send usage data for improve product
```

其中：

- `Auto update pkgs config`：退出 `menuconfig` 后是否自动下载选定的软件包，默认为选定
- `Select download server`：选择下载软件包时的服务器，默认为根据IP或时区自动确定下载服务器，也可以手动选择使用Gitee或Github服务器下载软件包。
- `Auto create a Keil-MDK or IAR project`：退出 `menuconfig` 界面后是否自动生成Keil-MDK/IAR工程，默认不选定。RTduino不涉及，无需理会。
- `Send usage data for improve product`：向RT-Thread官方发送统计数据，用于RT-Thread统计软件包使用情况，默认为选定。

### menuconfig 配置界面

在PowerShell终端下，键入 `menuconfig` 命令，并进入到图形化配置界面中。

在 `menuconfig` 界面中，操作功能键如下：

- 上下方向键，或者数字区小键盘的加减按键用于控制上下功能的选择
- 空格键用于选定（使能）某个功能
- 回车键用于进入到子菜单
- ESC用于返回上一级或者连续按ESC可以退出，退出前会提示是否保存当前配置，一般选择Yes。
- 字母H键为帮助建，用于查看某个选项的详细信息

在 `menuconfig` 界面中，选择使能RTduino：

```Kconfig
Hardware Drivers Config --->
    Onboard Peripheral Drivers --->
        [*] Compatible with Arduino Ecosystem (RTduino)
```

选择 `Compatible with Arduino Ecosystem (RTduino)` 选项后，直接连续按ESC键保存并退出，Env会自动使能本BSP中RTduino所依赖的相关硬件（如I2C、PWM等），自动下载RTduino软件包并将其纳入到该BSP的RT-Thread工程中。

### scons 编译

在软件包均下载完毕之后，即可通过 `scons -j20` 命令来编译工程（20表示20个核心并行编译，数字根据电脑硬件实际情况填写）。也可以使用 `scons -j12 --exec-path='xxxx/bin'` 来指定本次编译所使用的的工具链（路径填写至工具链路径的bin文件路径下）。

编译出的 `.bin` 以及 `.elf` 文件即可烧入到板卡中。以STM32为例，则使用STM32CubeProgrammer软件将bin或elf文件烧入到板卡中。不同半导体厂商提供的烧录软件不一样，详情需要参考半导体厂商相关资料。
