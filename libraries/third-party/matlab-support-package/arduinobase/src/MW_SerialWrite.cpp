#include <inttypes.h>
#include "Arduino.h"
#include "MW_SerialWrite.h"
#include "io_wrappers.h"


extern "C" {
    void __cxa_pure_virtual(void);
}

// One-time initialization


extern "C" void MW_Serial_write(const uint8_T port, void *dataInput, size_t s,const uint8_T dataSizeInBytes, \
                                const uint8_T sendModeEnum, const uint8_T dataType, const uint8_T sendFormatEnum, \
                                const uint8_T floatprecision, char* label)
{
    int dataSize=dataSizeInBytes;
    unsigned int counter = s;
    int base;

#if !(defined(_ROTH_DUE_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_))
    if (dataSize==8){
        dataSize=4;
    }
#endif

    if (sendFormatEnum == 0) { //DEC
        base=DEC;
    }
    else if(sendFormatEnum == 1) { //HEX
        base=HEX;
    }
    else if(sendFormatEnum == 2) { //OCT
        base=OCT;
    }
    else { //BIN
        base=BIN;
    }

    switch(port) {
        case 0:
            if (sendModeEnum == 0)
            {
                Serial.write((uint8_t *)dataInput,(s*dataSize));
            }
            else
            {
                Serial.print(label);
                if(dataType == 0)
                {
                    uint8_t * ptr = (uint8_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial.println(*(ptr++),base);
                        }

                    }
                } //uint8
                else if(dataType == 1)
                {
                    uint16_t * ptr = (uint16_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial.println(*(ptr++),base);
                        }
                    }
                } //uint16
                else if(dataType == 2)
                {
                    uint32_t * ptr = (uint32_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial.println(*(ptr++),base);
                        }
                    }
                } //uint32
                else if(dataType == 3)
                {
                    int8_t * ptr = (int8_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial.println(*(ptr++),base);
                        }
                    }
                } //int8
                else if(dataType == 4)
                {
                    int16_t * ptr = (int16_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial.println(*(ptr++),base);
                        }
                    }
                } //int16
                else if(dataType == 5)
                {
                    int32_t * ptr = (int32_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial.println(*(ptr++),base);
                        }
                    }
                } //int32
                else if(dataType == 6)
                {
                    double * ptr = (double *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                          Serial.print(*(ptr++),floatprecision); // for double and single, DE is the only supported mode
                        }
                        else
                        {
                           Serial.println(*(ptr++),floatprecision);   // for double and single, DE is the only supported mode
                        }
                    }
                } //double, single
            } //print and println
            break;
#if (defined(_ROTH_LEONARDO_) || defined(_ROTH_MEGA2560_) || defined(_ROTH_DUE_) || defined(_ROTH_MEGAADK_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_))
        case 1:
            if (sendModeEnum == 0)
            {
                Serial1.write((uint8_t *)dataInput,(s*dataSize));
            }
            else
            {
                Serial1.print(label);
                if(dataType == 0)
                {
                    uint8_t * ptr = (uint8_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial1.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial1.println(*(ptr++),base);
                        }

                    }
                } //uint8
                else if(dataType == 1)
                {
                    uint16_t * ptr = (uint16_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial1.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial1.println(*(ptr++),base);
                        }
                    }
                } //uint16
                else if(dataType == 2)
                {
                    uint32_t * ptr = (uint32_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial1.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial1.println(*(ptr++),base);
                        }
                    }
                } //uint32
                else if(dataType == 3)
                {
                    int8_t * ptr = (int8_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial1.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial1.println(*(ptr++),base);
                        }
                    }
                } //int8
                else if(dataType == 4)
                {
                    int16_t * ptr = (int16_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial1.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial1.println(*(ptr++),base);
                        }
                    }
                } //int16
                else if(dataType == 5)
                {
                    int32_t * ptr = (int32_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial1.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial1.println(*(ptr++),base);
                        }
                    }
                } //int32
                else if(dataType == 6)
                {
                    double * ptr = (double *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            if (sendFormatEnum == 0) { //DEC
                                Serial1.print(*(ptr++),floatprecision);
                            }
                            else { //HEX, OCT, BIN
                                Serial1.print((long)*(ptr++),base);
                            }
                        }
                        else
                        {
                            if (sendFormatEnum == 0) { //DEC
                                Serial1.println(*(ptr++),floatprecision);
                            }
                            else { //HEX, OCT, BIN
                                Serial1.println((long)*(ptr++),base);
                            }
                        }
                    }
                } //double, single
            } //print and println
            break;
