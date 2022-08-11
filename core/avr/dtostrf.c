/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     first version
 * 2022-05-12     Meco Man     add dtostrnf
 */

#include "dtostrf.h"
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
