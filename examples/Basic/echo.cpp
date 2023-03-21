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
 * 2022-09-27     Meco Man     first version
 */

#include <Arduino.h>

void setup(void)
{
    Serial.begin();
    delay(10000);
}

void loop(void)
{
    int ch;
    while(!Serial.available());
    ch = Serial.read();
    if (ch > 0)
    {
        Serial.print((char)ch);
    }
}
