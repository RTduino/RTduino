/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-26     Meco Man     first version
 */

#include <Arduino.h>

#ifdef RTDUINO_CMD_SERIAL_USING_FOCUSON

int ConsoleSerial::available(void)
{
    return 0;
}

int ConsoleSerial::peek(void)
{
    return 0;
}

int ConsoleSerial::read(void)
{
    return 0;
}

#endif /* RTDUINO_CMD_SERIAL_USING_FOCUSON */
