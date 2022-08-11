/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-23     Meco Man     first version
 */

#ifndef MsTimer2_h
#define MsTimer2_h

#include <Arduino.h>

namespace MsTimer2{
    void set(unsigned long ms, void (*f)(void));
    void start();
    void stop();
}

#endif
