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
#include <WString.h>
#include <Print.h>
#include <Printable.h>

#include "catch_compatible.h"
#include "Print/mock/PrintMock.h"
#include "Print/mock/PrintableMock.h"

static void TC_availableForWrite(void)
{
    #include "Print/test_availableForWrite.cpp"
}

static void TC_clearWriteError(void)
{
    #include "Print/test_clearWriteError.cpp"
}

static void TC_getWriteError(void)
{
    #include "Print/test_getWriteError.cpp"
}

static void TC_print(void)
{
    #include "Print/test_print.cpp"
}

static void TC_println(void)
{
    #include "Print/test_println.cpp"
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_availableForWrite);
    UTEST_UNIT_RUN(TC_clearWriteError);
    UTEST_UNIT_RUN(TC_getWriteError);
    UTEST_UNIT_RUN(TC_print);
    UTEST_UNIT_RUN(TC_println);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.Print", RT_NULL, RT_NULL, 1000);
