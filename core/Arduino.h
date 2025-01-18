/**
 * @file Arduino.h
 * @brief Arduino Core Header File.
 * @author Meco Man
 * @date 2021-12-10
 * @version 1.0
 *
 * @details
 * This file is part of RTduino project.
 *
 * The `Arduino.h` file is a core header file that plays a pivotal role in Arduino development.
 * It provides fundamental definitions, constants, and functions essential for writing
 * Arduino sketches and working with the Arduino framework.
 *
 * This file includes declarations for common data types, standard constants, and core
 * functions used in Arduino programming. It serves as the entry point for Arduino sketches,
 * ensuring that the necessary core features are available for use.
 *
 * @copyright
 * - Copyright (c) 2005-2013, Arduino Team.  All right reserved.
 * - Copyright (c) 2021-2023, RTduino Development Team. All rights reserved.
 *
 * @note
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * @see
 * - GitHub: https://github.com/RTduino/RTduino
 * - Gitee: https://gitee.com/rtduino/RTduino
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

#ifndef __ARDUINO_CORE_H__
#define __ARDUINO_CORE_H__

#include <rtdevice.h>
#include <rthw.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "Binary.h"
#include "avr/io.h"
#include "avr/stdlib.h"
#include "avr/pgmspace.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef RTDUINO_TINY_MODE
#include "pins_arduino.h"
#endif /* RTDUINO_TINY_MODE */

#ifdef RT_VER_NUM
#if RT_VER_NUM < 0x50000
#ifndef rt_align
#define rt_align(x)  ALIGN(x)
#endif /* rt_align */
#ifndef rt_weak
#define rt_weak      RT_WEAK
#endif /* rt_weak */
#elif RT_VER_NUM < 0x40101
#error "The minimum version requirement of RT-Thread is 4.1.1"
#endif /* RT_VER_NUM < 0x50000 */
#else
#error "Please use RT-Thread Standard Version with software packages management system!"
#endif /* RT_VER_NUM */

/**
 * @defgroup Bits and Bytes
 * @brief Bit and byte data type definitions.
 */

/**
 * @defgroup Digital I/O
 * @brief Digital input and output pins.
 */

/**
 * @defgroup Maths
 * @brief Mathematical definitions and functions.
 */

/**
 * @defgroup Time
 * @brief Time-related functions and definitions.
 */

/**
 * @defgroup Advanced I/O
 * @brief Advanced input and output functions.
 */

/**
 * @defgroup Analog I/O
 * @brief Analog input and output functions.
 */

/**
 * @defgroup Interrupts
 * @brief Interrupt-related functions and definitions.
 */

/**
 * @defgroup External Interrupts
 * @brief External interrupt functions and definitions.
 */

/**
 * @defgroup Random Numbers
 * @brief Random number generation functions.
 */

/**
 * @defgroup System
 * @brief System-related functions and initialization.
 */

/**
 * @struct pin_map_t
 * @brief RTduino pin map structure.
 * @details
 * This structure defines the mapping between Arduino pins and RT-Thread pins.
 */
typedef struct
{
    uint8_t     arduino_pin;    /**< Arduino pin number */
    rt_base_t   rt_pin;         /**< RT-Thread pin number */
    const char* device_name;    /**< Device name */
    rt_int8_t   channel;        /**< Channel number */
} pin_map_t;

/**
 * @typedef word
 * @brief Represents a 16-bit unsigned integer.
 * @ingroup Bits and Bytes
 */
typedef unsigned int word;

/**
 * @typedef boolean
 * @brief Represents a boolean value (true or false).
 * @ingroup Bits and Bytes
 */
typedef bool boolean;

/**
 * @typedef byte
 * @brief Represents an 8-bit unsigned integer.
 * @ingroup Bits and Bytes
 */
typedef uint8_t byte;

/**
 * @brief Represents a low state (0V).
 * @ingroup External Interrupts
 */
#define LOW     0x0

/**
 * @brief Represents a high state (usually 3.3V or 5V).
 * @ingroup External Interrupts
 */
#define HIGH    0x1

/**
 * @brief Represents a change in state.
 * @ingroup External Interrupts
 */