#endif
#if (defined(_ROTH_MEGA2560_) || defined(_ROTH_DUE_) || defined(_ROTH_MEGAADK_))
        case 2:
            if (sendModeEnum == 0)
            {
                Serial2.write((uint8_t *)dataInput,(s*dataSize));
            }
            else
            {
                Serial2.print(label);
                if(dataType == 0)
                {
                    uint8_t * ptr = (uint8_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial2.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial2.println(*(ptr++),base);
                        }

                    }
                } //uint8
                else if(dataType == 1)
                {
                    uint16_t * ptr = (uint16_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial2.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial2.println(*(ptr++),base);
                        }
                    }
                } //uint16
                else if(dataType == 2)
                {
                    uint32_t * ptr = (uint32_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial2.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial2.println(*(ptr++),base);
                        }
                    }
                } //uint32
                else if(dataType == 3)
                {
                    int8_t * ptr = (int8_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial2.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial2.println(*(ptr++),base);
                        }
                    }
                } //int8
                else if(dataType == 4)
                {
                    int16_t * ptr = (int16_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial2.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial2.println(*(ptr++),base);
                        }
                    }
                } //int16
                else if(dataType == 5)
                {
                    int32_t * ptr = (int32_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial2.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial2.println(*(ptr++),base);
                        }
                    }
                } //int32
                else if(dataType == 6)
                {
                    double * ptr = (double *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            if (sendFormatEnum == 0) { //DEC
                                Serial2.print(*(ptr++),floatprecision);
                            }
                            else { //HEX, OCT, BIN
                                Serial2.print((long)*(ptr++),base);
                            }
                        }
                        else
                        {
                            if (sendFormatEnum == 0) { //DEC
                                Serial2.println(*(ptr++),floatprecision);
                            }
                            else { //HEX, OCT, BIN
                                Serial2.println((long)*(ptr++),base);
                            }
                        }
                    }
                } //double, single
            } //print and println
            break;
        case 3:
            if (sendModeEnum == 0)
            {
                Serial3.write((uint8_t *)dataInput,(s*dataSize));
            }
            else
            {
                Serial3.print(label);
                if(dataType == 0)
                {
                    uint8_t * ptr = (uint8_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial3.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial3.println(*(ptr++),base);
                        }

                    }
                } //uint8
                else if(dataType == 1)
                {
                    uint16_t * ptr = (uint16_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial3.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial3.println(*(ptr++),base);
                        }
                    }
                } //uint16
                else if(dataType == 2)
                {
                    uint32_t * ptr = (uint32_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial3.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial3.println(*(ptr++),base);
                        }
                    }
                } //uint32
                else if(dataType == 3)
                {
                    int8_t * ptr = (int8_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial3.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial3.println(*(ptr++),base);
                        }
                    }
                } //int8
                else if(dataType == 4)
                {
                    int16_t * ptr = (int16_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial3.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial3.println(*(ptr++),base);
                        }
                    }
                } //int16
                else if(dataType == 5)
                {
                    int32_t * ptr = (int32_t *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            Serial3.print(*(ptr++),base);
                        }
                        else
                        {
                            Serial3.println(*(ptr++),base);
                        }
                    }
                } //int32
                else if(dataType == 6)
                {
                    double * ptr = (double *) dataInput;
                    while(counter--)
                    {
                        if (sendModeEnum == 1)
                        {
                            if (sendFormatEnum == 0) { //DEC
                                Serial3.print(*(ptr++),floatprecision);
                            }
                            else { //HEX, OCT, BIN
                                Serial3.print((long)*(ptr++),base);
                            }
                        }
                        else
                        {
                            if (sendFormatEnum == 0) { //DEC
                                Serial3.println(*(ptr++),floatprecision);
                            }
                            else { //HEX, OCT, BIN
                                Serial3.println((long)*(ptr++),base);
                            }
                        }
                    }
                } //double, single
            } //print and println
            break;
#endif
    } //Port
}
