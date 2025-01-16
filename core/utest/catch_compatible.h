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

#ifndef __CATCH_COMPATIBLE_H__
#define __CATCH_COMPATIBLE_H__

#include <utest.h>

#define TEST_CASE(name, tags) LOG_I("[==========] %s %s", tags, name);
#define WHEN(name)  LOG_I("[==========] [   WHEN   ] %s", name);
#define THEN(name)  LOG_I("[==========] [   THEN   ] %s", name);
#define GIVEN(name) LOG_I("[==========] [  GIVEN   ] %s", name);

#define REQUIRE(x)  uassert_true(x)
#define REQUIRE_FALSE(x)  uassert_false(x)

#endif /* __CATCH_COMPATIBLE_H__ */
