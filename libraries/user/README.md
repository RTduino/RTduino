用户可以直接将Arduino的库文件夹（或者是zip压缩文件）拖入此文件夹中，在生成工程时，Scons会自动识别到用户新加入的Arduino库，并将除了examples文件夹之外的.c/.cpp文件以及.h/.hpp文件路径加入到工程中。其行为与Arduino IDE的行为一致。

此外，也可以将Arduino库直接注册为RT-Thread软件包，无需在Arduino库仓库中增加Sconscript脚本，RTduino会自动在Env软件包下载文件夹的跟录下搜索，根据'library.properties'文件判定是否为Arduino库，并将其添加到工程中。

更多信息

https://arduino.github.io/arduino-cli/0.21/sketch-build-process/
