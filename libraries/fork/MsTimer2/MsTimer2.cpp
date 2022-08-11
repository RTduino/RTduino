/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-23     Meco Man     first version
 */

#include "MsTimer2.h"
#include <rtthread.h>

static struct rt_timer mstimer2;

void MsTimer2::set(unsigned long ms, void (*f)(void)) {
    rt_timer_init(&mstimer2, "mstimer2", (void (*)(void *parameter))f,
            RT_NULL, ms, RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);
}

void MsTimer2::start() {
    rt_timer_start(&mstimer2);
}

void MsTimer2::stop() {
    rt_timer_stop(&mstimer2);
    rt_timer_detach(&mstimer2);
}
