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

static void TC_pinmode_ard2rt(void)
{
    int error_mode = PIN_MODE_INPUT + 50;
    uassert_int_equal(LLT__ard2rt_pinmode(error_mode), -1);
    uassert_int_equal(LLT__ard2rt_pinmode(INPUT), PIN_MODE_INPUT);
    uassert_int_equal(LLT__ard2rt_pinmode(OUTPUT), PIN_MODE_OUTPUT);
    uassert_int_equal(LLT__ard2rt_pinmode(INPUT_PULLUP), PIN_MODE_INPUT_PULLUP);
    uassert_int_equal(LLT__ard2rt_pinmode(INPUT_PULLDOWN), PIN_MODE_INPUT_PULLDOWN);
    uassert_int_equal(LLT__ard2rt_pinmode(OUTPUT_OPEN_DRAIN), PIN_MODE_OUTPUT_OD);
}

static void TC_pinlevel_ard2rt(void)
{
    int error_val = PIN_HIGH + 50;
    uassert_int_equal(LLT__ard2rt_pinlevel(error_val), -1);
    uassert_int_equal(LLT__ard2rt_pinlevel(HIGH), PIN_HIGH);
    uassert_int_equal(LLT__ard2rt_pinlevel(LOW), PIN_LOW);
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_pinmode_ard2rt);
    UTEST_UNIT_RUN(TC_pinlevel_ard2rt);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.IO", RT_NULL, RT_NULL, 1000);
