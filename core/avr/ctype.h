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

#ifndef __AVR_CTYPE_H__
#define __AVR_CTYPE_H__

#include <rtconfig.h>

#if RT_VER_NUM >= 0x40101
#include <posix/ctype.h>
#else

#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define isascii(__c) ((unsigned)(__c)<=0177)
#define toascii(__c)  ((__c)&0177)

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* RT_VER_NUM >= 0x40101 */

#endif /* __AVR_CTYPE_H__ */
