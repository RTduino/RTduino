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

#include "stdlib.h"
#include <rtthread.h>
#include <math.h>

// port from ESP32's version
char *dtostrf(double number, signed char width, unsigned char prec, char *s) {
  rt_bool_t negative = RT_FALSE;

  if (isnan(number)) {
    rt_strcpy(s, "nan");
    return s;
  }
  if (isinf(number)) {
    rt_strcpy(s, "inf");
    return s;
  }

  char *out = s;

  int fillme = width;  // how many cells to fill for the integer part
  if (prec > 0) {
    fillme -= (prec + 1);
  }

  // Handle negative numbers
  if (number < 0.0) {
    negative = RT_TRUE;
    fillme--;
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  // I optimized out most of the divisions
  double rounding = 2.0;
  for (unsigned int i = 0; i < prec; ++i) {
    rounding *= 10.0;
  }
  rounding = 1.0 / rounding;

  number += rounding;

  // Figure out how big our number really is
  double tenpow = 1.0;
  unsigned int digitcount = 1;
  while (number >= 10.0 * tenpow) {
    tenpow *= 10.0;
    digitcount++;
  }

  number /= tenpow;
  fillme -= digitcount;

  // Pad unused cells with spaces
  while (fillme-- > 0) {
    *out++ = ' ';
  }

  // Handle negative sign
  if (negative) {
    *out++ = '-';
  }

  // Print the digits, and if necessary, the decimal point
  digitcount += prec;
  int8_t digit = 0;
  while (digitcount-- > 0) {
    digit = (int8_t)number;
    if (digit > 9) {
      digit = 9;  // insurance
    }
    *out++ = (char)('0' | digit);
    if ((digitcount == prec) && (prec > 0)) {
      *out++ = '.';
    }
    number -= digit;
    number *= 10.0;
  }

  // make sure the string is terminated
  *out = 0;
  return s;
}

#if RT_VER_NUM < 0x40101

/**
 * @brief   Convert a long integer to a string representation with a specified radix.
 *
 * @param   value is the long integer to convert.
 * @param   string is the destination string where the result will be stored.
 * @param   radix is the base of the number system to be used for conversion.
 *
 * @return  Returns a pointer to the destination string.
 */
char *ltoa(long value, char *string, int radix)
{
    char tmp[33];
    char *tp = tmp;
    long i;
    unsigned long v;
    int sign;
    char *sp;

    if (string == NULL)
    {
        return 0;
    }

    if (radix > 36 || radix <= 1)
    {
        return 0;
    }

    sign = (radix == 10 && value < 0);
    if (sign)
    {
        v = -value;
    }
    else
    {
        v = (unsigned long)value;
    }

    while (v || tp == tmp)
    {
        i = v % radix;
        v = v / radix;
        if (i < 10)
            *tp++ = (char)(i + '0');
        else
            *tp++ = (char)(i + 'a' - 10);
    }

    sp = string;

    if (sign)
        *sp++ = '-';
    while (tp > tmp)
        *sp++ = *--tp;
    *sp = 0;

    return string;
}

/**
 * @brief   Convert an integer to a string representation with a specified radix.
 *
 * @param   value is the integer to convert.
 * @param   string is the destination string where the result will be stored.
 * @param   radix is the base of the number system to be used for conversion.
 *
 * @return  Returns a pointer to the destination string.
 */
char *itoa(int value, char *string, int radix)
{
    return ltoa(value, string, radix);
}

/**
 * @brief   Convert an unsigned long integer to a string representation with a specified radix.
 *
 * @param   value is the unsigned long integer to convert.
 * @param   string is the destination string where the result will be stored.
 * @param   radix is the base of the number system to be used for conversion.
 *
 * @return  Returns a pointer to the destination string.
 */
char *ultoa(unsigned long value, char *string, int radix)
{
    char tmp[33];
    char *tp = tmp;
    long i;
    unsigned long v = value;
    char *sp;

    if (string == NULL)
    {
        return 0;
    }

    if (radix > 36 || radix <= 1)
    {
        return 0;
    }

    while (v || tp == tmp)
    {
        i = v % radix;
        v = v / radix;
        if (i < 10)
            *tp++ = (char)(i + '0');
        else
            *tp++ = (char)(i + 'a' - 10);
    }

    sp = string;

    while (tp > tmp)
        *sp++ = *--tp;
    *sp = 0;

    return string;
}

/**
 * @brief   Convert an unsigned integer to a string representation with a specified radix.
 *
 * @param   value is the unsigned integer to convert.
 * @param   string is the destination string where the result will be stored.
 * @param   radix is the base of the number system to be used for conversion.
 *
 * @return  Returns a pointer to the destination string.
 */
char *utoa(unsigned value, char *string, int radix)
{
    return ultoa(value, string, radix);
}

#endif /* RT_VER_NUM < 0x40101 */
