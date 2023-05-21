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
 * 2022-09-27     Meco Man     first version
 */

#ifndef __RTDUINO__
#define __RTDUINO__

#include "Arduino.h"

#if RT_VER_NUM < 0x50000
#define rt_align(x)  ALIGN(x)
#define rt_weak      RT_WEAK
#endif

#ifndef RTDUINO_THREAD_SIZE
#define RTDUINO_THREAD_SIZE     2048
#endif /* RTDUINO_THREAD_SIZE */

#ifndef RTDUINO_THREAD_PRIO
#define RTDUINO_THREAD_PRIO     30
#endif /* RTDUINO_THREAD_PRIO */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    const char* name;
    void (*setup)(void);
    void (*loop)(void);
    rt_thread_t thread;
}rtduino_loader, *rtduino_loader_t;

rtduino_loader_t rtduino_sketch_loader_create(const char* name, void (*setup)(void), void (*loop)(void));
rtduino_loader_t rtduino_sketch_loader_create_prio(const char* name, void (*setup)(void), void (*loop)(void), rt_uint32_t stack_size, rt_uint8_t priority);
rt_err_t rtduino_sketch_loader_delete(rtduino_loader_t loader);

#define RTDUINO_SKETCH_LOADER_PRIO(name, setup, loop, stack_size, priority) \
    static int sketch_loader(void) \
    { \
        rtduino_sketch_loader_create_prio(name, setup, loop, stack_size, priority);\
        return 0; \
    } \
    INIT_APP_EXPORT(sketch_loader)

#define RTDUINO_SKETCH_LOADER(name, setup, loop) \
    RTDUINO_SKETCH_LOADER_PRIO(name, setup, loop, RTDUINO_THREAD_SIZE, RTDUINO_THREAD_PRIO)

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __RTDUINO__ */