#define CHANGE  0x2

/**
 * @brief Represents a transition from high to low.
 * @ingroup External Interrupts
 */
#define FALLING 0x3

/**
 * @brief Represents a transition from low to high.
 * @ingroup External Interrupts
 */
#define RISING  0x4

/**
 * @brief Configures the pin as an input.
 * @ingroup Digital I/O
 */
#define INPUT               0x0

/**
 * @brief Configures the pin as an output.
 * @ingroup Digital I/O
 */
#define OUTPUT              0x1

/**
 * @brief Configures the pin as an input with an internal pull-up resistor.
 * @ingroup Digital I/O
 */
#define INPUT_PULLUP        0x2

/**
 * @brief Configures the pin as a floating input (same as INPUT).
 * @note RTduino extension.
 * @ingroup Digital I/O
 */
#define INPUT_FLOATING      INPUT

/**
 * @brief Configures the pin as an input with an internal pull-down resistor.
 * @note RTduino extension.
 * @ingroup Digital I/O
 */
#define INPUT_PULLDOWN      0x3

/**
 * @brief Configures the pin as an open-drain output.
 * @note RTduino extension.
 * @ingroup Digital I/O
 */
#define OUTPUT_OPEN_DRAIN   0x4

/**
 * @brief A constant representing the ratio of a circle's circumference to its diameter.
 * @ingroup Maths
 */
#define PI 3.1415926535897932384626433832795

/**
 * @brief A constant representing half of the mathematical constant π (pi) in programming and mathematics.
 * @ingroup Maths
 */
#define HALF_PI 1.5707963267948966192313216916398

/**
 * @brief A constant representing two times the mathematical constant π (pi) in programming and mathematics.
 * @ingroup Maths
 */
#define TWO_PI 6.283185307179586476925286766559

/**
 * @brief A constant for converting degrees to radians in mathematics and programming applications.
 * @ingroup Maths
 */
#define DEG_TO_RAD 0.017453292519943295769236907684886

/**
 * @brief A constant for converting radians to degrees in mathematical and programming contexts.
 * @ingroup Maths
 */
#define RAD_TO_DEG 57.295779513082320876798154814105

/**
 * @brief Euler's constant.
 * @ingroup Maths
 */
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

/**
 * @brief Least Significant Bit first.
 * @ingroup Bits and Bytes
 */
#define LSBFIRST 0

/**
 * @brief Most Significant Bit first.
 * @ingroup Bits and Bytes
 */
#define MSBFIRST 1


/*
  to prevent the error of redefinition of min and max:
  #define RTDUINO_NO_MIN
  #define RTDUINO_NO_MAX
  #include <RTduino.h>
*/
#ifndef RTDUINO_NO_MIN
/**
 * @ingroup Maths
 * @brief Returns the smaller of two values.
 * @param a First value.
 * @param b Second value.
 * @return The smaller of the two values.
 */
#define min(a, b) ((a)<(b)?(a):(b))
#endif /* RTDUINO_NO_MIN */

#ifndef RTDUINO_NO_MAX
/**
 * @ingroup Maths
 * @brief Returns the larger of two values.
 * @param a First value.
 * @param b Second value.
 * @return The larger of the two values.
 */
#define max(a, b) ((a)>(b)?(a):(b))
#endif /* RTDUINO_NO_MAX */

/**
 * @ingroup Maths
 * @brief Constrains a value to be within a specified range.
 * @param amt The value to constrain.
 * @param low The lower end of the range.
 * @param high The upper end of the range.
 * @return The constrained value.
 */
#define constrain(amt, low, high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

/**
 * @ingroup Maths
 * @brief Converts degrees to radians.
 * @param deg The angle in degrees.
 * @return The angle in radians.
 */
#define radians(deg) ((deg)*DEG_TO_RAD)

/**
 * @ingroup Maths
 * @brief Converts radians to degrees.
 * @param rad The angle in radians.
 * @return The angle in degrees.
 */
#define degrees(rad) ((rad)*RAD_TO_DEG)

/**
 * @ingroup Maths
 * @brief Squares a number.
 * @param x The number to square.
 * @return The squared value.
 */
