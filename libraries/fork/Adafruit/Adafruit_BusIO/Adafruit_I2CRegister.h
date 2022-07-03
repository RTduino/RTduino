#ifndef _ADAFRUIT_I2C_REGISTER_H_
#define _ADAFRUIT_I2C_REGISTER_H_

#include <Arduino.h>

#ifdef RTDUINO_USING_WIRE
#include <Adafruit_BusIO_Register.h>

typedef Adafruit_BusIO_Register Adafruit_I2CRegister;
typedef Adafruit_BusIO_RegisterBits Adafruit_I2CRegisterBits;

#endif /* RTDUINO_USING_WIRE */
#endif /* _ADAFRUIT_I2C_REGISTER_H_ */
