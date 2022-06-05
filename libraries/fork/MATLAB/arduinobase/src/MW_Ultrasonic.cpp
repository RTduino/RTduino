/*
 * Wrappers to make Ultrasonic sensor functions available with C linkage.
 * This allows C++ methods to be called from C code.
 *
/* Copyright 2018 The MathWorks, Inc. */
#include "MW_Ultrasonic.h"
#if !( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )

    #include <Arduino.h>
    #define PULSE_LOW_TIME 5

    extern "C" {
            void __cxa_pure_virtual(void);
    }

    /* Ultrasonic sensor initialization */
    /* Called only for sensors having both Trigger and Echo pins*/
    extern "C" void MW_UltrasonicSetup(uint8_T TrigPin, uint8_T EchoPin)
    {
        /*Set mode for Trigger and Echo Pins*/
        pinMode(TrigPin, OUTPUT);
        pinMode(EchoPin, INPUT);
    }

    /* Get the duration from Ultrasonic sensor */
    extern "C" void MW_UltrasonicRead(uint32_T *duration, uint8_T hasTrig, uint8_T TrigPin, uint8_T EchoPin, uint8_T Trigger_Pulse_Duration)
    {
        unsigned long startTime;
        /* Both TrigPin and EchoPin are same as SignalPin for sensors
         * having 1 signal pin*/
        if (!hasTrig) {
           /*Set Signal Pin to Output for sensors having 1 signal pin
            * before sending Trigger*/
            pinMode(TrigPin, OUTPUT);
        }
        /*Send Trigger*/
        digitalWrite(TrigPin, LOW);
        startTime = micros();
        while(micros() - startTime <= PULSE_LOW_TIME){
        }

        digitalWrite(TrigPin, HIGH);
        startTime = micros();
        while(micros() - startTime <= Trigger_Pulse_Duration){
        }
        digitalWrite(TrigPin, LOW);

        if (!hasTrig) {
            /*Set Signal Pin to Input for sensors having 1 signal pin
             * to read incoming echo*/
            pinMode(EchoPin, INPUT);
        }

        /* Read Echo duration*/
        *duration = pulseIn(EchoPin, HIGH);
    }

#endif /*Rapid Accel condition check*/
/* [EOF] */
