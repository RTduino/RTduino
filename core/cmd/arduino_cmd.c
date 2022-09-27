/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-26     Meco Man     first version
 */

#include <rtthread.h>

#ifdef RT_USING_FINSH
static int _cmd_arduino(int argc, char **argv)
{
    if(rt_strcmp(argv[1], "serial") == 0)
    {
        if(rt_strcmp(argv[2], "focuson") == 0) /* arduino serial focuson */
        {
            extern void _cmd_serial_foucson(void);
            _cmd_serial_foucson();
        }
    }
    return 0;
}
MSH_CMD_EXPORT_ALIAS(_cmd_arduino, arduino, arduino [option]);
#endif /* RT_USING_FINSH */
