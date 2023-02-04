/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       		Notes
 * 2023-02-04     Stanley Lwin     	first version
 */
 
#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

extern rt_sem_t msgsem; 

/*msg queue control block*/
extern struct rt_messagequeue mq;
extern rt_uint8_t msg_pool[2048];

struct data{
 float temp;
 float humidity;
};
typedef struct data Data;

#ifdef __cplusplus
}
#endif
#endif

