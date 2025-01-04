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

static void TC_makeWord(void)
{
    uassert_int_equal(makeWord(0xDEAD), 0xDEAD);
    uassert_int_equal(makeWord(0xDE), 0x00DE);
    uassert_int_equal(makeWord(0xDE, 0xAD), 0xDEAD);
}

static void TC_word(void)
{
    uassert_int_equal(word(0xDEAD), 0xDEAD);
    uassert_int_equal(word(0xDE), 0x00DE);
    uassert_int_equal(word(0xDE, 0xAD), 0xDEAD);
}

static void TC_map(void)
{
    uassert_int_equal(map(50, 0, 100, 0, 10), 5);
    uassert_int_equal(map(5, 0, 10, 0, 100), 50);
    uassert_int_equal(map(1, 0, 100, 0, 10), 0);
    uassert_int_equal(map(0, 0, -256, 0, 1024), 0);
    uassert_int_equal(map(-256, 0, -256, 0, 1024), 1024);
    uassert_int_equal(map(-128, 0, -256, 0, 1024), 512);
    uassert_int_equal(map(0, 0, 256, 0, -1024), 0);
    uassert_int_equal(map(256, 0, 256, 0, -1024), -1024);
    uassert_int_equal(map(128, 0, 256, 0, -1024), -512);
}

static void TC_random(void)
{
    /* Test randomSeed with a known seed */
    randomSeed(1);

    /* Test random(long howbig) */
    long result = random(10);
    uassert_in_range(result, 0, 9); /* The result should be between 0 and 9 inclusive */

    result = random(1);
    uassert_int_equal(result, 0); /* The result should be 0 since 1 % 1 is 0 */

    result = random(100);
    uassert_in_range(result, 0, 99); /* The result should be between 0 and 99 inclusive */

    /* Test random(long howsmall, long howbig) */
    result = random(5, 15);
    uassert_in_range(result, 5, 14); /* The result should be between 5 and 14 inclusive */

    result = random(10, 10);
    uassert_int_equal(result, 10); /* The result should be 10 since howsmall is equal to howbig */

    result = random(15, 5);
    uassert_int_equal(result, 15); /* The result should be 15 since howsmall is greater than howbig */

    /* Test edge cases */
    result = random(0, 0);
    uassert_int_equal(result, 0); /* Both howsmall and howbig are 0, should return 0 */

    result = random(-5, 5);
    uassert_in_range(result, -5, 4); /* Negative range test */

    result = random(1000);
    uassert_in_range(result, 0, 999); /* Ensure large range works */

    result = random(990, 1000);
    uassert_in_range(result, 990, 999); /* Small range test */
}

static void TC_constrain(void)
{
    /* Test values within the range */
    uassert_int_equal(constrain(50, 0, 100), 50);

    /* Test value below the range */
    uassert_int_equal(constrain(-10, 0, 100), 0);

    /* Test value above the range */
    uassert_int_equal(constrain(150, 0, 100), 100);

    /* Test boundary values */
    uassert_int_equal(constrain(0, 0, 100), 0);
    uassert_int_equal(constrain(100, 0, 100), 100);
}

static void TC_radians(void)
{
    /* Test case 1: 0 degrees should be 0 radians */
    double result = radians(0);
    uassert_float_equal(result, 0.0); /* Exact match for 0 radians */

    /* Test case 2: 90 degrees should be approximately PI/2 radians */
    result = radians(90);
    uassert_float_equal(result, HALF_PI); /* Allowing a small tolerance for floating-point comparison */

    /* Test case 3: 180 degrees should be approximately PI radians */
    result = radians(180);
    uassert_float_equal(result, PI); /* Allowing a small tolerance */

    /* Test case 4: 360 degrees should be approximately 2*PI radians */
    result = radians(360);
    uassert_float_equal(result, TWO_PI); /* Allowing a small tolerance */

    /* Additional test case: -90 degrees should be approximately -PI/2 radians */
    result = radians(-90);
    uassert_float_equal(result, -HALF_PI); /* Allowing a small tolerance */
}

