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
 * 2021-12-10     Meco Man     first version
 */

#ifndef __WIRING_PRIVATE_H__
#define __WIRING_PRIVATE_H__

#include <Arduino.h>

#ifndef RTDUINO_TINY_MODE
extern const pin_map_t pin_map_table[];

/*
 * If pins_arduino.h defined RTDUINO_PIN_MAX_LIMIT which is the maximum of RTduino pin number,
 * RTduino will check parameter validity, but will cause some ROM overhead.
 */
#ifdef RTDUINO_PIN_MAX_LIMIT
#define RTDUINO_CHECK_PIN_LIMIT_RETURN(pin, value) \
    if (pin > RTDUINO_PIN_MAX_LIMIT) \
    { \
        LOG_E("pin number exceed RTduino maximum for this board, please check in pins_arduino.h!"); \
        return value; \
    } \
    if (pin != pin_map_table[pin].arduino_pin) \
    { \
        LOG_E("pin_map_table is not sorted in sequence!"); \
        return value; \
    }
#else
#define RTDUINO_CHECK_PIN_LIMIT_RETURN(pin, value)
#endif /* RTDUINO_PIN_MAX_LIMIT */

#endif /* RTDUINO_TINY_MODE */

#endif /* __WIRING_PRIVATE_H__ */
