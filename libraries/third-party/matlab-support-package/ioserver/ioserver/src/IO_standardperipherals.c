/**
 * @file IO_standardperipherals.c
 *
 * Contains definition of standard peripheral functions
 *
 * @Copyright 2017-2019 The MathWorks, Inc.
 *
 */

#include "IO_standardperipherals.h"
#include "IO_requestID.h"
#include "IO_wrapperDigitalIO.h"
#include "IO_wrapperAnalogInput.h"
#include "IO_wrapperPWM.h"
#if ADD_ON
#include "IO_addOn.h"
#endif
#ifndef ESB_BETA2_IOSERVER
#include "IO_wrapperI2C.h"
#include "IO_wrapperSPI.h"
#include "IO_wrapperSCI.h"
#endif
/***************************************************************************************************************************
 *              Hook for initialization of standard peripherals.
 ****************************************************************************************************************************/
void standardPeripheralsInit()
{
}

/***************************************************************************************************************************
 *              Standard peripherals supported by IO server.
 ****************************************************************************************************************************/
uint8_T standardPeripherals(uint16_T cmdID,uint8_T* payloadBufferRx, uint8_T* payloadBufferTx,uint16_T* peripheralDataSizeResponse, uint8_T* status)
{

    switch (cmdID) {
#if IO_STANDARD_DIGITALIO
        case (CONFIGUREDIGITALPIN):
            /* Configures a digital pin as input or output */
            configureDigitalPin(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case (WRITEDIGITALPIN):
            /* Sets or clears a digital pin */
            writeDigitalPin(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case (READDIGITALPIN):
            /* Reads the status of a digital pin */
            readDigitalPin(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case (UNCONFIGUREDIGITALPIN):
            /* Unconfigures a digital pin */
            unconfigureDigitalPin(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case (RESETALLDIGITALPINS):
            /* Reset all digital pins */
            resetDigitalPins(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;
#endif

#if IO_STANDARD_I2C
        case SCANI2CBUS:
            /* Scans I2C bus and returns all 7-bit addressable slaves */
            scanI2CBus(peripheralDataSizeResponse, payloadBufferRx, payloadBufferTx, status);
            break;

        case OPENI2CBUS:
            /* Opens the I2C bus and sets the frequency */
            openI2CBus(peripheralDataSizeResponse,payloadBufferRx,payloadBufferTx, status);
            break;

        case CLOSEI2CBUS:
            /* Closes the I2C bus */
            closeI2CBus(peripheralDataSizeResponse, payloadBufferRx, payloadBufferTx, status);
            break;

#ifndef LTC_BAREMETAL_HARDWARE
        case ENABLEI2C:
            /* Required by Rasperry pi to mount the I2C driver module */
            enableI2C(peripheralDataSizeResponse,payloadBufferRx,payloadBufferTx, status);
            break;

        case DISABLEI2C:
            /* Required by Rasperry pi to unmount the I2C driver module */
            disableI2C(peripheralDataSizeResponse,payloadBufferRx,payloadBufferTx, status);
            break;

        case SETI2CBUSSPEED:
            /* Configures I2C bus frequency */
            setI2CFrequency(peripheralDataSizeResponse,payloadBufferRx,payloadBufferTx, status);
            break;
#endif

        case RAWI2CWRITE:
            /* Takes vector of data and writes to the I2C bus */
            rawI2CWrite(peripheralDataSizeResponse,payloadBufferRx,payloadBufferTx, status);
            break;

        case RAWI2CREAD:
            /* Reads data from the I2C bus */
            rawI2CRead(peripheralDataSizeResponse,payloadBufferRx,payloadBufferTx, status);
            break;

        case REGISTERI2CWRITE:
            /* Writes data to a slave's I2C configuration register */
            registerI2CWrite(peripheralDataSizeResponse,payloadBufferRx,payloadBufferTx, status);
            break;

        case REGISTERI2CREAD:
            /* Reads data from a slave's I2C configuration register */
            registerI2CRead(peripheralDataSizeResponse,payloadBufferRx,payloadBufferTx, status);
            break;
#endif

#if IO_STANDARD_ANALOGINPUT

        case CONFIGUREANALOGINSINGLE:
            /* Configures a analog input pin */
            configureAnalogInSingle(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

#ifndef LTC_BAREMETAL_HARDWARE
        case CONFIGUREANALOGINGROUP:
            /* Configures a group of analog input pin with conversion time*/
            configureAnalogInGroup(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

        case SETTRIGGERSOURCEANALOGIN:
            /* Select trigger source for analog input pin conversion */
            setTriggerSourceAnalogIn(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

        case ENABLENOTIFICATIONANALOGIN:
            /* Enable conversion complete notification for analog input pin */
            enableNotificationAnalogIn(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

        case DISABLENOTIFICATIONANALOGIN:
            /* Disable conversion complete notifications for analog input pin*/
            disableNotificationAnalogIn(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

        case ENABLECONTCONVERSIONANALOGIN:
            /* Enable continuous conversion for analog input pin*/
            enableContConversionAnalogIn(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

        case SETCHANNELCONVRANKANALOGIN:
            /* Set channel conversion priority for analog input pin*/
            setChannelConvRankAnalogIn(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

        case GETSTATUSANALOGIN:
            /* Get status of analog input conversion */
            getStatusAnalogIn(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;
#endif

        case READRESULTANALOGINSINGLE:
            /* Read single channel conversion result for analog input pin*/
            readResultAnalogInSingle(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;
#ifndef LTC_BAREMETAL_HARDWARE
        case READRESULTANALOGINGROUP:
            /* Read group conversion result for analog input pins */
            readResultAnalogInGroup(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

        case STARTANALOGINCONVERSION:
            /* Start analog input pin conversion */
            startAnalogInConversion(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

        case STOPANALOGINCONVERSION :
            /* Stop analog input pin conversion */
            stopAnalogInConversion(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

        case UNCONFIGUREANALOGINSINGLE:
            /* Unconfigures a analog input pin */
            unconfigureAnalogInSingle(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse, status);
            break;

        case (RESETALLANALOGPINS):
            /* Reset all Analog pins */
            resetAnalogPins(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;
#endif

#endif

#if IO_STANDARD_PWM
        case CONFIGUREPWMPIN:
            /* Configures a PWM pin */
            configurePWMPin(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

#ifndef LTC_BAREMETAL_HARDWARE
        case STARTPWM:
            /* Start PWM */
            startPWM(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;
#endif

        case SETDUTYCYCLE:
            /* Sets duty cycle on a PWM pin */
            setDutyCycle(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

#ifndef LTC_BAREMETAL_HARDWARE
        case SETFREQUENCY:
            /* Sets frequency on a PWM pin */
            setFrequency(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case DISABLEPWMNOTIFICATION:
            /* Disable notifications on the channel */
            disablePWMNotification(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case ENABLEPWMNOTIFICATION:
            /* Enable notifications on the channel */
            enablePWMNotification(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case SETPWMOUTPUTTOIDLE:
            /* Set PWM output to idle state */
            setPWMOutputToIdle(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case GETPWMOUTPUTSTATUS:
            /* Get the PWM output status */
            getPWMOutputState(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case STOPPWM:
            /* Stop PWM */
            stopPWM(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case UNCONFIGUREPWMPIN:
            /* Unconfigures a PWM pin */
            unconfigurePWMPin(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;

        case (RESETALLPWMPINS):
            /* Reset all PWM pins */
            resetPWMPins(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;
#endif

#endif

#if IO_STANDARD_SPI
#ifndef LTC_BAREMETAL_HARDWARE
        case ENABLESPI:
            /* Enable the SPI device peripheral */
            enableSPI(peripheralDataSizeResponse, payloadBufferRx, payloadBufferTx, status);
            break;

        case DISABLESPI:
            /* Disable the SPI device peripheral */
            disableSPI(peripheralDataSizeResponse, payloadBufferRx, payloadBufferTx, status);
            break;
#endif

        case OPENSPI:
            /* Setup the SPI device properties and to open the SPI bus */
            openSPI(peripheralDataSizeResponse, payloadBufferRx, payloadBufferTx, status);
            break;

        case SETFORMATSPI:
            /* Set the SPI Mode and SPI bit order*/
            setFormatSPI(peripheralDataSizeResponse, payloadBufferRx, payloadBufferTx, status);
            break;

        case SETBUSSPEEDSPI:
            /* Set the SPI bus frequency*/
            setBusSpeedSPI(peripheralDataSizeResponse, payloadBufferRx, payloadBufferTx, status);
            break;

        case WRITEREADSPI:
            /* Write and read data to or from an SPI device */
            writeReadSPI(peripheralDataSizeResponse, payloadBufferRx, payloadBufferTx, status);
            break;

        case CLOSESPI:
            /* Close the SPI bus */
            closeSPI(peripheralDataSizeResponse, payloadBufferRx, payloadBufferTx, status);
            break;
#endif

#if IO_STANDARD_SCI

        case (OPENSCIBUS):
            /* Initialize a SCI */
            openSCIBus(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse,status);
            break;

        case (CONFIGURESCIHARDWAREFLOWCONTROL):
            /* Set SCI frame format */
            configureSCIHardwareFlowControl(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse,status);
            break;

        case (SETSCIBAUDRATE):
            /* Set the SCI bus speed */
            setSCIBaudrate(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse,status);
            break;

#ifdef LTC_BAREMETAL_HARDWARE
        case (SETSCITIMEOUT):
            /* Set the SCI bus speed */
            setSCITimeOut(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse,status);
            break;
#endif
        case (SETSCIFRAMEFORMAT):
            /* Set SCI frame format */
            setSCIFrameFormat(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse,status);
            break;

        case (SCIRECEIVE):
            /* Receive the data over SCI */
            sciReceive(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse,status);
            break;

        case (SCITRASMIT):
            /* Trasmit the data over SCI */
            sciTrasmit(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse,status);
            break;

        case (GETSCISTATUS):
            /* Get the status of SCI device */
            getSCIStatus(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse,status);
            break;

        case (SCISENDBREAK):
            /* Send break command */
            sciSendBreak(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse,status);
            break;

        case (SCICLOSE):
            /* Release SCI module */
            sciClose(payloadBufferRx,payloadBufferTx,peripheralDataSizeResponse,status);
            break;
#endif

#if ADD_ON
        case ADDON:
            addon(payloadBufferRx, payloadBufferTx, peripheralDataSizeResponse, status);
            break;
#endif
        default:
            return 1;
    }
    return 0;
}
