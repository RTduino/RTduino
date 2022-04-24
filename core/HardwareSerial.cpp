/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-24     Meco Man     first version
 */

#include <rtdevice.h>
#include "HardwareSerial.h"

HardwareSerial::HardwareSerial(void)
{
    this->uart_dev = rt_console_get_device();
}

HardwareSerial::HardwareSerial(const char* dev_name)
{
    this->uart_dev = rt_device_find(dev_name);
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

void HardwareSerial::begin(uint32_t baud)
{
    begin(baud, SERIAL_8N1);
}

void HardwareSerial::begin(uint32_t baud, uint8_t config)
{
    struct serial_configure rt_config = RT_SERIAL_CONFIG_DEFAULT;

    switch(baud)
    {
    case 2400:
        rt_config.baud_rate = BAUD_RATE_2400;

    case 4800:
        rt_config.baud_rate = BAUD_RATE_4800;

    case 9600:
        rt_config.baud_rate = BAUD_RATE_9600;

    case 19200:
        rt_config.baud_rate = BAUD_RATE_19200;

    case 38400:
        rt_config.baud_rate = BAUD_RATE_38400;

    case 57600:
        rt_config.baud_rate = BAUD_RATE_57600;

    case 230400:
        rt_config.baud_rate = BAUD_RATE_230400;

    case 460800:
        rt_config.baud_rate = BAUD_RATE_460800;

    case 921600:
        rt_config.baud_rate = BAUD_RATE_921600;

    case 2000000:
        rt_config.baud_rate = BAUD_RATE_2000000;

    case 3000000:
        rt_config.baud_rate = BAUD_RATE_3000000;

    case 115200:
    default:
        rt_config.baud_rate = BAUD_RATE_115200;
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

    rt_device_control(this->uart_dev, RT_DEVICE_CTRL_CONFIG, &rt_config);
}

void HardwareSerial::end(void)
{
    // Nothing to do.
    // RTduino has not right to close RT-Thread devices.
    // This could cause other threads make errors.
}

#ifdef RT_USING_CONSOLE
HardwareSerial Serial;
#endif
#ifdef RTDUINO_SERIAL2_DEVICE_NAME
HardwareSerial Serial2(RTDUINO_SERIAL2_DEVICE_NAME);
#endif
#ifdef RTDUINO_SERIAL3_DEVICE_NAME
HardwareSerial Serial3(RTDUINO_SERIAL3_DEVICE_NAME);
#endif
