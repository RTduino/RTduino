/*
 * Wrappers to make WiFi TCP Send functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2017 The MathWorks, Inc. */

#if !( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER))
   #ifndef _MW_WIFITCPSEND_H
       #define _MW_WIFITCPSEND_H
        #include <inttypes.h>
        #include <stdio.h> /* for size_t */

        #ifdef __cplusplus
        extern "C" {
        #endif
            #if defined(_RTT_THINGSPEAK_WIFI_) || _RTT_WIFI_TCP_ !=0
                void MW_WifiAndWifiTCPServerBegin(uint8_t wifitcpindex, uint16_t serverport, uint8_t isServer, uint8_t remoteIPOctet1,uint8_t remoteIPOctet2,uint8_t remoteIPOctet3,uint8_t remoteIPOctet4);
            #endif
            #if  _RTT_WIFI_TCP_ !=0
                void MW_WifiTCPFinalWrite(uint8_t wifitcpindex, void *data, uint8_t isServer, uint16_t dataSize);
                void MW_WifiTCPFinalread(uint8_t wifitcpindex, uint8_t *data, uint16_t dataSizeinBytes, int32_t *outStatus, uint8_t isServer, uint32_t blockingTimeout);
                #ifdef _RTT_WIFI_ESP8266_
                uint8_t checkESP8266BlockingMode(uint8_t *data, uint16_t dataSizeinBytes, int32_t *outStatus, unsigned long blockingTimeout);
                #else
                void checkDataBlockingMode(uint8_t wifitcpindex, uint8_t *data, uint16_t dataSizeinBytes, int32_t *outStatus,uint8_t isServer, unsigned long blockingTimeout);
                void checkClientConnectionBlockingMode(uint8_t wifitcpindex, uint8_t *data, uint16_t dataSizeinBytes, int32_t *outStatus, unsigned long blockingTimeout);
                #endif
            #endif
        #ifdef __cplusplus
        }
        #endif
   #endif /*_MW_WIFITCPSEND_H*/
#else
   /* This will be run in Rapid Accelerator Mode */
   #define MW_WifiAndWifiTCPServerBegin(a,b,c,d,e,f,g)  (0)
   #define MW_WifiTCPFinalWrite(a,b,c,d) (0)
   #define MW_WifiTCPFinalread(a,b,c,d,e,f)   (0)
#endif /*Rapid Accel condition check*/
