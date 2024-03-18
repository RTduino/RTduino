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

char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
{
    char fmt[20];
    rt_sprintf(fmt, "%%%d.%df", width, prec);
    rt_sprintf(sout, fmt, val);
    return sout;
}

char *dtostrnf(double val, signed char width, unsigned char prec, char *sout, size_t sout_size)
{
    char fmt[20];
    rt_snprintf(fmt, sizeof(fmt), "%%%d.%df", width, prec);
    rt_snprintf(sout, sout_size, fmt, val);
    return sout;
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
