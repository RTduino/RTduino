/*
 * Change Logs:
 * Date           Author       Notes
 * 02-02-2023     Stanley Lwin   first version
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

extern rt_sem_t msgsem; 

/*msg queue control block*/
extern struct rt_messagequeue mq;

/*the memory pool used to replace msg in the msg queue*/
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

