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
 * 2022-05-22     Meco Man     first version
 */

#ifndef __AVR_IO_H__
#define __AVR_IO_H__

#ifdef __cplusplus
extern "C" {
#endif

#define _BV(bit) (1 << (bit))
#define cbi(reg, bitmask) *(reg) &= ~(bitmask)
#define sbi(reg, bitmask) *(reg) |= (bitmask)

#ifdef __cplusplus
}
#endif

#endif /* __AVR_IO_H__ */
