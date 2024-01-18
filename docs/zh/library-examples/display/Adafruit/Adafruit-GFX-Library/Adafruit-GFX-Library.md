# Adafruit GFX图形库

## 1 Adafruit GFX 图形库简介

Adafruit GFX Library 是一套开源的 C/C++ 图形库，专门用于驱动各种图形液晶显示屏（LCD）。该库的设计理念是为开发者提供简单而高效的方式来在图形液晶显示屏上创建和显示各种图形。Adafruit GFX Library 的 API 经过精心设计，使得开发者可以轻松地控制LCD屏幕的各种特性和参数，从而在有限的时间内实现更多的创意和功能。

### 1.1 仓库地址

Adafruit GFX 库是专门针对 RTduino/RT-Thread 环境适配过的，对官方上游仓库有部分修改。

- 官方上游仓库（Github）：https://github.com/adafruit/Adafruit-GFX-Library
- RTduino适配仓库（Github）：https://github.com/RTduino-libraries/Adafruit-GFX-Library
- 镜像源（Gitee）：https://gitee.com/RT-Thread-Mirror/Adafruit-GFX-Library

### 1.2 自动依赖Arduino库

- [Adafruit BusIO](/zh/library-examples/signal-io/Adafruit/Adafruit-BusIO/Adafruit-BusIO)

### 1.3 BSP要求

- 已对接RT-Thread I2C设备驱动框架
- 已对接RT-Thread SPI设备驱动框架
- 已对接RTduino Wire(I2C)
- 已对接RTduino SPI

## 2 使能Adafruit GFX库

一般情况下，Adafruit GFX库不需要单独使能，在使用Adafruit屏幕驱动库时（如Adafruit ST7735库），Env会自动依赖Adafruit GFX库，因此用户一般情况不需要自己手动使能。用户如需手动使能，可以参考以下方法：

使用Env进入 `menuconfig` ：

```Kconfig
RT-Thread online packages --->
    Arduino libraries  --->
        Display  --->
             [*]  Adafruit-GFX-Library: Adafruit GFX graphics core library
```

## 3 Adafruit GFX Library API详细介绍及使用示例

以下是Adafruit GFX Library中一些常用的API函数及其详细说明和示例代码：

### 3.1 基本函数

#### 3.1.1 初始化函数

* `begin()`: 此函数用于初始化LCD屏幕，并设置SPI通信的速率。

示例代码：
```c
#include <Adafruit_GFX.h>
Adafruit_ST7735 lcd = Adafruit_ST7735(/* 参数 */);
lcd.begin(); // 初始化LCD屏幕
```
* `setRotation()`: 此函数用于设置LCD的显示方向。

示例代码：
```c
lcd.setRotation(1); // 参数为0、1、2或3，对应0度、90度、180度和270度旋转
```

#### 3.1.2 绘图函数

* `fillScreen()`: 此函数用于填充整个屏幕背景色。

示例代码：
```c
lcd.fillScreen( Adafruit_ST7735::RED ); // 使用红色填充屏幕
```
* `drawPixel()`: 此函数用于在指定位置绘制一个像素点。

示例代码：
```c
lcd.drawPixel(10, 10, Adafruit_ST7735::BLUE); // 在指定位置绘制像素点
```
* `drawLine()`: 此函数用于绘制一条线段。

示例代码：
```c
lcd.drawLine(0, 0, 10, 10, Adafruit_ST7735::RED); // 绘制线段
```
* `drawRect()`: 此函数用于绘制一个矩形。

示例代码：
```c
lcd.drawRect(0, 0, 100, 100, Adafruit_GFX::RED); // 在 (0, 0) 位置绘制一个 100x100 像素的红色矩形
```

#### 3.1.3 图像处理函数

* `drawBitmap()`: 此函数用于在LCD屏幕上绘制一个位图图像。

示例代码：
```c
// 首先确保位图图像已经加载到数组中
uint8_t bitmapImage[] = { /* 位图数据 */ };

// 然后调用drawBitmap函数来绘制图像
lcd.drawBitmap(bitmapImage, x, y, color, transparent, scaling);
```
参数说明：
- `bitmapImage`: 位图图像数组，包含位图数据。
- `x`: 位图的左上角x坐标。
- `y`: 位图的左上角y坐标。
- `color`: 位图的颜色，如果位图透明则颜色设置为透明。
- `transparent`: 是否透明，如果是透明位图，则为true。
- `scaling`: 是否缩放图像，如果缩放，则为缩放比例。

