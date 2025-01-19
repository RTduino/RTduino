/*
 * Copyright (c) 2021-2025, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-01-04     Meco Man     first version
 */
#include <RTduino.h>
#include <utest.h>

#include "TC_HIL.h"

static void TC_digital(void)
{
    pinMode(HIL_DIGITAL_PIN_TARGET, INPUT);

    pinMode(HIL_DIGITAL_PIN_PULSE, PIN_MODE_INPUT_PULLUP);
    rt_thread_mdelay(HIL_LEVEL_ESTABLISH_MS);
    uassert_true(digitalRead(HIL_DIGITAL_PIN_TARGET) == HIGH);

    pinMode(HIL_DIGITAL_PIN_PULSE, PIN_MODE_INPUT_PULLDOWN);
    rt_thread_mdelay(HIL_LEVEL_ESTABLISH_MS);
    uassert_true(digitalRead(HIL_DIGITAL_PIN_TARGET) == LOW);

    pinMode(HIL_DIGITAL_PIN_PULSE, OUTPUT);

    digitalWrite(HIL_DIGITAL_PIN_PULSE, LOW);
    rt_thread_mdelay(HIL_LEVEL_ESTABLISH_MS);
    uassert_true(digitalRead(HIL_DIGITAL_PIN_TARGET) == LOW);

    digitalWrite(HIL_DIGITAL_PIN_PULSE, HIGH);
    rt_thread_mdelay(HIL_LEVEL_ESTABLISH_MS);
    uassert_true(digitalRead(HIL_DIGITAL_PIN_TARGET) == HIGH);
}

static volatile rt_bool_t interrupted_flag = RT_FALSE;

void _cb_pin_interrupt(void)
{
    interrupted_flag = RT_TRUE;
}

#undef HIL_DIGITAL_PIN_TARGET
#define HIL_DIGITAL_PIN_TARGET D16

static void TC_interrupt(void)
{
    pinMode(HIL_DIGITAL_PIN_PULSE, OUTPUT);
    pinMode(HIL_DIGITAL_PIN_TARGET, INPUT);

    digitalWrite(HIL_DIGITAL_PIN_PULSE, HIGH);
    rt_thread_mdelay(HIL_LEVEL_ESTABLISH_MS);

    interrupted_flag = RT_FALSE;
    attachInterrupt(digitalPinToInterrupt(HIL_DIGITAL_PIN_TARGET), _cb_pin_interrupt, LOW);
    rt_thread_mdelay(HIL_LEVEL_ESTABLISH_MS);
    digitalWrite(HIL_DIGITAL_PIN_PULSE, LOW);
    rt_thread_mdelay(HIL_LEVEL_ESTABLISH_MS);
    uassert_true(interrupted_flag);

    interrupted_flag = RT_FALSE;
    attachInterrupt(digitalPinToInterrupt(HIL_DIGITAL_PIN_TARGET), _cb_pin_interrupt, HIGH);
    rt_thread_mdelay(HIL_LEVEL_ESTABLISH_MS);
    digitalWrite(HIL_DIGITAL_PIN_PULSE, HIGH);
    rt_thread_mdelay(HIL_LEVEL_ESTABLISH_MS);
    uassert_true(interrupted_flag);
}


/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_digital);
    //UTEST_UNIT_RUN(TC_interrupt);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.HIL.IO", RT_NULL, RT_NULL, 1000);