#define sq(x) ((x)*(x)) /* x^2 */

/**
 * @brief No Operation.
 * @details Performs no operation. Used as a placeholder or for creating short delays.
 */
#define _NOP() do { __asm__ volatile ("nop"); } while (0)

#ifdef F_CPU
/**
 * @brief Gets the number of clock cycles per microsecond.
 * @return The number of clock cycles per microsecond.
 */
#define clockCyclesPerMicrosecond()  (F_CPU / 1000000L)

/**
 * @brief Converts clock cycles to microseconds.
 * @param a The number of clock cycles.
 * @return The corresponding number of microseconds.
 */
#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())

/**
 * @brief Converts microseconds to clock cycles.
 * @param a The number of microseconds.
 * @return The corresponding number of clock cycles.
 */
#define microsecondsToClockCycles(a) ((a) * clockCyclesPerMicrosecond())
#elif !defined(RTDUINO_TINY_MODE)
/**
 * @warning Please define F_CPU in pins_arduino.h
 */
#warning "Please define F_CPU in pins_arduino.h"
#endif /* F_CPU */

/**
 * @ingroup Bits and Bytes
 * @brief Computes the value of the specified bit (bit 0 is 1, bit 1 is 2, bit 2 is 4, etc.).
 * @param b The bit whose value to compute. Note that n needs to be between 0-31 (32 bit).
 * @return The value of the specified bit.
 * @see https://www.arduino.cc/reference/en/language/functions/bits-and-bytes/bit
 */
#define bit(b) (1UL << (b))

/**
 * @ingroup Bits and Bytes
 * @brief Reads the value of a specific bit.
 * @param value The number from which to read.
 * @param bit Which bit to read, starting at 0 for the least-significant (rightmost) bit.
 * @return The value of the specified bit (0 or 1).
 * @details
 * Reads a bit of a variable, e.g. bool, int. Note that float & double are not supported. You can read the bit of variables up to an unsigned long long (64 bits / 8 bytes).
 * @see https://www.arduino.cc/reference/en/language/functions/bits-and-bytes/bitread
 */
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

/**
 * @ingroup Bits and Bytes
 * @brief Sets a specific bit.
 * @param value The number in which to set a bit.
 * @param bit Which bit to set, starting at 0 for the least-significant (rightmost) bit.
 * @return The value of the numeric variable after the bit at position n is set.
 * @details
 * Sets a bit of a variable, e.g. bool, int. Note that float & double are not supported. You can set the bit of variables up to an unsigned long long (64 bits / 8 bytes).
 * @see https://www.arduino.cc/reference/en/language/functions/bits-and-bytes/bitset
 */
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
/**
 * @ingroup Bits and Bytes
 * @brief Clears a specific bit.
 * @param value The number in which to clear a bit.
 * @param bit Which bit to clear, starting at 0 for the least-significant (rightmost) bit.
 * @return The value of the numeric variable after the bit at position n is cleared.
 * @see https://www.arduino.cc/reference/en/language/functions/bits-and-bytes/bitclear
 */
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
/**
 * @ingroup Bits and Bytes
 * @brief Toggles a specific bit.
 * @param value The number in which to toggle a bit.
 * @param bit Which bit to toggle, starting at 0 for the least-significant (rightmost) bit.
 * @return The value of the numeric variable after the bit at position n is toggled.
 * @see https://www.arduino.cc/reference/en/language/functions/bits-and-bytes/bittoggle
 */
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
/**
 * @ingroup Bits and Bytes
 * @brief Writes a specific bit.
 * @param value The number in which to write a bit.
 * @param bit Which bit to write, starting at 0 for the least-significant (rightmost) bit.
 * @param bitvalue The value to write (0 or 1).
 * @return The value of the numeric variable after the specified bit is written.
 * @see https://www.arduino.cc/reference/en/language/functions/bits-and-bytes/bitwrite
 */
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

/**
 * @ingroup Bits and Bytes
 * @brief Extracts the low-order (rightmost) byte of a variable (e.g. a word).
 * @param w A value of any type.
 * @return The low-order (rightmost) byte of the value.
 * @see https://www.arduino.cc/reference/en/language/functions/bits-and-bytes/lowbyte
 */
