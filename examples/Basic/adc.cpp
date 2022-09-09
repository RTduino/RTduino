/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-09     Meco Man     first version
 */

#include <Arduino.h>

uint8_t ADC = A0;

void setup(void)
{
}

void loop(void)
{
    Serial.println(analogRead(ADC));
    delay(500);
}