#### 3.1.4 其他常用函数

* `setCursor()`: 此函数用于设置光标位置，以便在LCD上绘制文本或图像。

示例代码：
```c
lcd.setCursor(x, y); // 设置光标位置为(x, y)
```
* `setTextSize()`: 此函数用于设置文本的大小。

示例代码：
```c
lcd.setTextSize(size); // 设置文本大小为指定值
```
* `setTextColor()`: 此函数用于设置文本的颜色。

示例代码：
```c
lcd.setTextColor(Adafruit_ST7735::WHITE); // 设置文本颜色为白色
```
* `setTextWrap()`: 此函数用于设置文本是否自动换行。

示例代码：
```c
lcd.setTextWrap(true); // 开启文本自动换行功能
```
* `drawChar()`: 此函数用于在LCD上绘制一个字符。

示例代码：
```c
lcd.drawChar(x, y, 'A', Adafruit_ST7735::BLACK, Adafruit_ST7735::WHITE); // 在指定位置绘制字符'A'
```
参数说明：
- `x`, `y`: 字符的左上角坐标。
- `c`: 要绘制的字符。
- `bg`: 字符的背景颜色。
- `fg`: 字符的前景色。
* `drawString()`: 此函数用于在LCD上绘制一段文本。

示例代码：
```c
lcd.drawString("Hello, world!", x, y, color); // 在指定位置绘制一段文本"Hello, world!"，颜色为指定颜色color。
```

### 3.2 高级函数

#### 3.2.1 实用辅助函数

* `invertDisplay()`: 此函数用于反转LCD屏幕的显示颜色。

示例代码：
```c
lcd.invertDisplay(true); // 开启颜色反转
```
* `pushColor()`: 此函数用于将一个颜色值推入颜色缓冲区。

示例代码：
```c
lcd.pushColor(Adafruit_ST7735::RED); // 将红色推入颜色缓冲区
```
* `popColor()`: 此函数用于从颜色缓冲区弹出一个颜色值。

示例代码：
```c
lcd.popColor(); // 从颜色缓冲区弹出一个颜色值
```

#### 3.2.2 颜色管理

Adafruit GFX Library支持多种颜色格式，包括RGB，HSV等。你可以使用`setColor()`, `getPixelColor()` 和 `clearBuffer()` 等函数来操作颜色。

示例代码：
```c
// 设置颜色格式为RGB
lcd.setColorMode(COLOR_565_RGB);

// 设置颜色值
lcd.setColor(0xFFFF); // 红色

// 获取指定像素的颜色值
uint16_t color = lcd.getPixelColor(x, y);

// 清空颜色缓冲区
lcd.clearBuffer();
```
#### 3.2.3 触摸屏支持

如果你正在使用带有触摸屏的设备，Adafruit GFX Library 也提供了相关API来处理触摸输入。你可以使用`touchStart()`, `touchEnd()`, `touchRead()`等函数来读取触摸屏的状态。

示例代码：
```c
// 开始触摸屏事件处理
lcd.touchStart();

// 结束触摸屏事件处理
lcd.touchEnd();

// 读取触摸屏状态，返回触摸点的位置和压力值
int16_t x, y, pressure;
lcd.touchRead(&x, &y, &pressure);
```
#### 3.2.4 动画和过渡效果

Adafruit GFX Library也支持一些动画和过渡效果，例如渐变和淡入淡出。你可以使用`drawPixelAnimation()`, `drawLineAnimation()`, `fillScreenAnimation()`等函数来实现这些效果。

示例代码：
```c
// 创建一个渐变动画对象，从红色渐变到蓝色，持续时间为100毫秒
Adafruit_GFX::gradient_t gradient = Adafruit_GFX::gradient_make(Adafruit_GFX::RED, Adafruit_GFX::BLUE);
lcd.drawPixelAnimation(&gradient, x, y, 100); // 在(x, y)位置绘制渐变动画
```
