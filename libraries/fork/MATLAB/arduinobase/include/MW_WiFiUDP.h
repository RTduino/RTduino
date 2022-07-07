/*
 * Wrappers to make WiFi UDP Send functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2017 The MathWorks, Inc. */

#if !( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER))
   #ifndef _MW_WIFIUDPSEND_H
       #define _MW_WIFIUDPSEND_H
        #include <inttypes.h>
        #include <stdio.h> /* for size_t */

        #ifdef __cplusplus
        extern "C" {
        #endif
            #if _RTT_WIFI_UDP_ != 0 //&& !(defined(_RTT_WIFI_ESP8266_))
                uint8_t MW_WifiAndUDPBegin(uint8_t wifiudpindex,uint8_t remoteIPOctet1,uint8_t remoteIPOctet2,uint8_t remoteIPOctet3,uint8_t remoteIPOctet4, uint16_t remoteport, uint16_t localport,uint8_t isSender);
                int MW_WifiUDPFinalWrite(uint8_t wifiudpindex, void *data, uint8_t remoteIPOctet1,uint8_t remoteIPOctet2,uint8_t remoteIPOctet3,uint8_t remoteIPOctet4, uint16_t remoteport, uint16_t dataSize);
                void MW_WifiUDPFinalRead(uint8_t wifiudpindex, uint8_t *data, uint16_t dataSizeinBytes, uint16_t dataTypeLength, uint16_t *outSize);
            #endif
        #ifdef __cplusplus
        }
        #endif
   #endif /*_MW_WIFIUDPSEND_H*/
#else
     /* This will be run in Rapid Accelerator Mode */
     #define MW_WifiAndUDPBegin(a,b,c,d,e,f,g,h) (0)
     #define MW_WifiUDPFinalWrite(a,b,c,d,e,f,g,h) (0)
     #define MW_WifiUDPFinalRead(a,b,c,d,e) (0)
#endif /*Rapid Accel condition check*/