#define lowByte(w) ((uint8_t) ((w) & 0xff))

/**
 * @ingroup Bits and Bytes
 * @brief Extracts the high-order (leftmost) byte of a word (or the second lowest byte of a larger data type).
 * @param w A value of any type.
 * @return The high-order (leftmost) byte of the value.
 * @see https://www.arduino.cc/reference/en/language/functions/bits-and-bytes/highbyte
 */
#define highByte(w) ((uint8_t) ((w) >> 8))

/**
 * @ingroup Digital I/O
 * @brief Configures the specified pin to behave either as an input or an output.
 * @param pin The number of the pin to be configured.
 * @param mode The mode to set for the pin (INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN, OUTPUT_OPEN_DRAIN).
 * @note
 * This function can only be used for digitalRead() and digitalWrite().<br>
 * For analogRead() or analogWrite(), please **DO NOT** use this function to configure the pin.
 * @see https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode
 */
void pinMode(uint8_t pin, uint8_t mode);

/**
 * @ingroup Digital I/O
 * @brief Writes a HIGH or a LOW value to a digital pin.
 * @param pin The number of the pin to write to.
 * @param val The value to write (HIGH or LOW).
 * @see https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite
 */
void digitalWrite(uint8_t pin, uint8_t val);

/**
 * @ingroup Digital I/O
 * @brief Reads the value from a specified digital pin, either HIGH or LOW.
 * @param pin The number of the pin to read from.
 * @return The value read from the pin (HIGH or LOW).
 * @see https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread
 */
int digitalRead(uint8_t pin);

/**
 * @ingroup Analog I/O
 * @brief Reads the analog input on a specified analog pin.
 * @param pin The number of the analog input pin.
 * @return The analog value read from the pin.
 * @see https://www.arduino.cc/reference/en/language/functions/analog-io/analogread
 */
int analogRead(uint8_t pin);

/**
 * @ingroup Analog I/O
 * @brief Writes an analog value (PWM wave) to a pin.
 * @param pin The number of the PWM pin.
 * @param val The duty cycle: between 0 (always off) and 255 (always on).
 * @see https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite
 */
void analogWrite(uint8_t pin, int val);

/**
 * @ingroup Analog I/O
 * @brief Configures the reference voltage used for analog input (e.g., external, internal, etc.).
 * @param mode The type of reference (e.g., DEFAULT, INTERNAL, EXTERNAL).
 * @see https://www.arduino.cc/reference/en/language/functions/analog-io/analogreference
 */
void analogReference(uint8_t mode);

/**
 * @ingroup Analog I/O
 * @brief Sets the number of bits of resolution for the analogRead function.
 * @param bits The number of bits (e.g., 8, 10, 12) for the resolution.
 * @see https://www.arduino.cc/reference/en/language/functions/analog-io/analogreadresolution
 */
void analogReadResolution(uint8_t bits);

/**
 * @ingroup Analog I/O
 * @brief Sets the number of bits of resolution for the analogWrite function.
 * @param bits The number of bits (e.g., 8, 10, 12) for the resolution.
 * @see https://www.arduino.cc/reference/en/language/functions/analog-io/analogwriteresolution
 */
void analogWriteResolution(uint8_t bits);

/**
 * @ingroup Analog I/O
 * @brief Sets the frequency of the PWM on a pin.
 * @param frequency The desired frequency in Hertz (cycles per second).
 * @see https://www.arduino.cc/reference/en/language/functions/analog-io/analogwritefrequency
 */
void analogWriteFrequency(uint32_t frequency);

