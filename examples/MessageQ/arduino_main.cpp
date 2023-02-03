/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       		Notes
 * 2023-02-03     Stanley Lwin     	first version
 */
 
#include <Adafruit_AHTX0.h>
#include <rtthread.h>
#include <common.h>

Adafruit_AHTX0 aht;
rt_sem_t msgsem;
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
    rt_mq_init(&mq,"msgQ",&msg_pool[0],sizeof(struct data),sizeof(msg_pool),RT_IPC_FLAG_FIFO);

}

void loop() 
{
    sensors_event_t humidity, temp;
    struct data Data;
  
    aht.getEvent(&humidity, &temp);
  
    Data.temp= temp.temperature;
    Data.humidity = humidity.relative_humidity;

    rt_mq_send(&mq,&Data, sizeof(struct data));

    delay(500);
}



