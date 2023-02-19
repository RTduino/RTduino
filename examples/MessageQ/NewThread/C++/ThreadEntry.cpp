/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2023-02-17     Stanley Lwin      first version
 */

#include <rtthread.h>
#include "Common.h"

Data tObj;

/*Entry function for tid1*/
void thread_entry(void *parameter)
{

    while(1)
    {
        /* Receive messages from the message queue */
        if (rt_mq_recv(&mq, &tObj, sizeof(tObj), RT_WAITING_FOREVER) == RT_EOK)
        {
                tObj.displayData();
        }
    }
}

void Data::setData(float t, float h)
{
    temp = t;
    humidity = h;
}

void Data::displayData(void)
{
    rt_kprintf("Temperature: %f\n", temp);
    rt_kprintf("Humidity: %f\n", humidity);
}
