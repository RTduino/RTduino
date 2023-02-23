/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     Meco Man     first version
 */

#ifndef __RTDUINO__
#define __RTDUINO__

#include "Arduino.h"

#if RT_VER_NUM < 0x50000
#define rt_align(x)  ALIGN(x)
#define rt_weak      RT_WEAK
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RTDUINO_USING_SKETCH
void sketch_setup(void);
void sketch_loop(void);
#endif /* RTDUINO_USING_SKETCH */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __RTDUINO__ */
