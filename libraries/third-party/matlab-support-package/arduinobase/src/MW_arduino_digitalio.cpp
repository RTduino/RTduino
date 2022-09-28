#ifndef MATLAB_MEX_FILE
#include <Arduino.h>
#include "MW_arduino_digitalio.h"
#endif
#include "rtwtypes.h"

// Digital I/O initialization
extern "C" void digitalIOSetup(uint8_T pin, uint8_T mode)
{
    #ifndef MATLAB_MEX_FILE
    // mode = 0: Input
    // mode = 1: Output
    // mode = 2: Input pullup
    if (mode==0) {
        pinMode(pin, INPUT);
    }
    else if (mode==1) {
        pinMode(pin, OUTPUT);
    }
    else {
        // mode = 2 is for input pullup
        pinMode(pin, INPUT_PULLUP);
    }
    #endif
}

// Write a logic value to pin
extern "C" void writeDigitalPin(uint8_T pin, boolean_T val)
{
    #ifndef MATLAB_MEX_FILE
    if (val) {
        digitalWrite(pin, HIGH);
    }
    else {
        digitalWrite(pin, LOW);
    }
    #endif
}

// Read logical state of a digital pin
extern "C" boolean_T readDigitalPin(uint8_T pin)
{
    #ifndef MATLAB_MEX_FILE
    boolean_T ret;

    ret = (digitalRead(pin) == HIGH) ? 1:0;
    return ret;
    #else
    return (boolean_T)0;
    #endif
}
// [EOF]
