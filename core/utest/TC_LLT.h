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

/* This header file is part of Arduino.h, and only can be included by Arduino.h */

#ifdef __cplusplus

void LLT__ard2rtt_serial_bound(uint32_t baud, uint8_t config, struct serial_configure &rt_config);

#endif /* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

rt_base_t LLT__ard2rt_pin_mode(uint8_t mode);
rt_base_t LLT__ard2rt_pin_level(uint8_t mode);
rt_base_t LLT__ard2rt_interrupt_mode(uint8_t mode);

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */
