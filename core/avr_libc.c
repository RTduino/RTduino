/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     first version
 */

/*
https://www.nongnu.org/avr-libc/user-manual/group__avr__stdlib.html
*/

#include "avr_libc.h"
#include <rtthread.h>

//https://github.com/esp8266/Arduino/pull/7093
//We don't need to implement like #7093 because rt_sprintf can support float
char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
{
    char fmt[20];
    rt_sprintf(fmt, "%%%d.%df", width, prec);
    rt_sprintf(sout, fmt, val);
    return sout;
}
