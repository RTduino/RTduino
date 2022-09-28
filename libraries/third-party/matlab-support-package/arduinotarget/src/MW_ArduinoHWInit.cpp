/* Arduino AVR Bareboard scheduler
 *
 * Copyright 2015-2019 The MathWorks, Inc.
 */

#include "Arduino.h"
#include "io_wrappers.h"
#if defined(_ROTH_MKR1000_) || defined(_ROTH_MKRWIFI1010_) || defined(_ROTH_MKRZERO_)
    #include "arduinoARM_M0plusScheduler.h"
#endif
#if defined(_ROTH_LEONARDO_)
    #include "arduinoAVRScheduler.h"
#endif
#if (defined(_ROTH_DUE_))
  #include "arduinoARMScheduler.h"
  #include "watchdog.h"
#endif

#ifdef _RTT_NUMI2C_
    #include "MW_arduinoI2C.h"
#endif

#if (defined(_RTT_SPI_SSPIN_)) || (defined(_RTT_DUE_SPI_SSPIN4_)) || (defined(_RTT_DUE_SPI_SSPIN10_)) || (defined(_RTT_DUE_SPI_SSPIN52_))
    #include "MW_SPIwriteRead.h"
#endif

#include "MW_ArduinoHWInit.h"

void MW_Arduino_Init(void)
{
    //Disable watchdog for Arduino Due. watchdog
    #if (defined(_ROTH_DUE_))
      watchdogDisable();
    #endif

    /*
     * Initialize Serial Peripheral
     */
    // Always Initialize Serial0
    Serial_begin(0, _RTT_BAUDRATE_SERIAL0_);


    #ifdef _RTT_USE_SERIAL1_
        Serial_begin(1, _RTT_BAUDRATE_SERIAL1_);
    #endif

    #ifdef _RTT_USE_SERIAL2_
        Serial_begin(2, _RTT_BAUDRATE_SERIAL2_);
    #endif

    #ifdef _RTT_USE_SERIAL3_
        Serial_begin(3, _RTT_BAUDRATE_SERIAL3_);
    #endif

    /* Overrun Detection Initialization */
    #ifdef _RTT_OVERRUN_DIGITAL_PIN_
        pinMode(_RTT_OVERRUN_DIGITAL_PIN_, OUTPUT);

        /* Init the overrun detection pin to zero or else the Pin is always in a
           high/tristate giving the user a wrong indication of overrun conditions */
        digitalWrite(_RTT_OVERRUN_DIGITAL_PIN_, LOW);
    #endif

    /*
     * Initialize Analog Reference Volatge
     * Default - Due
     * Default, Internal 1.1, Internal 2.56, External -  Mega 2560, Mega ADK
     * Deafult, Internal, External - Other Arduino targets
     */
    #if (defined(_ROTH_MEGA2560_))
        #if (defined(_RTT_ANALOG_REF_) && (_RTT_ANALOG_REF_ == 1))
             analogReference(INTERNAL1V1);
        #elif (defined(_RTT_ANALOG_REF_) && (_RTT_ANALOG_REF_ == 2))
             analogReference(INTERNAL2V56);
        #elif (defined(_RTT_ANALOG_REF_) && (_RTT_ANALOG_REF_ == 3))
             analogReference(EXTERNAL);
        #endif
    #else
        #if (defined(_RTT_ANALOG_REF_) && (_RTT_ANALOG_REF_ == 1))
            analogReference(INTERNAL);
        #elif (defined(_RTT_ANALOG_REF_) && (_RTT_ANALOG_REF_ == 2))
            analogReference(EXTERNAL);
        #endif
    #endif

    /*
     * Initialize SPI bus
     */
        //_RTT_SPI_SSPIN_ definition is applicable for all boards excluding Due
        #if defined(_RTT_SPI_SSPIN_)
            MW_SPIwriteReadAVRSetup();
        #endif

        //for Due
        #if defined(_RTT_DUE_SPI_SSPIN4_)
            MW_SPIwriteReadDueSetup(4);
        #endif

        #if defined(_RTT_DUE_SPI_SSPIN10_)
            MW_SPIwriteReadDueSetup(10);
        #endif

        #if defined(_RTT_DUE_SPI_SSPIN52_)
            MW_SPIwriteReadDueSetup(52);
        #endif

}
/* Function: MW_Arduino_Terminate ================================================
 * For Arduino MKR/Leonardo and its variants, the Virtual COM port is handled
 * by the controller. In case the code running on the target exits main,
 * the COM port cannot be accessed until a hard reset is performed.
 * To over come this issue, a while loop is added to make sure that
 * upon getting a stop command from external mode, the code running on
 * the target stops but the code will not exit the main.
 * This will ensure that the COM port is accessible even after the
 * external mode has been stopped. */
void MW_Arduino_Terminate(void){
    #if defined(_ROTH_DUE_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRWIFI1010_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_LEONARDO_)
        disable_rt_OneStep();
        while(1){};     /* To continue virtual COM port functionality */
    #endif
}
