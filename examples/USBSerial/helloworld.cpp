/*
 * Copyright (c) 2021-2022, RT-Thread RTduino Development Team
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-23     Meco Man     first version
 */

#include <Arduino.h>

void setup(void)
{
    SerialUSB.begin();
}

void loop(void)
{
    SerialUSB.println("Hello world!");
    delay(500);
}
