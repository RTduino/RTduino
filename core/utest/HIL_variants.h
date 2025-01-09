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

#ifndef __HIL_VARIANTS_H__
#define __HIL_VARIANTS_H__

#include <RTduino.h>
#include <rtconfig.h>

#if defined(BOARD_STM32L475_ATK_PANDORA)
#define HIL_DIGITAL_PIN_PULSE D13
#define HIL_DIGITAL_PIN_TARGET D12
#else
#define HIL_DIGITAL_PIN_PULSE D0
#define HIL_DIGITAL_PIN_TARGET D1
#endif

#ifndef HIL_LEVEL_ESTABLISH_MS
#define HIL_LEVEL_ESTABLISH_MS 1 /* the level establishment time in ms */
#endif /* HIL_LEVEL_ESTABLISH_MS */

#endif /* __HIL_VARIANTS_H__ */