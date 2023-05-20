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
 * 2022-09-09     Meco Man     first version
 */

#include <Arduino.h>

void setup(void)
{
    //Don't invoke pinMode function in here. It is not needed!
}

void loop(void)
{
    Serial.println(analogRead(A0));
    delay(500);
}
