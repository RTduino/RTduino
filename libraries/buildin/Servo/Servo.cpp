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
 * 2021-12-10     Meco Man     first version
 */

/*
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <Arduino.h>
#include "Servo.h"
#include <rtdevice.h>
#include <wiring_private.h>

#define DBG_TAG    "RTduino.Servo"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo in us
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo in us
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached in us

#define SERVO_MIN()         (this->min)   // minimum value in us for this servo in us
#define SERVO_MAX()         (this->max)   // maximum value in us for this servo in us

#define SERVO_PWM_HZ        50 /*20ms*/
#define PWM_PERIOD_NS       1000*1000*1000/*ns*//SERVO_PWM_HZ

Servo::Servo(void)
{
    this->servo_info.Pin.isActive = false;
    this->servo_info.Pin.nbr = 0;
    this->servo_info.rt_dev = RT_NULL;
    this->servo_info.pulsewidth = 0;
    this->max = 0;
    this->min = 0;
}

uint8_t Servo::attach(int pin)
{
    return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

// pulse width's min and max in us
// attach the given pin to the next free channel
// returns pin number or INVALID_SERVO if failure
uint8_t Servo::attach(int pin, int min, int max)
{
    struct rt_device_pwm *pwm_dev;
    int pwm_channel;

    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin, this->servo_info.Pin.nbr); /* with return value */

    pwm_dev = (struct rt_device_pwm*)rt_device_find(pin_map_table[pin].device_name);
    if(pwm_dev == RT_NULL)
    {
        LOG_E("Pin %d doesn't support PWM", pin);
        return INVALID_SERVO;
    }

    if(this->servo_info.Pin.isActive == false)
    {
        this->servo_info.Pin.nbr = pin;

        /*pulse width in us*/
        this->min  = min;
        this->max  = max;

        pwm_channel = pin_map_table[this->servo_info.Pin.nbr].channel;
        rt_pwm_disable(pwm_dev, pwm_channel);
        rt_pwm_set(pwm_dev, pwm_channel, PWM_PERIOD_NS, DEFAULT_PULSE_WIDTH * 1000/*ns*/);
        rt_pwm_enable(pwm_dev, pwm_channel);

        this->servo_info.rt_dev = pwm_dev;
        this->servo_info.pulsewidth = DEFAULT_PULSE_WIDTH;   // store default values
        this->servo_info.Pin.isActive = true;
    }
    else
    {
        LOG_E("This servo has been used!");
    }

    return this->servo_info.Pin.nbr;
}

void Servo::detach(void)
{
    if(this->servo_info.Pin.isActive == true)
    {
        rt_pwm_disable(this->servo_info.rt_dev, pin_map_table[this->servo_info.Pin.nbr].channel);
        this->servo_info.Pin.isActive = false;
    }
}

// angle in degree 0-180
void Servo::write(int angle)
{
    int pulsewidth;
    if(angle < 0) angle = 0;
    if(angle > 180) angle = 180;
    pulsewidth = map(angle, 0, 180, SERVO_MIN(), SERVO_MAX());
    this->writeMicroseconds(pulsewidth);
}

// value in ms
void Servo::writeMicroseconds(int pulsewidth)
{
    // calculate and store the values for the given channel
    int pwm_channel;
    struct rt_device_pwm *pwm_dev;

    if(this->servo_info.Pin.isActive == true)   // ensure channel is valid
    {
        if(pulsewidth < SERVO_MIN())          // ensure pulse width is valid
            pulsewidth = SERVO_MIN();
        else if(pulsewidth > SERVO_MAX())
            pulsewidth = SERVO_MAX();

        this->servo_info.pulsewidth = pulsewidth;

        pwm_channel = pin_map_table[this->servo_info.Pin.nbr].channel;
        pwm_dev = this->servo_info.rt_dev;

        rt_pwm_disable(pwm_dev, pwm_channel);
        rt_pwm_set(pwm_dev, pwm_channel, PWM_PERIOD_NS, pulsewidth * 1000/*ns*/);
        rt_pwm_enable(pwm_dev, pwm_channel);
    }
    else
    {
        LOG_E("You need to attach a servo first!");
    }
}

int Servo::read(void) // return the value as degrees
{
    return map(this->readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds(void)
{
    if(this->servo_info.Pin.isActive == true)
    {
        return this->servo_info.pulsewidth;
    }
    else
    {
        LOG_E("You need to attach a servo first!");
        return 0;
    }
}

bool Servo::attached(void)
{
    return this->servo_info.Pin.isActive;
}
