/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-31     Meco Man     first version
 */

/* https://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html */

#define PROGMEM
#define pgm_read_byte(x)    (*(x))
