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
 * 2022-08-07     Meco Man     first version
 */

#include <Arduino.h>

uint8_t led = LED_BUILTIN;

void setup(void)
{
    pinMode(led, OUTPUT);
}

void loop(void)
{
    digitalWrite(led, !digitalRead(led));
    delay(100);
}
