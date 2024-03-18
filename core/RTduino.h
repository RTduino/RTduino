/**
 * @file RTduino.h
 * @brief RTduino Core Header File.
 * @author Meco Man
 * @date 2022-09-27
 * @version 1.0
 *
 * @details
 * This file is part of the RTduino project.
 *
 * The `RTduino.h` file serves as a pivotal component in the integration of the RT-Thread Real-Time Operating System (RTOS)
 * into Arduino sketches. It extends the core functionality provided by the `Arduino.h` file by introducing structures,
 * functions, and macros essential for creating and managing real-time tasks with specified stack sizes and priorities.
 *
 * This file builds upon the features of the `Arduino.h` file, enhancing development by introducing RT-Thread RTOS capabilities.
 * This integration allows for efficient multitasking and real-time task execution in Arduino sketches. Notably, `RTduino.h` includes
 * macros for creating RTduino loaders, critical components for effectively managing real-time tasks in Arduino programming.
 *
 * @copyright
 * Copyright (c) 2021-2023, RTduino Development Team. All rights reserved.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 *
 * @see
 * - GitHub: https://github.com/RTduino/RTduino
 * - Gitee: https://gitee.com/rtduino/RTduino
 */


#ifndef __RTDUINO_CORE__
#define __RTDUINO_CORE__

#include "Arduino.h"

#ifndef RTDUINO_THREAD_SIZE
#define RTDUINO_THREAD_SIZE     2048
#endif /* RTDUINO_THREAD_SIZE */

#ifndef RTDUINO_THREAD_PRIO
#define RTDUINO_THREAD_PRIO     (RT_THREAD_PRIORITY_MAX - 2)
#endif /* RTDUINO_THREAD_PRIO */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup MultiThread
 * @brief Multi-threading support.
 */

/**
 * @struct rtduino_loader_t
 * @brief RTduino loader.
 */
typedef struct
{
    const char* name;               /**< Name of the loader. */
    void (*setup)(void);            /**< Setup function. */
    void (*loop)(void);             /**< Loop function. */
    rt_thread_t thread;             /**< Loader thread entity. */
} rtduino_loader, *rtduino_loader_t;

/**
 * @ingroup MultiThread
 * @brief Deletes an RTduino loader.
 *
 * @param loader    Pointer to the RTduino loader to be deleted.
 * @return          RT_EOK on success, negative value on failure.
 */
rt_err_t rtduino_sketch_loader_delete(rtduino_loader_t loader);

/**
 * @ingroup MultiThread
 * @brief Creates an RTduino loader with specified stack size and priority.
 *
 * @param name      Name of the loader.
 * @param setup     Setup function.
 * @param loop      Loop function.
 * @param stack_size Stack size of the loader thread.
 * @param priority  Priority of the loader thread.
 * @return          Pointer to the created RTduino loader, or RT_NULL if the creation failed.
 */
rtduino_loader_t rtduino_sketch_loader_create_stacksize_prio(const char* name,
        void (*setup)(void), void (*loop)(void), rt_uint32_t stack_size, rt_uint8_t priority);

/**
* @ingroup MultiThread
 * @brief Creates an RTduino loader with default stack size and priority.
 *
 * @param name      Name of the loader.
 * @param setup     Setup function.
 * @param loop      Loop function.
 * @return          Pointer to the created RTduino loader.
 */
rt_inline rtduino_loader_t rtduino_sketch_loader_create(const char* name,
        void (*setup)(void), void (*loop)(void))
{
    return rtduino_sketch_loader_create_stacksize_prio(name, setup, loop, RTDUINO_THREAD_SIZE, RTDUINO_THREAD_PRIO);
}

/**
 * @ingroup MultiThread
 * @brief Creates an RTduino loader with specified priority.
 *
 * @param name      Name of the loader.
 * @param setup     Setup function.
 * @param loop      Loop function.
 * @param priority  Priority of the loader thread.
 * @return          Pointer to the created RTduino loader.
 */
