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
static int _cmd_rtduino(int argc, char **argv)
{
#ifdef RTDUINO_CMD_USING_SERIAL
    if(rt_strcmp(argv[1], "serial") == 0)
    {
#ifdef RTDUINO_CMD_SERIAL_USING_FOCUSON /* rtduino serial focuson */
        if(rt_strcmp(argv[2], "focuson") == 0)
        {
            extern void _cmd_serial_foucson(void);
            _cmd_serial_foucson();
        }
#endif /* RTDUINO_CMD_SERIAL_USING_FOCUSON */
    }
#endif /* RTDUINO_CMD_USING_SERIAL */
    return 0;
}
MSH_CMD_EXPORT_ALIAS(_cmd_rtduino, rtduino, rtduino [option]);
#endif /* RT_USING_FINSH */
