/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-07     Meco Man     first version
 */

#include <Arduino.h>

uint8_t led = LED_BUILTIN;

void setup(void)
{
    /* put your setup code here, to run once: */
    pinMode(led, OUTPUT);
}

void loop(void)
{
    /* put your main code here, to run repeatedly: */
    digitalWrite(led, !digitalRead(led));
    delay(100);
}