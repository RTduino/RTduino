用户可以直接将Arduino的库文件夹（或者是zip压缩文件）拖入此文件夹中，在生成工程时，Scons会自动识别到用户新加入的Arduino库，并将`src`、`utility` 以及根目录下的.c/.cpp文件以及.h/.hpp文件路径加入到工程中（只有 `src` 递归添加.c/cpp文件）。其行为与Arduino IDE/CLI的行为一致。

此外，也可以将Arduino库直接注册为RT-Thread软件包，无需在Arduino库仓库中增加Sconscript脚本，RTduino会自动在Env软件包下载文件夹的跟录下搜索，根据 `library.properties` 文件判定是否为Arduino库，并将其添加到工程中。

更多信息

The user can directly drag the Arduino library folder (or zip compressed file) into this folder, and when generating the project, Scons will automatically recognize the newly added Arduino library, and put `src`, `utility` and The .c/.cpp files and .h/.hpp file paths in the root directory are added to the project (only `src` recursively adds .c/.cpp files). Its behavior is consistent with that of the Arduino IDE/CLI.

In addition, the Arduino library can also be directly registered as an RT-Thread software package, without adding Sconscript scripts in the Arduino library warehouse, RTduino will automatically search under the directory of the Env software package download folder, and judge whether is a Arduino library according to the `library.properties` file exsits or not, then add the Arduino library into the project.

More information

https://arduino.github.io/arduino-cli/0.21/library-specification/#layout-of-folders-and-files
https://arduino.github.io/arduino-cli/0.33/sketch-build-process/#dependency-resolution
