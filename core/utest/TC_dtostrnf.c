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
 * 2025-01-05     Meco Man     first version
 */

#include <RTduino.h>
#include <utest.h>

static void TC_dtostrf(void)
{
    char buffer[50];
    dtostrf(123.456, 8, 3, buffer);
    uassert_str_equal(" 123.456", buffer);
    dtostrf(123.456, 6, 1, buffer);
    uassert_str_equal(" 123.5", buffer);
    dtostrf(7.123456, 5, 4, buffer);
    uassert_str_equal("7.1235", buffer);
    dtostrf(123.456, 10, 2, buffer);
    uassert_str_equal("    123.46", buffer);
    dtostrf(-123.456, 10, 2, buffer);
    uassert_str_equal("   -123.46", buffer);
    dtostrf(0.0, 5, 2, buffer);
    uassert_str_equal(" 0.00", buffer);
    dtostrf(12.34, 10, 5, buffer);
    uassert_str_equal("  12.34000", buffer);
    dtostrf(123.456, 5, 2, buffer);
    uassert_str_equal("123.46", buffer);
    dtostrf(123.456, -10, 2, buffer);
    uassert_str_equal("123.46    ", buffer);
    dtostrf(123456789.12345, 15, 3, buffer);
    uassert_str_equal("  123456789.123", buffer);
    dtostrf(0.000123, 10, 6, buffer);
    uassert_str_equal("  0.000123", buffer);
    dtostrf(123.456, 0, 2, buffer);
    uassert_str_equal("123.46", buffer);
    dtostrf(123.0, 10, 0, buffer);
    uassert_str_equal("       123", buffer);
    dtostrf(0.0000001234, 15, 8, buffer);
    uassert_str_equal("     0.00000012", buffer);
    dtostrf(12345678.9, 10, 0, buffer);
    uassert_str_equal("  12345679", buffer);
    dtostrf(-0.000123456, 10, 6, buffer);
    uassert_str_equal(" -0.000123", buffer);
    dtostrf(12345.6789, 5, 2, buffer);
    uassert_str_equal("12345.68", buffer);
    dtostrf(123.45, 6, 2, buffer);
    uassert_str_equal("123.45", buffer);
    dtostrf(1234567890123.456, 20, 2, buffer);
    // uassert_str_equal("    1234567890123.46", buffer);
    dtostrf(1000, 5, 0, buffer);
    uassert_str_equal(" 1000", buffer);
    dtostrf(-1234.56, 5, 2, buffer);
    uassert_str_equal("-1234.56", buffer);
    dtostrf(123.4, 8, 5, buffer);
    uassert_str_equal("123.40000", buffer);
    dtostrf(123.456, 0, 0, buffer);
    uassert_str_equal("123", buffer);
    dtostrf(123.456, -5, 2, buffer);
    uassert_str_equal("123.46", buffer);
    dtostrf(-0.000000001, 15, 9, buffer);
    uassert_str_equal("   -0.000000001", buffer);
    dtostrf(9876543210.123, 15, 1, buffer);
    // uassert_str_equal("   9876543210.1", buffer);
}

static void TC_dtostrnf(void)
{
    char buffer[50];
    dtostrnf(123.456, 8, 3, buffer, sizeof(buffer));
    uassert_str_equal(" 123.456", buffer);
    dtostrnf(123.456, 6, 1, buffer, sizeof(buffer));
    uassert_str_equal(" 123.5", buffer);
    dtostrnf(7.123456, 5, 4, buffer, sizeof(buffer));
    uassert_str_equal("7.1235", buffer);
    dtostrnf(123.456, 10, 2, buffer, sizeof(buffer));
    uassert_str_equal("    123.46", buffer);
    dtostrnf(-123.456, 10, 2, buffer, sizeof(buffer));
    uassert_str_equal("   -123.46", buffer);
    dtostrnf(0.0, 5, 2, buffer, sizeof(buffer));
    uassert_str_equal(" 0.00", buffer);
    dtostrnf(12.34, 10, 5, buffer, sizeof(buffer));
    uassert_str_equal("  12.34000", buffer);
    dtostrnf(123.456, 5, 2, buffer, sizeof(buffer));
    uassert_str_equal("123.46", buffer);
    dtostrnf(123.456, -10, 2, buffer, sizeof(buffer));
    uassert_str_equal("123.46    ", buffer);
    dtostrnf(123456789.12345, 15, 3, buffer, sizeof(buffer));
    uassert_str_equal("  123456789.123", buffer);
    dtostrnf(0.000123, 10, 6, buffer, sizeof(buffer));
    uassert_str_equal("  0.000123", buffer);
    dtostrnf(123.456, 0, 2, buffer, sizeof(buffer));
    uassert_str_equal("123.46", buffer);
    dtostrnf(123.0, 10, 0, buffer, sizeof(buffer));
    uassert_str_equal("       123", buffer);
    dtostrnf(0.0000001234, 15, 8, buffer, sizeof(buffer));
    uassert_str_equal("     0.00000012", buffer);
    dtostrnf(12345678.9, 10, 0, buffer, sizeof(buffer));
    uassert_str_equal("  12345679", buffer);
    dtostrnf(-0.000123456, 10, 6, buffer, sizeof(buffer));
    uassert_str_equal(" -0.000123", buffer);
    dtostrnf(12345.6789, 5, 2, buffer, sizeof(buffer));
    uassert_str_equal("12345.68", buffer);
    dtostrnf(123.45, 6, 2, buffer, sizeof(buffer));
    uassert_str_equal("123.45", buffer);
    dtostrnf(1234567890123.456, 20, 2, buffer, sizeof(buffer));
    // uassert_str_equal("    1234567890123.46", buffer);
    dtostrnf(1000, 5, 0, buffer, sizeof(buffer));
    // uassert_str_equal(" 1000", buffer);
    dtostrnf(-1234.56, 5, 2, buffer, sizeof(buffer));
    uassert_str_equal("-1234.56", buffer);
    dtostrnf(123.4, 8, 5, buffer, sizeof(buffer));
    uassert_str_equal("123.40000", buffer);
    dtostrnf(123.456, 0, 0, buffer, sizeof(buffer));
    uassert_str_equal("123", buffer);
    dtostrnf(123.456, -5, 2, buffer, sizeof(buffer));
    uassert_str_equal("123.46", buffer);
    dtostrnf(-0.000000001, 15, 9, buffer, sizeof(buffer));
    uassert_str_equal("   -0.000000001", buffer);
    dtostrnf(9876543210.123, 15, 1, buffer, sizeof(buffer));
    // uassert_str_equal("   9876543210.1", buffer);
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_dtostrf);
    UTEST_UNIT_RUN(TC_dtostrnf);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.dtostrnf", RT_NULL, RT_NULL, 1000);
