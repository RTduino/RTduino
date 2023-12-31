# 使用VSCode开发RTduino

**待补充**

## 1 下载VSCode并安装

到[VSCode官网](https://code.visualstudio.com)下载VSCode并安装。

在RT-Thread 某个BSP板卡目录打开Env终端（例如PowerShell）使用 `scons --target=vsc` 生成VSCode工程

![target_vsc](figures/target_vsc.png)

在Env终端（例如PowerShell）输入 `code .` 基于当前路径来打开VSCode工程。

可以在VSCode的终端内直接敲入 `menuconfig`、`scons` 等命令，与上述在PowerShell终端下的教程一致。

> ⚠️注意：
> 
> 1. 在打开VSCode工程的时候，务必通过Env终端（例如PowerShell）去打开工程，因为RT-Thread的Env工具，会对系统的环境变量进行单独的配置，比如Env内部自带的python路径，编译工具链等一些重要环境变量，如果用户不使用此方式去打开VSCode工程，可能会因为环境变量缺失而引起一系列问题。
> 2. 在VSCode中使用menuconfig图形化界面时，上下方向键无效，需要使用数字小键盘区的加减号键代替。
> 3. 在执行完 `menuconfig` 重新配置之后，需要执行 `scons --target=vsc` 重新更新一下VSCode工程文件。