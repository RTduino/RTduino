/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-17     Meco Man     first version
 */

#ifndef __AVR_STDLIB_H__
#define __AVR_STDLIB_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <rtconfig.h>
#include <stddef.h>

#if RT_VER_NUM >= 0x40101
#include <posix/stdlib.h>
#else
#include <stdlib.h>
char* itoa(int value, char *string, int radix);
char* ltoa(long value, char *string, int radix);
char* utoa(unsigned value, char *string, int radix);
char* ultoa(unsigned long value, char *string, int radix);
#endif /* RT_VER_NUM >= 0x40101 */

/**
   The dtostrf() function converts the double value passed in \c val into
   an ASCII representationthat will be stored under \c s.  The caller
   is responsible for providing sufficient storage in \c s.

   Conversion is done in the format \c "[-]d.ddd".  The minimum field
   width of the output string (including the possible \c '.' and the possible
   sign for negative values) is given in \c width, and \c prec determines
   the number of digits after the decimal sign. \c width is signed value,
   negative for left adjustment.

   The dtostrf() function returns the pointer to the converted string \c s.
*/
char *dtostrf(double val, signed char width, unsigned char prec, char *sout);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* __AVR_STDLIB_H__ */
