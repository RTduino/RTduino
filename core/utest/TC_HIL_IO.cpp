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

static void TC_digital(void)
{
    pinMode(D13, INPUT);
    pinMode(D12, OUTPUT);

    digitalWrite(D12, LOW);
    rt_thread_delay(5);
    uassert_true(digitalRead(D13) == LOW);

    digitalWrite(D12, HIGH);
    rt_thread_delay(5);
    uassert_true(digitalRead(D13) == HIGH);
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_digital);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.HIL.IO", RT_NULL, RT_NULL, 1000);
