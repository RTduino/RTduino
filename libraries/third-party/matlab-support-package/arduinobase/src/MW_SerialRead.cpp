#include <inttypes.h>
#include "Arduino.h"
#include "MW_SerialRead.h"
#include "io_wrappers.h"


extern "C" {
    void __cxa_pure_virtual(void);
}

// Serial Read functions
extern "C" void MW_Serial0_read(char *bufferData,const uint16_t DataSizeInBytes)
{
    Serial.readBytes(bufferData,DataSizeInBytes);
}

extern "C" int MW_Serial0_available()
{
    int BufferSize;
    BufferSize = Serial.available();
    return(BufferSize);
}

#if (defined(_ROTH_LEONARDO_) || defined(_ROTH_MEGA2560_) || defined(_ROTH_DUE_) || defined(_ROTH_MEGAADK_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_))
    extern "C" void MW_Serial1_read(char *bufferData,const uint16_t DataSizeInBytes)
    {
        Serial1.readBytes(bufferData,DataSizeInBytes);
    }

    extern "C" int MW_Serial1_available()
    {
        int BufferSize;
        BufferSize = Serial1.available();
        return(BufferSize);
    }
#endif

#if (defined(_ROTH_MEGA2560_) || defined(_ROTH_DUE_) || defined(_ROTH_MEGAADK_))
    extern "C" void MW_Serial2_read(char *bufferData,const uint16_t DataSizeInBytes)
    {
        Serial2.readBytes(bufferData,DataSizeInBytes);
    }

    extern "C" void MW_Serial3_read(char *bufferData,const uint16_t DataSizeInBytes)
    {
        Serial3.readBytes(bufferData,DataSizeInBytes);
    }

    extern "C" int MW_Serial2_available()
    {
        int BufferSize;
        BufferSize = Serial2.available();
        return(BufferSize);
    }

    extern "C" int MW_Serial3_available()
    {
        int BufferSize;
        BufferSize = Serial3.available();
        return(BufferSize);
    }
#endif

#if (defined(_ROTH_MEGA2560_) || defined(_ROTH_DUE_) || defined(_ROTH_MEGAADK_))
    void (*read_func_ptr[])(char *, uint16_t) = {MW_Serial0_read, MW_Serial1_read, MW_Serial2_read, MW_Serial3_read};
    int (*available_func_ptr[])() = {MW_Serial0_available,MW_Serial1_available,MW_Serial2_available,MW_Serial3_available};
#else
    #if (defined(_ROTH_LEONARDO_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_))
        void (*read_func_ptr[])(char *, uint16_t) = {MW_Serial0_read, MW_Serial1_read};
        int (*available_func_ptr[])() = {MW_Serial0_available,MW_Serial1_available};
    #else
        void (*read_func_ptr[])(char *, uint16_t) = {MW_Serial0_read};
        int (*available_func_ptr[])() = {MW_Serial0_available};
    #endif
#endif


extern "C" void fillDataBuffer(const uint8_T port,const uint16_t DataSizeInBytes,char *data,uint8_T *status)
{
    char databuffer[2*DataSizeInBytes];
    (*read_func_ptr[port])(databuffer,DataSizeInBytes);
    for(int ii=0;ii< DataSizeInBytes;ii++)
    {
        *data++ = databuffer[ii];
    }
    *status = 1;
}



extern "C" void MW_Serial_read(const uint8_T port,const uint16_t DataSizeInBytes, char *data,uint8_T *status)
{

    int dataSizeinBuffer;
    char tempdatabuffer[64];


    /* Read the size of data available in the Arduino Serial buffer */
    dataSizeinBuffer = (*available_func_ptr[port])();
    if (dataSizeinBuffer == DataSizeInBytes)
    {
        /* If the size of data in buffer == required data size, data=received data and status =1 */
        fillDataBuffer(port,DataSizeInBytes,data,status);
    }
    else if ( dataSizeinBuffer >= DataSizeInBytes)
    {
        if (dataSizeinBuffer <= 2*DataSizeInBytes)
        {
            /* If the size of data in buffer > required data size and < 2*required data size, data=received data(of the rqquired size) and status =1 */
            fillDataBuffer(port,DataSizeInBytes,data,status);
        }
        else
        {
            /* If the size of data in buffer >  2*required data size, trash data till size of data in buffer < 2*required data size.data=received data and status =1 */
            (*read_func_ptr[port])(tempdatabuffer,(((dataSizeinBuffer/DataSizeInBytes)-1)*DataSizeInBytes));
            fillDataBuffer(port,DataSizeInBytes,data,status);
        }
    }
    else
    {
        /* If the size of data in buffer < required data size, data=0's  and status =0 */
        for(int ii=0;ii< DataSizeInBytes;ii++)
        {
            *data++ = 0;
        }
        *status = 0;
    }
}

