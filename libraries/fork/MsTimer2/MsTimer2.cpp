/*
 * Copyright (c) 2021-2022, RT-Thread RTduino Development Team
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-23     Meco Man     first version
 */
/*
  MsTimer2.h - Using timer2 with 1ms resolution
  Javier Valencia <javiervalencia80@gmail.com>
  https://github.com/PaulStoffregen/MsTimer2

  History:
    6/Jun/14  - V0.7 added support for Teensy 3.0 & 3.1
    29/Dec/11 - V0.6 added support for ATmega32u4, AT90USB646, AT90USB1286 (paul@pjrc.com)
        some improvements added by Bill Perry
        note: uses timer4 on Atmega32u4
    29/May/09 - V0.5 added support for Atmega1280 (thanks to Manuel Negri)
    19/Mar/09 - V0.4 added support for ATmega328P (thanks to Jerome Despatis)
    11/Jun/08 - V0.3
        changes to allow working with different CPU frequencies
        added support for ATMega128 (using timer2)
        compatible with ATMega48/88/168/8
    10/May/08 - V0.2 added some security tests and volatile keywords
    9/May/08 - V0.1 released working on ATMEGA168 only

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
