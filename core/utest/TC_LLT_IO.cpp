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

static void TC_ard2rt_pin_mode(void)
{
    int error_mode = PIN_MODE_INPUT + 50;
    uassert_int_equal(LLT__ard2rt_pin_mode(error_mode), -1);
    uassert_int_equal(LLT__ard2rt_pin_mode(INPUT), PIN_MODE_INPUT);
    uassert_int_equal(LLT__ard2rt_pin_mode(OUTPUT), PIN_MODE_OUTPUT);
    uassert_int_equal(LLT__ard2rt_pin_mode(INPUT_PULLUP), PIN_MODE_INPUT_PULLUP);
    uassert_int_equal(LLT__ard2rt_pin_mode(INPUT_PULLDOWN), PIN_MODE_INPUT_PULLDOWN);
    uassert_int_equal(LLT__ard2rt_pin_mode(OUTPUT_OPEN_DRAIN), PIN_MODE_OUTPUT_OD);
}

static void TC_ard2rt_pin_level(void)
{
    int error_val = PIN_HIGH + 50;
    uassert_int_equal(LLT__ard2rt_pin_level(error_val), -1);
    uassert_int_equal(LLT__ard2rt_pin_level(HIGH), PIN_HIGH);
    uassert_int_equal(LLT__ard2rt_pin_level(LOW), PIN_LOW);
}

static void TC_ard2rt_interrupt_mode(void)
{
    int error_mode = PIN_MODE_INPUT + 50;
    uassert_int_equal(LLT__ard2rt_interrupt_mode(error_mode), -1);
    uassert_int_equal(LLT__ard2rt_interrupt_mode(LOW), PIN_IRQ_MODE_LOW_LEVEL);
    uassert_int_equal(LLT__ard2rt_interrupt_mode(HIGH), PIN_IRQ_MODE_HIGH_LEVEL);
    uassert_int_equal(LLT__ard2rt_interrupt_mode(CHANGE), PIN_IRQ_MODE_RISING_FALLING);
    uassert_int_equal(LLT__ard2rt_interrupt_mode(RISING), PIN_IRQ_MODE_RISING);
    uassert_int_equal(LLT__ard2rt_interrupt_mode(FALLING), PIN_IRQ_MODE_FALLING);
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_ard2rt_pin_mode);
    UTEST_UNIT_RUN(TC_ard2rt_pin_level);
    UTEST_UNIT_RUN(TC_ard2rt_interrupt_mode);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.LLT.IO", RT_NULL, RT_NULL, 1000);
