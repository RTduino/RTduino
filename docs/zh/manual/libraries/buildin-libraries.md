# Arduino 内建库

目前 RTduino 已经支持了大部分的 Arduino [原生内建库](https://github.com/arduino/ArduinoCore-avr/tree/master/libraries)，存放于 `libraries/buildin` 文件夹内。具体如下表所示：

| 库名称       | 说明       | 使能宏                     | 备注                                                                                    |
| --------- | -------- | ----------------------- | ------------------------------------------------------------------------------------- |
| Servo     | 舵机库      | RTDUINO_USING_SERVO     | 所有支持PWM功能的BSP均会默认开启该库                                                                 |
| SPI       | SPI库     | RTDUINO_USING_SPI       | 所有支持SPI功能的BSP均会默认开启该库                                                                 |
| Wire      | I2C库     | RTDUINO_USING_WIRE      | 所有支持I2C功能的BSP均会默认开启该库                                                                 |
| USBSerial | USB虚拟串口库 | RTDUINO_USING_USBSERIAL | 按需求手动开启，自动依赖[TinyUSB for RT-Thread](https://github.com/RT-Thread-packages/tinyusb)软件包 |

