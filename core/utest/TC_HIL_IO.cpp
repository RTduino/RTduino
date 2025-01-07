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
    digitalWrite(D3, LOW);
    uassert_true(digitalRead(D3) == LOW);
    digitalWrite(D3, HIGH);
    uassert_true(digitalRead(D3) == HIGH);
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_digital);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.HIL.IO", RT_NULL, RT_NULL, 1000);