rt_inline rtduino_loader_t rtduino_sketch_loader_create_prio(const char* name,
        void (*setup)(void), void (*loop)(void), rt_uint8_t priority)
{
    return rtduino_sketch_loader_create_stacksize_prio(name, setup, loop, RTDUINO_THREAD_SIZE, priority);
}

/**
 * @ingroup MultiThread
 * @brief Creates an RTduino loader with specified stack size.
 *
 * @param name      Name of the loader.
 * @param setup     Setup function.
 * @param loop      Loop function.
 * @param stack_size Stack size of the loader thread.
 * @return          Pointer to the created RTduino loader.
 */
rt_inline rtduino_loader_t rtduino_sketch_loader_create_stacksize(const char* name,
        void (*setup)(void), void (*loop)(void), rt_uint32_t stack_size)
{
    return rtduino_sketch_loader_create_stacksize_prio(name, setup, loop, stack_size, RTDUINO_THREAD_PRIO);
}

/**
 * @ingroup MultiThread
 * @brief Macro to define an RTduino loader with specified stack size, priority, and initialization level.
 *
 * @param name          Name of the loader.
 * @param setup         Setup function.
 * @param loop          Loop function.
 * @param stack_size    Stack size of the loader thread.
 * @param priority      Priority of the loader thread.
 * @param init_level    Initialization level macro.
 */
#define RTDUINO_SKETCH_LOADER_STACKSIZE_PRIO_INITLEVEL(name, setup, loop, stack_size, priority, init_level) \
    static int _sketch_loader(void) \
    { \
        rtduino_sketch_loader_create_stacksize_prio(name, setup, loop, stack_size, priority);\
        return 0; \
    } \
    init_level(_sketch_loader)

/**
 * @ingroup MultiThread
 * @brief Macro to define an RTduino loader with specified stack size and priority.
 *
 * @param name          Name of the loader.
 * @param setup         Setup function.
 * @param loop          Loop function.
 * @param stack_size    Stack size of the loader thread.
 * @param priority      Priority of the loader thread.
 */
#define RTDUINO_SKETCH_LOADER_STACKSIZE_PRIO(name, setup, loop, stack_size, priority) \
    RTDUINO_SKETCH_LOADER_STACKSIZE_PRIO_INITLEVEL(name, setup, loop, stack_size, priority, INIT_APP_EXPORT)

/**
 * @ingroup MultiThread
 * @brief Macro to define an RTduino loader with default stack size and priority.
 *
 * @param name          Name of the loader.
 * @param setup         Setup function.
 * @param loop          Loop function.
 */
#define RTDUINO_SKETCH_LOADER(name, setup, loop) \
    RTDUINO_SKETCH_LOADER_STACKSIZE_PRIO(name, setup, loop, RTDUINO_THREAD_SIZE, RTDUINO_THREAD_PRIO)

/**
 * @ingroup MultiThread
 * @brief Macro to define an RTduino loader with specified priority.
 *
 * @param name          Name of the loader.
 * @param setup         Setup function.
 * @param loop          Loop function.
 * @param priority      Priority of the loader thread.
 */
#define RTDUINO_SKETCH_LOADER_PRIO(name, setup, loop, priority) \
    RTDUINO_SKETCH_LOADER_STACKSIZE_PRIO(name, setup, loop, RTDUINO_THREAD_SIZE, priority)

/**
 * @ingroup MultiThread
 * @brief Macro to define an RTduino loader with specified stack size.
 *
 * @param name          Name of the loader.
 * @param setup         Setup function.
 * @param loop          Loop function.
 * @param stack_size    Stack size of the loader thread.
 */
#define RTDUINO_SKETCH_LOADER_STACKSIZE(name, setup, loop, stack_size) \
    RTDUINO_SKETCH_LOADER_STACKSIZE_PRIO(name, setup, loop, stack_size, RTDUINO_THREAD_PRIO)

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __RTDUINO_CORE__ */
