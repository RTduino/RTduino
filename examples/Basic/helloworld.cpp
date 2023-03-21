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

/* You also can use Serial2 or Serial3 to replace Serial if Serial2 or Serial3 is available on your board. */

void setup(void)
{
    Serial.begin();
}

void loop(void)
{
    Serial.println("Hello World!");
    delay(500);
}
