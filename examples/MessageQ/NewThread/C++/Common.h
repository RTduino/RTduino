/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author            Notes
 * 2023-02-17     Stanley Lwin      first version
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

/*New thread configuration*/
#define THREAD_PRIORITY 21
#define THREAD_STACK_SIZE 1024
#define THREAD_TIMESLICE 5

void thread_entry(void *parameter);

/*msg queue control block*/
extern struct rt_messagequeue mq;
extern rt_uint8_t msg_pool[2048];

class Data
{
    public:
        volatile float temp = 0.0;
        volatile float humidity = 0.0;
        void setData(float t, float h);
        void displayData(void);

};

#ifdef __cplusplus
}
#endif
#endif