/**
 * @ingroup Time
 * @brief Pauses the execution of the current task, allowing other tasks to run.
 * @details
 * The `yield` function is used to pause the execution of the current task, giving
 * the RT-Thread scheduler an opportunity to run other tasks. This function is typically
 * used in scenarios where a task voluntarily relinquishes control to the scheduler,
 * allowing a cooperative multitasking system.
 *
 * When a task calls `yield`, it enters a waiting state, and the scheduler may choose
 * to run another task that is ready to execute. After yielding, the task that called
 * `yield` may be scheduled to run again in the future.
 *
 * @note
 * - This function is essential for cooperative multitasking, where tasks voluntarily
 * relinquish control to the scheduler.
 * - In preemptive multitasking systems, tasks yield control based on the scheduler's
 * predefined time slices.
 * - The behavior of `yield` can vary depending on the underlying RT-Thread configuration
 * and scheduler settings.
 *
 * @see https://www.arduino.cc/reference/en/language/functions/scheduler/yield
 */
void yield(void);

/**
 * @ingroup Time
 * @brief Delays the program for the specified number of milliseconds.
 * @param ms The number of milliseconds to delay.
 * @see https://www.arduino.cc/reference/en/language/functions/time/delay
 */
void delay(unsigned long ms);

/**
 * @ingroup Time
 * @brief Delays the program for the specified number of microseconds.
 * @param us The number of microseconds to delay.
 * @see https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds
 */
void delayMicroseconds(unsigned int us);

/**
 * @ingroup Time
 * @brief Returns the number of milliseconds since the Arduino board began running the current program.
 * @return The number of milliseconds since program start.
 * @see https://www.arduino.cc/reference/en/language/functions/time/millis
 */
unsigned long millis(void);

/**
 * @ingroup Time
 * @brief Returns the number of microseconds since the Arduino board began running the current program.
 * @return The number of microseconds since program start.
 * @see https://www.arduino.cc/reference/en/language/functions/time/micros
 */
unsigned long micros(void);

/**
 * @ingroup Advanced I/O
 * @brief Measures the duration of a pulse (in us) on the specified pin.
 * @details
 * The state parameter specifies the type of pulse to measure (HIGH or LOW).
 * The timeout parameter specifies the maximum time to wait for the pulse to complete.
 * This function works on pulses from 2-3 microseconds to 3 minutes in length.
 * It cannot work in noInterrupt() context.
 * @param pin The pin number to measure the pulse on.
 * @param state The type of pulse to measure (HIGH or LOW).
 * @param timeout The maximum time to wait for the pulse to complete, in microseconds.
 * @return The duration of the pulse in microseconds, or 0 if no pulse starts before the timeout.
 * @see https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein
 */
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);

/**
 * @ingroup Advanced I/O
 * @brief Measures the duration of a pulse (in us) on the specified pin.
 * @details
 * The state parameter specifies the type of pulse to measure (HIGH or LOW).
 * The timeout parameter specifies the maximum time to wait for the pulse to complete.
 * This function works on pulses from 2-3 microseconds to 3 minutes in length.
 * It relies on the micros() function, so it cannot be used in noInterrupt() context.
 * @param pin The pin number to measure the pulse on.
 * @param state The type of pulse to measure (HIGH or LOW).
 * @param timeout The maximum time to wait for the pulse to complete, in microseconds.
 * @return The duration of the pulse in microseconds, or 0 if no pulse starts before the timeout.
 * @see https://www.arduino.cc/reference/en/language/functions/advanced-io/pulseinlong
 */
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

/**
 * @ingroup Advanced I/O
 * @brief Sends out a byte of data on the specified data pin using the specified clock and bit order.
 * @param dataPin The pin on which to output the data.
 * @param clockPin The pin to toggle to signal the receiving device that the data is ready.
 * @param bitOrder The order in which to send the bits (LSBFIRST or MSBFIRST).
 * @param val The data byte to send.
 * @see https://www.arduino.cc/reference/en/language/functions/advanced-io/shiftout
 */
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

/**
 * @ingroup Advanced I/O
 * @brief Shifts in data on a specified data pin using a specified clock pin and bit order.
 * @param dataPin The pin on which to read the incoming data.
 * @param clockPin The pin to toggle to signal the sending device that the Arduino is ready to receive the next bit.
 * @param bitOrder The order in which to receive the bits (LSBFIRST or MSBFIRST).
 * @return The received data byte.
 * @see https://www.arduino.cc/reference/en/language/functions/advanced-io/shiftin
 */
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

