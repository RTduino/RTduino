/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-22     Meco Man     first version
 */

#ifndef __AVR_IO_H__
#define __AVR_IO_H__

#ifdef __cplusplus
extern "C" {
#endif

#define _BV(bit) (1 << (bit))
#define cbi(reg, bitmask) *reg &= ~bitmask
#define sbi(reg, bitmask) *reg |= bitmask

#ifdef __cplusplus
}
#endif

#endif /* __AVR_IO_H__ */
