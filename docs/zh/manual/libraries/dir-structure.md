# RTduino文件夹目录结构

RTduino软件包包含有两个主要的文件夹：*core* 和 *libraries*。

![dir-structure](figures/dir-structure.png)

- core文件夹主要是提供Arduino内置的所有的API函数，例如 `analogWrite`、`analogRead` 函数等等，这些函数可以在[Arduino官方](https://www.arduino.cc/reference/en/)找到详细的介绍，也可以参考[Arduino API可兼容性一览表](/zh/manual/api/api-compatibility)。

- libraries文件夹是Arduino库所在文件夹。其中：
  
  - buildin文件夹下存放着一些Arduino内置库，例如Servo舵机驱动库，Wire(I2C) 驱动库等等；
  
  - user文件夹是用户文件夹，这是对用户来说很重要的一个文件夹，里边默认是空的，用户可以把下载好的Arduino社区库拖入到此文件夹中来。参见[手动导入Arduino社区库](/zh/manual/libraries/community-libraries?id=手动导入arduino社区库)。