/**
 * @ingroup Interrupts
 * @brief Enables interrupts, allowing the execution of interrupt service routines.
 * @see https://www.arduino.cc/reference/en/language/functions/interrupts/interrupts
 */
void interrupts(void);

/**
 * @ingroup Interrupts
 * @brief Disables interrupts, preventing the execution of interrupt service routines.
 * @see https://www.arduino.cc/reference/en/language/functions/interrupts/nointerrupts
 */
void noInterrupts(void);

/**
 * @ingroup External Interrupts
 * @brief Converts a digital pin number to the corresponding interrupt number.
 * @param pin The digital pin number.
 * @return The corresponding interrupt number, or NOT_AN_INTERRUPT if the pin does not support interrupts.
 * @see https://www.arduino.cc/reference/en/language/functions/external-interrupts/digitalpintointerrupt
 */
uint8_t digitalPinToInterrupt(uint8_t pin);

/**
 * @ingroup External Interrupts
 * @brief Attaches an interrupt service routine to a specified interrupt.
 * @param interruptNum The interrupt number to attach the ISR to.
 * @param userFunc The function to call when the interrupt occurs.
 * @param mode The type of interrupt to attach the ISR to (e.g., CHANGE, FALLING, RISING).
 * @see https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt
 */
void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode);

/**
 * @ingroup External Interrupts
 * @brief Detaches an interrupt service routine from a specified interrupt.
 * @param interruptNum The interrupt number to detach the ISR from.
 * @see https://www.arduino.cc/reference/en/language/functions/external-interrupts/detachinterrupt
 */
void detachInterrupt(uint8_t interruptNum);

/**
 * @ingroup Random Numbers
 * @brief Initializes the pseudo-random number generator.
 * @param seed The seed value for the random number generator.
 * @see https://www.arduino.cc/reference/en/language/functions/random-numbers/randomseed
 */
void randomSeed(unsigned long seed);

/**
 * @ingroup Maths
 * @brief Maps a value from one range to another.
 * @param x The value to map.
 * @param in_min The lower bound of the input range.
 * @param in_max The upper bound of the input range.
 * @param out_min The lower bound of the output range.
 * @param out_max The upper bound of the output range.
 * @return The mapped value.
 * @see https://www.arduino.cc/reference/en/language/functions/math/map
 */
long map(long x, long in_min, long in_max, long out_min, long out_max);

/**
 * @ingroup System
 * @brief Initialization function for the Arduino environment.
 * @details This function should be called at the beginning of the program to set up the Arduino environment.
 * @see https://www.arduino.cc/reference/en/language/structure/sketch/setup
 */
void setup(void);

/**
 * @ingroup System
 * @brief Main loop function for the Arduino program.
 * @details This function is called repeatedly after the setup function, forming the main loop of the Arduino program.
 * @see https://www.arduino.cc/reference/en/language/structure/sketch/loop
 */
void loop(void);

/**
 * @ingroup System
 * @brief Variant-specific initialization function.
 * @details This function is called during the initialization process to set up variant-specific configurations.
 * @note Do not call this function manually.
 */
void initVariant(void);

#ifdef RTDUINO_USING_SPI
/**
 * @ingroup System
 * @brief Switches to the SPI bus.
 * @param bus_name The name of the SPI bus to switch to.
 * @note Do not call this function manually.
 */
void switchToSPI(const char *bus_name);
#endif /* RTDUINO_USING_SPI */

#ifdef __cplusplus
} /* extern "C" { */
#endif

#ifdef __cplusplus

/**
 * @ingroup Maths
 * @brief Generates a random long integer within the specified range.
 * @param max The upper bound of the range (exclusive).
 * @return A random long integer.
 * @see https://www.arduino.cc/reference/en/language/functions/random-numbers/random
 */
long random(long max);

/**
 * @ingroup Random numbers
 * @brief Generates a random long integer within the specified range.
 * @param min The lower bound of the range.
 * @param max The upper bound of the range (exclusive).
 * @return A random long integer.
 * @see https://www.arduino.cc/reference/en/language/functions/random-numbers/random
 */
long random(long min, long max);

