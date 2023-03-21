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
 * 2022-05-12     Meco Man     add dtostrnf
 */

#ifndef __AVR_DTOSTRF_H__
#define __AVR_DTOSTRF_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

char *dtostrf(double val, signed char width, unsigned char prec, char *sout);
char *dtostrnf(double val, signed char width, unsigned char prec, char *sout, size_t sout_size);

#ifdef __cplusplus
}
#endif

#endif /* __AVR_DTOSTRF_H__ */
