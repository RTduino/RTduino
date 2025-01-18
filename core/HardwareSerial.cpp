/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-24     Meco Man     first version
 */

#include <rtdevice.h>
#include "HardwareSerial.h"

#define DBG_TAG    "RTduino.HardwareSerial"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

static void ard2rtt_serial_bound(uint32_t baud, uint8_t config, struct serial_configure &rt_config)
{
    switch(baud)
    {
    case 2400:
        rt_config.baud_rate = BAUD_RATE_2400;
        break;

    case 4800:
        rt_config.baud_rate = BAUD_RATE_4800;
        break;

    case 9600:
        rt_config.baud_rate = BAUD_RATE_9600;
        break;

    case 19200:
        rt_config.baud_rate = BAUD_RATE_19200;
        break;

    case 38400:
        rt_config.baud_rate = BAUD_RATE_38400;
        break;

    case 57600:
        rt_config.baud_rate = BAUD_RATE_57600;
        break;

    case 230400:
        rt_config.baud_rate = BAUD_RATE_230400;
        break;

    case 460800:
        rt_config.baud_rate = BAUD_RATE_460800;
        break;

    case 921600:
        rt_config.baud_rate = BAUD_RATE_921600;
        break;

    case 2000000:
        rt_config.baud_rate = BAUD_RATE_2000000;
        break;

    case 3000000:
        rt_config.baud_rate = BAUD_RATE_3000000;
        break;

    case 115200:
    default:
        rt_config.baud_rate = BAUD_RATE_115200;
        break;
    }

    switch(config)
    {
    case SERIAL_5N1:
        rt_config.data_bits = DATA_BITS_5;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_NONE;
        break;

    case SERIAL_6N1:
        rt_config.data_bits = DATA_BITS_6;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_NONE;
        break;

    case SERIAL_7N1:
        rt_config.data_bits = DATA_BITS_7;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_NONE;
        break;

    case SERIAL_5N2:
        rt_config.data_bits = DATA_BITS_5;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_NONE;
        break;

    case SERIAL_6N2:
        rt_config.data_bits = DATA_BITS_6;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_NONE;
        break;

    case SERIAL_7N2:
        rt_config.data_bits = DATA_BITS_7;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_NONE;
        break;

    case SERIAL_8N2:
        rt_config.data_bits = DATA_BITS_8;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_NONE;
        break;

    case SERIAL_5E1:
        rt_config.data_bits = DATA_BITS_5;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_EVEN;
        break;

    case SERIAL_6E1:
        rt_config.data_bits = DATA_BITS_6;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_EVEN;
        break;

    case SERIAL_7E1:
        rt_config.data_bits = DATA_BITS_7;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_EVEN;
        break;

    case SERIAL_8E1:
        rt_config.data_bits = DATA_BITS_8;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_EVEN;
        break;

    case SERIAL_5E2:
        rt_config.data_bits = DATA_BITS_5;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_EVEN;
        break;

    case SERIAL_6E2:
        rt_config.data_bits = DATA_BITS_6;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_EVEN;
        break;

    case SERIAL_7E2:
        rt_config.data_bits = DATA_BITS_7;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_EVEN;
        break;

    case SERIAL_8E2:
        rt_config.data_bits = DATA_BITS_8;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_EVEN;
        break;

    case SERIAL_5O1:
        rt_config.data_bits = DATA_BITS_5;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_ODD;
        break;

    case SERIAL_6O1:
        rt_config.data_bits = DATA_BITS_6;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_ODD;
        break;

    case SERIAL_7O1:
        rt_config.data_bits = DATA_BITS_7;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_ODD;
        break;

    case SERIAL_8O1:
        rt_config.data_bits = DATA_BITS_8;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_ODD;
        break;

    case SERIAL_5O2:
        rt_config.data_bits = DATA_BITS_5;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_ODD;
        break;

    case SERIAL_6O2:
        rt_config.data_bits = DATA_BITS_6;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_ODD;
        break;

    case SERIAL_7O2:
        rt_config.data_bits = DATA_BITS_7;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_ODD;
        break;

    case SERIAL_8O2:
        rt_config.data_bits = DATA_BITS_8;
        rt_config.stop_bits = STOP_BITS_2;
        rt_config.parity = PARITY_ODD;
        break;

    case SERIAL_8N1:
    default:
        rt_config.data_bits = DATA_BITS_8;
        rt_config.stop_bits = STOP_BITS_1;
        rt_config.parity = PARITY_NONE;
        break;
    }
}

