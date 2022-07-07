/*
 * Wrappers to make SPI functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2015 The MathWorks, Inc. */

#if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
/* This will be run in Rapid Accelerator Mode */
/*do nothing */

#else

#include "Arduino.h"
#include <SPI.h>
#include "io_wrappers.h"
#include "MW_SPIwriteRead.h"

extern "C" {
    void __cxa_pure_virtual(void);
}

//Initialize Slave Select pin as output and pull it high
extern "C" void MW_SSpinSetup(uint8_T slaveSelectPin) {
    pinMode(slaveSelectPin, OUTPUT); // set the Slave Select Pin as an output
    digitalWrite(slaveSelectPin, HIGH);
}


//Initiate SPI library for Due and join SPI bus as master
#ifdef _ROTH_DUE_
extern "C" void MW_SPIwriteReadDueSetup(uint8_T slaveSelectPin) {
    SPI.begin(slaveSelectPin);
}
#else
//Initiate SPI library for AVR boards and join SPI bus as master
extern "C" void MW_SPIwriteReadAVRSetup(void) {
    SPI.begin();
}
#endif

extern "C" void MW_SPIwriteReadLoop(uint8_T slaveSelectPin, void *input, uint8_T datalength, uint8_T dataType,void *output) {
  SPISettings settingsA(_RTT_SPI_CLOCK_, _RTT_SPI_BITORDER_, _RTT_SPI_MODE_);
  // Apply the settings with beginTransaction. beginTransaction disables all the SPI interrupts and ensures a sole hold over the SPI bus
  #ifdef _ROTH_DUE_
    SPI.beginTransaction(slaveSelectPin, settingsA);
  #else
    SPI.beginTransaction(settingsA);
  #endif

  // pull the Slave Select pin low to select the slave
  digitalWrite(slaveSelectPin, LOW);

      // exchange data via SPI
        switch (dataType){
            case 1:
            {
                uint8_T *datainPtr = (uint8_T *) input;
                uint8_T *dataoutPtr = (uint8_T *) output;

    #ifdef _ROTH_DUE_
                //keep the SS pin selected till last but one byte
                for(int i = 0; i < (datalength - 1); i++){
                    dataoutPtr[i] = SPI.transfer(slaveSelectPin, *datainPtr++, SPI_CONTINUE);

                }
                //deselect SS pin after the last byte

                dataoutPtr[datalength-1] = SPI.transfer(slaveSelectPin, *datainPtr);


    #else
                    for(int i = 0; i < datalength; i++){
                        dataoutPtr[i] = SPI.transfer(*datainPtr++);
                    }

    #endif

            }
            break;
            case 2:
            {
                uint16_T *datainPtr = (uint16_T *)input;
                uint16_T *dataoutPtr = (uint16_T *) output;
                union { uint16_T val; struct { uint8_T lsb; uint8_T msb; }; } in, out;

    #ifdef _ROTH_DUE_
                    //transfer till last but one byte
                    for(int i = 0; i < (datalength - 1); i++){
                        in.val = *datainPtr++;

                        if (_RTT_SPI_BITORDER_){
                            //true for LSB first case
                            out.lsb = SPI.transfer(slaveSelectPin, in.lsb, SPI_CONTINUE);
                            out.msb = SPI.transfer(slaveSelectPin, in.msb, SPI_CONTINUE);
                        }
                        else {
                            //MSB first case
                            out.msb = SPI.transfer(slaveSelectPin, in.msb, SPI_CONTINUE);
                            out.lsb = SPI.transfer(slaveSelectPin, in.lsb, SPI_CONTINUE);
                        }
                        dataoutPtr[i] = out.val;
                    }

                    //transfer last byte
                    in.val = *datainPtr;

                    if (_RTT_SPI_BITORDER_){
                        //true for LSB first case
                        out.lsb = SPI.transfer(slaveSelectPin, in.lsb);
                        out.msb = SPI.transfer(slaveSelectPin, in.msb);
                    }
                    else {
                        //MSB first case
                        out.msb = SPI.transfer(slaveSelectPin, in.msb);
                        out.lsb = SPI.transfer(slaveSelectPin, in.lsb);
                    }
                    dataoutPtr[datalength-1] = out.val;

    #else

                    for(int i = 0; i < datalength; i++){
                        in.val = *datainPtr++;

                        if (_RTT_SPI_BITORDER_){
                            //true for LSB first case
                            out.lsb = SPI.transfer(in.lsb);
                            out.msb = SPI.transfer(in.msb);
                        }
                        else {
                            //MSB first case
                            out.msb = SPI.transfer(in.msb);
                            out.lsb = SPI.transfer(in.lsb);
                        }
                        dataoutPtr[i] = out.val;
                    }

    #endif
            }
            break;
            case 3:
            {
                uint32_T *datainPtr = (uint32_T *)input;
                uint32_T *dataoutPtr = (uint32_T *) output;
                union { uint32_T val; struct { uint8_T byte0; uint8_T byte1; uint8_T byte2; uint8_T byte3;}; } in, out;

    #ifdef _ROTH_DUE_
                    //transfer till last but one byte
                    for(int i = 0; i < (datalength - 1); i++){
                        in.val = *datainPtr++;

                        if (_RTT_SPI_BITORDER_){
                            //true for LSB first case
                            out.byte0 = SPI.transfer(slaveSelectPin, in.byte0, SPI_CONTINUE);
                            out.byte1 = SPI.transfer(slaveSelectPin, in.byte1, SPI_CONTINUE);
                            out.byte2 = SPI.transfer(slaveSelectPin, in.byte2, SPI_CONTINUE);
                            out.byte3 = SPI.transfer(slaveSelectPin, in.byte3, SPI_CONTINUE);
                        }
                        else {
                            //MSB first case
                            out.byte3 = SPI.transfer(slaveSelectPin, in.byte3, SPI_CONTINUE);
                            out.byte2 = SPI.transfer(slaveSelectPin, in.byte2, SPI_CONTINUE);
                            out.byte1 = SPI.transfer(slaveSelectPin, in.byte1, SPI_CONTINUE);
                            out.byte0 = SPI.transfer(slaveSelectPin, in.byte0, SPI_CONTINUE);
                        }
                        dataoutPtr[i] = out.val;
                    }

                    // transfer last byte
                    in.val = *datainPtr;

                    if (_RTT_SPI_BITORDER_){
                        //true for LSB first case
                        out.byte0 = SPI.transfer(slaveSelectPin, in.byte0);
                        out.byte1 = SPI.transfer(slaveSelectPin, in.byte1);
                        out.byte2 = SPI.transfer(slaveSelectPin, in.byte2);
                        out.byte3 = SPI.transfer(slaveSelectPin, in.byte3);
                    }
                    else {
                        //MSB first case
                        out.byte3 = SPI.transfer(slaveSelectPin, in.byte3);
                        out.byte2 = SPI.transfer(slaveSelectPin, in.byte2);
                        out.byte1 = SPI.transfer(slaveSelectPin, in.byte1);
                        out.byte0 = SPI.transfer(slaveSelectPin, in.byte0);
                    }
                    dataoutPtr[datalength-1] = out.val;

    #else
                    for(int i = 0; i < datalength; i++){
                        in.val = *datainPtr++;

                        if (_RTT_SPI_BITORDER_){
                            //true for LSB first case
                            out.byte0 = SPI.transfer(in.byte0);
                            out.byte1 = SPI.transfer(in.byte1);
                            out.byte2 = SPI.transfer(in.byte2);
                            out.byte3 = SPI.transfer(in.byte3);
                        }
                        else {
                            //MSB first case
                            out.byte3 = SPI.transfer(in.byte3);
                            out.byte2 = SPI.transfer(in.byte2);
                            out.byte1 = SPI.transfer(in.byte1);
                            out.byte0 = SPI.transfer(in.byte0);
                        }
                        dataoutPtr[i] = out.val;
                    }

    #endif
            }
            break;
            default:
            {
                uint8_T *datainPtr = (uint8_T *) input;
                uint8_T *dataoutPtr = (uint8_T *) output;
    #ifdef _ROTH_DUE_
                    //keep the SS pin selected till last but one byte
                    for(int i = 0; i < (datalength - 1); i++){
                        dataoutPtr[i] = SPI.transfer(slaveSelectPin, *datainPtr++, SPI_CONTINUE);
                    }
                    //deselect SS pin after the last byte
                    dataoutPtr[datalength-1] = SPI.transfer(slaveSelectPin, *datainPtr);

    #else
                    for(int i = 0; i < datalength; i++){
                        dataoutPtr[i] = SPI.transfer(*datainPtr++);
                    }

    #endif
            }
            break;
        }

        // pull the Slave Select pin high to deselect the slave
        digitalWrite(slaveSelectPin, HIGH); // may be repititive for Due
        SPI.endTransaction(); // release the SPI bus, endTransaction also re-enables the SPI interrupts
    }
#endif
