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

// Include the Servo library
#include <Servo.h>

// Declare the Servo pin
int servoPin = 3;
// Create a servo object
Servo Servo1;

void setup()
{
    // We need to attach the servo to the used pin number
    Servo1.attach(servoPin);
}

void loop()
{
    // Make servo go to 0 degrees
    Servo1.write(0);
    delay(1000);
    // Make servo go to 90 degrees
    Servo1.write(90);
    delay(1000);
    // Make servo go to 180 degrees
    Servo1.write(180);
    delay(1000);
}