static void TC_degrees(void)
{
    /* Test case 1: 0 radians should be 0 degrees */
    double result = degrees(0);
    uassert_float_equal(result, 0.0); /* Exact match for 0 degrees */

    /* Test case 2: PI/2 radians should be approximately 90 degrees */
    result = degrees(HALF_PI);
    uassert_float_equal(result, 90.0); /* Allowing a small tolerance for floating-point comparison */

    /* Test case 3: PI radians should be approximately 180 degrees */
    result = degrees(PI);
    uassert_float_equal(result, 180.0); /* Allowing a small tolerance */

    /* Test case 4: 2*PI radians should be approximately 360 degrees */
    result = degrees(TWO_PI);
    uassert_float_equal(result, 360.0); /* Allowing a small tolerance */

    /* Additional test case: -PI/2 radians should be approximately -90 degrees */
    result = degrees(-HALF_PI);
    uassert_float_equal(result, -90.0); /* Allowing a small tolerance */
}

static void TC_sq(void)
{
    int rst_int = 0;
    float rst_float = 0.0f;

    /* Test with positive integer */
    rst_int = sq(3);
    uassert_int_equal(rst_int, 9); /* Expected 3^2 = 9 */

    /* Test with negative integer */
    rst_int = sq(-2);
    uassert_int_equal(rst_int, 4); /* Expected (-2)^2 = 4 */

    /* Test with zero */
    rst_int = sq(0);
    uassert_int_equal(rst_int, 0); /* Expected 0^2 = 0 */

    /* Test to ensure inequality for a wrong result */
    rst_int = sq(4);
    uassert_int_not_equal(rst_int, 15); /* 4^2 should not be 15, it should be 16 */

    /* Test with positive floating-point number */
    rst_float = sq(2.5f);
    uassert_float_equal(rst_float, 6.25); /* Expected 2.5^2 = 6.25, allowing for minor floating-point precision differences */

    /* Test with negative floating-point number */
    rst_float = sq(-1.7f);
    uassert_float_equal(rst_float, 2.89); /* Expected (-1.7)^2 = 2.89 */

    /* Test with zero */
    rst_float = sq(0.0f);
    uassert_float_equal(rst_float, 0); /* Expected 0.0^2 = 0.0 */

    /* Test to ensure inequality for a wrong result (using not_in_range for a clearly wrong value) */
    rst_float = sq(3.0f);
    uassert_float_equal(rst_float, 9); /* 3.0^2 should not be in the range 8.9 to 9.1, it should be 9.0 */
}

#if defined(min) && !defined(RTDUINO_NO_MIN)
static void TC_min(void)
{
    uassert_int_equal(min(5, 10), 5);
    uassert_int_equal(min(10, 5), 5);
    uassert_int_equal(min(5, 5), 5);
    uassert_int_equal(min(-5, 10), -5);
    uassert_int_equal(min(-10, -5), -10);
    uassert_int_equal(min(0, 0), 0);
}
#endif /* defined(min) && !defined(RTDUINO_NO_MIN) */

#if defined(max) && !defined(RTDUINO_NO_MAX)
static void TC_max(void)
{
    uassert_int_equal(max(5, 10), 10);
    uassert_int_equal(max(10, 5), 10);
    uassert_int_equal(max(5, 5), 5);
    uassert_int_equal(max(-5, 10), 10);
    uassert_int_equal(max(-10, -5), -5);
    uassert_int_equal(max(0, 0), 0);
}
#endif /* defined(max) && !defined(RTDUINO_NO_MAX) */

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_makeWord);
    UTEST_UNIT_RUN(TC_word);
    UTEST_UNIT_RUN(TC_map);
    UTEST_UNIT_RUN(TC_random);
    UTEST_UNIT_RUN(TC_constrain);
    UTEST_UNIT_RUN(TC_radians);
    UTEST_UNIT_RUN(TC_degrees);
    UTEST_UNIT_RUN(TC_sq);
#if defined(min) && !defined(RTDUINO_NO_MIN)
    UTEST_UNIT_RUN(TC_min);
#endif /* defined(min) && !defined(RTDUINO_NO_MIN) */
#if defined(max) && !defined(RTDUINO_NO_MAX)
    UTEST_UNIT_RUN(TC_max);
#endif /* defined(max) && !defined(RTDUINO_NO_MAX) */
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.WMath", RT_NULL, RT_NULL, 1000);
