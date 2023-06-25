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
 * 2022-04-12     Meco Man     first version
 */

#include <Arduino.h>

#define DBG_TAG    "RTduino.Tone"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

rt_weak void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
    LOG_E("tone() has not been implemented yet!");
}

rt_weak void noTone(uint8_t _pin)
{
    LOG_E("noTone() has not been implemented yet!");
}