/**
 * @ingroup Maths
 * @brief Maps a value from one range to another range.
 *
 * This template function maps a number from one range to another range, maintaining the ratio of the input range 
 * to output range. It can be used with any data type that supports arithmetic operations (e.g., int, float, double).
 *
 * @tparam T The data type of the input and output values. Must support arithmetic operations.
 * @param x The number to map.
 * @param in_min The lower bound of the input range.
 * @param in_max The upper bound of the input range.
 * @param out_min The lower bound of the output range.
 * @param out_max The upper bound of the output range.
 *
 * @return The mapped value of type T.
 *
 * @code
 * int input = 10;
 * int output = map<int>(input, 0, 100, -50, 50); // Maps 10 from range 0-100 to -40 in range -50 to 50
 * @endcode
 *
 * @see https://www.arduino.cc/reference/en/language/functions/math/map
 */
template <typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (T)(((double)(x - in_min) * (double)(out_max - out_min)) / (double)((in_max - in_min) + out_min));
}

/**
 * @ingroup Conversion
 * @brief Combines two bytes into a word.
 * @param w The high byte.
 * @return The combined word.
 * @see https://www.arduino.cc/reference/en/language/functions/bits-and-bytes/makeword
 */
uint16_t makeWord(uint16_t w);

/**
 * @ingroup Conversion
 * @brief Combines two bytes into a word.
 * @param h The high byte.
 * @param l The low byte.
 * @return The combined word.
 * @see https://www.arduino.cc/reference/en/language/functions/bits-and-bytes/makeword
 */
uint16_t makeWord(byte h, byte l);

/**
 * @ingroup Conversion
 * @brief Converts a value to the word data type.
 * @see https://www.arduino.cc/reference/en/language/variables/conversion/wordcast
 */
#define word(...) makeWord(__VA_ARGS__)

/**
 * @ingroup Advanced I/O
 * @brief Measures the duration of a pulse on the specified pin.
 * @param pin The number of the pin on which to measure the pulse.
 * @param state The type of pulse to measure (e.g., HIGH, LOW, CHANGE, FALLING, RISING).
 * @param timeout The maximum duration to wait for the pulse, in microseconds.
 * @return The duration of the pulse in microseconds, or 0 if no pulse starts before the timeout.
 * @see https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein
 */
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

/**
 * @ingroup Advanced I/O
 * @brief Measures the duration of a pulse on the specified pin.
 * @param pin The number of the pin on which to measure the pulse.
 * @param state The type of pulse to measure (e.g., HIGH, LOW, CHANGE, FALLING, RISING).
 * @param timeout The maximum duration to wait for the pulse, in microseconds.
 * @return The duration of the pulse in microseconds, or 0 if no pulse starts before the timeout.
 * @see https://www.arduino.cc/reference/en/language/functions/advanced-io/pulseinlong
 */
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

/**
 * @ingroup Advanced I/O
 * @brief Generates a square wave of the specified frequency (and 50% duty cycle) on a pin.
 * @param pin The number of the pin on which to output the tone.
 * @param frequency The frequency of the square wave in hertz.
 * @param duration The duration of the tone in milliseconds (optional).
 * @see https://www.arduino.cc/reference/en/language/functions/advanced-io/tone
 */
void tone(uint8_t pin, unsigned int frequency, unsigned long duration = 0);

/**
 * @ingroup Advanced I/O
 * @brief Stops the generation of a square wave previously started by the tone() function.
 * @param pin The number of the pin on which to stop the tone.
 * @see https://www.arduino.cc/reference/en/language/functions/advanced-io/notone
 */
void noTone(uint8_t pin);

#include "WCharacter.h"
#include "WString.h"
#include "HardwareSerial.h"
#ifdef RTDUINO_USING_USBSERIAL
#include "USBSerial.h"
#endif /* RTDUINO_USING_USBSERIAL */

#endif /* __cplusplus */

/* low level test functions */
#if defined(RTDUINO_USING_UTEST) || defined(RT_UTEST_USING_ALL_CASES)
#include "utest/TC_LLT.h"
#endif /* RTDUINO_USING_UTEST || RT_UTEST_USING_ALL_CASES */

#endif /* __ARDUINO_CORE_H__ */
