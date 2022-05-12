/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     first version
 */

#ifndef __AVR_DTOSTRF_H__
#define __AVR_DTOSTRF_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

char *dtostrf(double val, signed char width, unsigned char prec, char *sout);
char *dtostrnf(double val, signed char width, unsigned char prec, char *sout, size_t sout_size);

#ifdef __cplusplus
}
#endif

#endif /* __AVR_DTOSTRF_H__ */
