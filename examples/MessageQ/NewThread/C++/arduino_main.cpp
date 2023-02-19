/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2023-02-17     Stanley Lwin      first version
 */

#include <Adafruit_AHTX0.h>
#include <Common.h>
#include <rtthread.h>

rt_thread_t tid = RT_NULL;
Adafruit_AHTX0 aht;
Data obj;

struct rt_messagequeue mq;
rt_uint8_t msg_pool[2048];

void setup() {
    Serial.begin();
    Serial.println("Adafruit AHT10/AHT20 demo!");

    if (! aht.begin())
    {
        Serial.println("Could not find AHT? Check wiring");
        while (1) delay(10);
    }

    Serial.println("AHT10 or AHT20 found");

    rt_mq_init(&mq,"msgQ",&msg_pool[0],sizeof(obj),sizeof(msg_pool),RT_IPC_FLAG_FIFO);

    tid = rt_thread_create("thread", thread_entry, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);

    if(tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        rt_thread_delete(tid);
    }
}

void loop()
{
    sensors_event_t humidity, temp;

    aht.getEvent(&humidity, &temp);

    obj.setData(temp.temperature, humidity.relative_humidity);
    rt_mq_send(&mq, &obj, sizeof(obj));

    delay(500);
}
