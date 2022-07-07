/*
 * Wrappers to make Tachometer functions available with C linkage.
 * This allows C++ methods to be called from C code.
 *
/* Copyright 2018 The MathWorks, Inc. */

#include "MW_Tachometer.h"

#if !( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )

    #include <Arduino.h>
    volatile uint32_T counter;
    unsigned long passedtime;

    /*
     * Interrupt Routine
     */
    void countISR(void)

    {
       //Update count
        counter++;
    }


    /*
     * Tachometer initialization
     */
    extern "C" void MW_Tachometer_Init(uint8_T pin, uint8_T mode)
    {
        counter = 0;
        passedtime = 0;

        //Set the pin to INPUT mode
        pinMode(pin, INPUT);

        switch(mode){
            case 0:
                attachInterrupt(digitalPinToInterrupt(pin), countISR, RISING);
                break;
            case 1:
                attachInterrupt(digitalPinToInterrupt(pin), countISR, FALLING);
                break;
            case 2:
                attachInterrupt(digitalPinToInterrupt(pin), countISR, CHANGE);
                break;
            case 3:
                attachInterrupt(digitalPinToInterrupt(pin), countISR, LOW);
                break;
            case 4:
                attachInterrupt(digitalPinToInterrupt(pin), countISR, HIGH);
                break;
            default:
                //Do Nothing
                break;
        }


    }

    /*
     * Calculate RPM
     */
    extern "C" uint32_T MW_Tachometer_GetSpeed(void)
    {
        uint32_T rpm;
        /* Using micros() to calculate time elapsed instead of millis()
        to avoid divide by zero possibility
        */
        rpm = (counter * (1000000/(micros() - passedtime)) * 60);
        passedtime = micros();

        counter = 0;
        return rpm;
    }
#endif /*Rapid Accel condition check*/
/* [EOF] */
