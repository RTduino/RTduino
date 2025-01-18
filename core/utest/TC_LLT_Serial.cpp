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
 * 2025-01-18     Meco Man     first version
 */
#include <RTduino.h>
#include <utest.h>

static void TC_ard2rtt_serial_bound(void)
{
    struct serial_configure rt_config = {0};

    /* undefined bond rate should be 115200 by default */
    LLT__ard2rtt_serial_bound(123456, SERIAL_8N1, rt_config);
    uassert_int_equal(BAUD_RATE_115200, rt_config.baud_rate);
    uassert_int_equal(DATA_BITS_8, rt_config.data_bits);
    uassert_int_equal(STOP_BITS_1, rt_config.stop_bits);
    uassert_int_equal(PARITY_NONE, rt_config.parity);

    /* test baud rate 2400, SERIAL_8N1 */
    LLT__ard2rtt_serial_bound(2400, SERIAL_8N1, rt_config);
    uassert_int_equal(BAUD_RATE_2400, rt_config.baud_rate);
    uassert_int_equal(DATA_BITS_8, rt_config.data_bits);
    uassert_int_equal(STOP_BITS_1, rt_config.stop_bits);
    uassert_int_equal(PARITY_NONE, rt_config.parity);
 
    /* test baud rate 4800, SERIAL_8N1 */
    LLT__ard2rtt_serial_bound(9600, SERIAL_7E2, rt_config);
    uassert_int_equal(BAUD_RATE_9600, rt_config.baud_rate);
    uassert_int_equal(DATA_BITS_7, rt_config.data_bits);
    uassert_int_equal(STOP_BITS_2, rt_config.stop_bits);
    uassert_int_equal(PARITY_EVEN, rt_config.parity);
 
    /* test baud rate 115200, SERIAL_5O1 */
    LLT__ard2rtt_serial_bound(115200, SERIAL_5O1, rt_config);
    uassert_int_equal(BAUD_RATE_115200, rt_config.baud_rate);
    uassert_int_equal(DATA_BITS_5, rt_config.data_bits);
    uassert_int_equal(STOP_BITS_1, rt_config.stop_bits);
    uassert_int_equal(PARITY_ODD, rt_config.parity);
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_ard2rtt_serial_bound);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.LLT.Serial", RT_NULL, RT_NULL, 1000);
