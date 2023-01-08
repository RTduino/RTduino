/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-07     Meco Man     first version
 */

#ifndef WInterrupts_h
#define WInterrupts_h

#include <rthw.h>

class rtduino_noInterrupts
{
    private:
        rt_base_t rtduino_interrupt_nesting;

    public:
        rtduino_noInterrupts()
        {
            rtduino_interrupt_nesting = rt_hw_interrupt_disable();
        }

        ~rtduino_noInterrupts()
        {
            rt_hw_interrupt_enable(rtduino_interrupt_nesting);
        }
};

#define noInterrupts() rtduino_noInterrupts *rtduino_noInterrupts_obj = new rtduino_noInterrupts()
#define interrupts()   delete rtduino_noInterrupts_obj

#endif /* WInterrupts_h */