HardwareSerial::HardwareSerial(rt_device_t device)
{
    this->uart_dev = device;
    if (this->uart_dev != RT_NULL)
    {
        LOG_D("HardwareSerial %s (console) initiate successfully", RT_CONSOLE_DEVICE_NAME);
    }
    else
    {
        LOG_E("HardwareSerial %s (console) initiate failure", RT_CONSOLE_DEVICE_NAME);
    }
}

HardwareSerial::HardwareSerial(const char* dev_name)
{
    rt_err_t err = -RT_ERROR;
    this->uart_dev = rt_device_find(dev_name);
    if (this->uart_dev != RT_NULL)
    {
        err = rt_device_open(this->uart_dev, RT_DEVICE_FLAG_INT_RX);
    }

    if (err == RT_EOK)
    {
        LOG_D("HardwareSerial %s initiate successfully", dev_name);
    }
    else
    {
        LOG_E("HardwareSerial %s initiate failure", dev_name);
    }
}

/*Code to display letter when given the ASCII code for it*/
size_t HardwareSerial::write(uint8_t c)
{
    return write(&c, 1);
}

/*Code to display array of chars when given a pointer to the beginning of the array and a size
    -- this will not end with the null character*/
size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
    return rt_device_write(this->uart_dev, 0, buffer, size);
}

void HardwareSerial::begin(void)
{
    /* Do nothing. Use RT-Thread default configurations: typically 115200, 8N1 */
}

void HardwareSerial::begin(uint32_t baud)
{
    begin(baud, SERIAL_8N1);
}

void HardwareSerial::begin(uint32_t baud, uint8_t config)
{
    struct serial_configure rt_config = RT_SERIAL_CONFIG_DEFAULT;
    ard2rtt_serial_bound(baud, config, rt_config);
    rt_device_control(this->uart_dev, RT_DEVICE_CTRL_CONFIG, &rt_config);
}

void HardwareSerial::end(void)
{
    // Nothing to do.
    // RTduino has not right to close RT-Thread devices.
    // This could cause other threads make errors.
}

int HardwareSerial::availableForWrite(void)
{
    return 0; //TODO
}

void HardwareSerial::flush(void)
{
    //TODO
}

int HardwareSerial::available(void)
{
    return 0; //TODO
}

int HardwareSerial::peek(void)
{
    return 0; //TODO
}

int HardwareSerial::read(void)
{
    return 0; //TODO
}

#ifdef RT_USING_CONSOLE
ConsoleSerial Serial;
#endif /* RT_USING_CONSOLE */
#ifdef RTDUINO_SERIAL2_DEVICE_NAME
HardwareSerial Serial2(RTDUINO_SERIAL2_DEVICE_NAME);
#endif /* RTDUINO_SERIAL2_DEVICE_NAME */
#ifdef RTDUINO_SERIAL3_DEVICE_NAME
HardwareSerial Serial3(RTDUINO_SERIAL3_DEVICE_NAME);
#endif /* RTDUINO_SERIAL3_DEVICE_NAME */

#if defined(RTDUINO_USING_UTEST) || defined(RT_UTEST_USING_ALL_CASES)
void LLT__ard2rtt_serial_bound(uint32_t baud, uint8_t config, struct serial_configure &rt_config)
{
    ard2rtt_serial_bound(baud, config, rt_config);
}
#endif /* RTDUINO_USING_UTEST || RT_UTEST_USING_ALL_CASES */
