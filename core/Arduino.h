/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     first version
 */
/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef __ARDUINO_H__
#define __ARDUINO_H__

#include <rtdevice.h>
#include <rthw.h>
#include <board.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <posix/stdlib.h>
#include <math.h>

#include "avr/io.h"
#include "avr/pgmspace.h"
#include "avr/dtostrf.h"
#include "binary.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RTDUINO_TINY_MODE
/*
  Please read the document if pins_arduino.h cannot be found:
  https://github.com/RTduino/RTduino#readme
*/
#include "pins_arduino.h"
#endif /* RTDUINO_TINY_MODE */

#if RT_VER_NUM < 0x40101
#error "The minimum version requirement of RT-Thread is 4.1.1"
#endif /* RT_VER_NUM < 0x40101 */

/* Arduino Pin Map */
typedef struct
{
    uint8_t     arduino_pin;
    rt_base_t   rt_pin;
    const char* device_name;
    rt_int8_t   channel;
}pin_map_t;

typedef unsigned int word;
typedef bool boolean;
typedef uint8_t byte;

#define LOW     0x0
#define HIGH    0x1
#define CHANGE  0x2
#define FALLING 0x3
#define RISING  0x4

/* Official Arduino */
#define INPUT               0x0
#define OUTPUT              0x1
#define INPUT_PULLUP        0x2
/* RT-Thread extension */
#define INPUT_FLOATING      INPUT
#define INPUT_PULLDOWN      0x3
#define OUTPUT_OPEN_DRAIN   0x4

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

/* math operations */
/* adb() in stdlib.h and round() in math.h */
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x)) /* x^2 */

/* define interrupts and noInterrupts */
#if defined(ARCH_ARM) || defined(ARCH_RISCV)
#define interrupts()   __enable_irq()
#define noInterrupts() __disable_irq()
#else
#define interrupts()
#define noInterrupts()
#warning "Please define interrupts for this architecture in Arduino.h"
#endif /* defined(ARCH_ARM) || defined(ARCH_RISCV) */

#define _NOP() do { __asm__ volatile ("nop"); } while (0)

#ifdef F_CPU
#define clockCyclesPerMicrosecond()  (F_CPU / 1000000L)
#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a) * clockCyclesPerMicrosecond())
#elif !defined(RTDUINO_TINY_MODE)
#warning "Please define F_CPU in pins_arduino.h"
#endif /* F_CPU */

/* byte operations */
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

/* bit operations */
#define bit(b) (1UL << (b))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);
void analogReference(uint8_t mode);
void analogReadResolution(uint8_t bits);
void analogWriteResolution(uint8_t bits);
void analogWriteFrequency(uint32_t frequency);

void yield(void);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

uint8_t digitalPinToInterrupt(uint8_t pin);
void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode);
void detachInterrupt(uint8_t interruptNum);

void setup(void);
void loop(void);

long long map(long long, long long, long long, long long, long long);
float mapFloat(float, float, float, float, float);
void randomSeed(unsigned long);

/* Don't invoke these functions manually */
void initVariant(void);
#ifdef RTDUINO_USING_SPI
void switchToSPI(const char *bus_name);
#endif /* RTDUINO_USING_SPI */

#ifdef __cplusplus
} /* extern "C" { */
#endif

#ifdef __cplusplus

long random(long);
long random(long, long);

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);
#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

void tone(uint8_t pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t pin);

#include "WCharacter.h"
#include "WString.h"
#include "HardwareSerial.h"
#ifdef RTDUINO_USING_USBSERIAL
#include "USBSerial.h"
#endif /* RTDUINO_USING_USBSERIAL */

#endif /* __cplusplus */

#endif /* __ARDUINO_H__ */
