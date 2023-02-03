/*
 * Change Logs:
 * Date           Author       Notes
 * 02-02-2023     Stanley Lwin   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <common.h>

/* defined the LD2 (user LED) pin: PB1 */
#define LED2_PIN    GET_PIN(A, 5)

int main(void)
{
    rt_thread_mdelay(1000);

    struct data mainData;
    while (1)
    {
		/* Receive messages from the message queue */
        if (rt_mq_recv(&mq, &mainData, sizeof(struct data), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("Temp: %f\n",mainData.temp);
            rt_kprintf("Humidity: %f\n",mainData.humidity);

        }
    }
}
