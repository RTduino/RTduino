/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-12     Meco Man     first version
 */

#include <rtdevice.h>
#include "wiring_private.h"

#define DBG_TAG    "Arduino"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

// frequency (in hertz) and duration (in milliseconds).
void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
    LOG_E("tone() has not been implemented yet!");
}

void noTone(uint8_t _pin)
{
    LOG_E("noTone() has not been implemented yet!");
}
