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

int led = 9; /* LED pin */
int brightness = 0;
int fadeAmount = 5;

void setup()
{
    /* Do not invoke pinMode for the PWM IO! */
}

void loop()
{
    analogWrite(led, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255)
    {
        fadeAmount = -fadeAmount;
    }

    delay(30);
}
