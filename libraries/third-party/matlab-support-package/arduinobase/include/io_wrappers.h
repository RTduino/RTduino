/*
 * Wrappers to make I/O functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2011-2019 The MathWorks, Inc. */
#ifndef _IO_WRAPPERS_H_
#define _IO_WRAPPERS_H_
#if !(( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) ))
    #include <inttypes.h>
    #include "rtwtypes.h"
    #include <stdio.h> /* for size_t */
#endif
#ifdef __cplusplus
extern "C" {
#endif
    #if !(( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) ))
        void Serial_begin(int port, long r);
        void Serial_read(int port, int showOutStatus, uint8_t *outData, int16_t *outStatus);
        void Serial_write(int port, uint8_t * c, size_t s);
        void MW_pinModeAnalogInput(uint32_t pinNumber);
        #if _RTT_UDP_ != 0
        uint8_t MW_EthernetAndUDPBeginMW_EthernetAndUDPBegin(uint8_t udpindex, uint32_t localPort);
        int MW_UDPFinalWrite(uint8_t udpindex, uint8_t data,uint8_t remoteIPOctect1,uint8_t remoteIPOctect2,uint8_t remoteIPOctect3,uint8_t remoteIPOctect4, uint32_t remoteport);
        void MW_UDPFinalRead(uint8_t udpindex, uint8_t *data,int *outStatus);
        #endif

        #if defined(_RTT_THINGSPEAK_) || (_RTT_TCP_ != 0)
        void MW_EthernetAndTCPServerBegin(uint8_t tcpindex, uint32_t serverPort);
        #endif
        #if _RTT_TCP_ != 0
        void MW_TCPFinalWrite(uint8_t tcpindex, uint8_t data, uint32_t serverPort);
        void MW_TCPFinalread(uint8_t tcpindex, uint8_t *data, uint32_t serverPort,int *outStatus);
        #endif

        #if defined(_ROTH_LEONARDO_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_)
        void MW_usbattach();
        #endif

    #endif

    #if (( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) ))
        #define MW_delay_in_milliseconds(a) 0
    #else
            void MW_delay_in_milliseconds(uint32_T period);
    #endif
#ifdef __cplusplus
}
#endif

#endif //_IO_WRAPPERS_H_
