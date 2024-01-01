# RTduino API 注意事项

### 1.1 pinMode()

PWM功能引脚不能调用 pinMode() 函数，否则PWM会失效。

ADC、DAC同理。

```c
void setup() {
  //pinMode(led_pin, OUTPUT); //不能设置为OUTPUT，否则PWM会失效
}
void loop() {
  //Fading the LED
  for(int i=0; i<255; i++){
    analogWrite(led_pin, i);
    delay(5);
  }
  for(int i=255; i>0; i--){
    analogWrite(led_pin, i);
    delay(5);
  }
}
```

因为底层已经将对应的PWM、ADC或DAC的IO设置为模拟输入或者复用推挽，调用pinMode之后把IO模式改成了纯输入输出，原有的PWM、ADC或DAC功能将无法使用。该问题无要修正，只需要知道调用analogRead和analogWrite的时候不需要设置pinMode即可。一旦调用pinMode，该引脚将丧失analogWrite或者analogRead功能，后续只能当做普通IO使用。
Arduino [官方文档](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/)也是这么建议的：

> You do not need to call pinMode() to set the pin as an output before calling analogWrite().
> The analogWrite function has nothing to do with the analog pins or the analogRead function.

用户如果对PWM、ADC或DAC引脚使用pinMode函数，在终端也会给出警告：

![pwmwarning](figures/pwm-warning.png)

当然，如果用户已经知道这样做的后果，但是故意需要将PWM、ADC或DAC引脚通过pinMode函数转为普通IO也是完全可以的。

### 1.2 Serial.begin()

在很多Arduino例程中，都喜欢使用如下语句来初始化串口：

```c
  Serial.begin(9600);
```

这句话将串口默认初始化成波特率为9600. 但是在RT-Thread中，串口的初始化实际是有RT-Thread驱动框架负责的，并且默认波特率为115200. 因此如果调用`Serial.begin(9600)` 函数后，串口的波特率将会从默认的115200调整为9600。如果你的终端或者串口助手还保持在115200的波特率，那么接收数据将出现乱码。

⚠️**因此建议：** 使用`Serial.begin()`代替`Serial.begin(9600)`。`Serial.begin()`无参数方法是RTduino的扩充方法，其表示跟随使用RT-Thread串口波特率配置，不重新配置串口波特率。

### 1.3 SPI.begin() / Wire.begin()

在操作SPI和Wire(I2C)时，默认调用的RT-Thread SPI和I2C设备在arduino_pin.h中定义，用户使用SPI和Wire库时，无需指定SPI和I2C设备，和使用Arduino没有任何区别。如果使用非默认的SPI/I2C时，只需要在初始化函数中传入对应的rt-thread设备名即可，如`SPI.begin("spi1")` 或 `Wire.begin("i2c1")`。

### 1.4 PWM与SPI功能复用

在Arduino UNO R3标准引脚布局中，D10-D13引脚为SPI引脚，但同时，D10与D11引脚也是PWM引脚，引发冲突。

在RTduino支持Arduino UNO R3标准引脚布局的BSP中，D10与D11默认优先支持PWM功能，当用户调用SPI.begin时，会自动将PWM功能重定向为SPI功能，重定向后，D10、D11引脚将无法再转回到PWM功能。
