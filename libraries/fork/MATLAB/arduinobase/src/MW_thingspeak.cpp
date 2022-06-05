/*
 * Wrappers to make ThingSpeak functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2015-2019 The MathWorks, Inc. */

#if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
/* This will be run in Rapid Accelerator Mode */
/*do nothing */

#else

#include "Arduino.h"
#if (defined(_RTT_THINGSPEAK_) && (_RTT_THINGSPEAK_==1))
#include <Ethernet.h>
#endif
#if (defined(_RTT_THINGSPEAK_WIFI_) && (_RTT_THINGSPEAK_WIFI_==1) && (!defined(_RTT_WIFI_ESP8266_)))
     #if (defined(_ROTH_MKR1000_))
            #include <WiFi101.h>
     #elif (defined(_ROTH_MKRWIFI1010_))
            #include <WiFiNINA.h>
     #else
            #include <WiFi.h>
     #endif
#endif

#include "MW_thingspeak.h"
#ifdef _RTT_THINGSPEAK_WIFI_
    #include "MW_WiFiTCP.h"
#endif
#include "io_wrappers.h"

#ifdef _RTT_WIFI_ESP8266_
#include "ESP8266.h"
extern ESP8266 esp8266obj;
#else
#include <SPI.h>
#include "ThingSpeak.h"
#endif

#define MAX_URL_SIZE    (41)

extern "C" {
    void __cxa_pure_virtual(void);
}


#if (defined(_RTT_THINGSPEAK_) && (_RTT_THINGSPEAK_==1))
EthernetClient client;
String tsData = "";
boolean lastConnected = false;
#endif

#if (defined(_RTT_THINGSPEAK_WIFI_) && (_RTT_THINGSPEAK_WIFI_==1) && (!defined(_RTT_WIFI_ESP8266_)))
WiFiClient client;
String tsData = "";
boolean lastConnected = false;
#endif

// One-time initialization
extern "C" void MW_initThingSpeak(  )
{

#if (defined(_RTT_THINGSPEAK_) && (_RTT_THINGSPEAK_==1))
    MW_EthernetAndTCPServerBegin(0, 0);
#else
#if (defined(_RTT_THINGSPEAK_WIFI_) && (_RTT_THINGSPEAK_WIFI_==1))
    #ifdef _RTT_WIFI_ESP8266_
    //ThingSpeak IP Address: 184.106.153.149, Port: 80
    MW_WifiAndWifiTCPServerBegin(0, _RTT_ThingSpeak_Port, 0, _RTT_ThingSpeak_IP1, _RTT_ThingSpeak_IP2, _RTT_ThingSpeak_IP3, _RTT_ThingSpeak_IP4);
    #else
    MW_WifiAndWifiTCPServerBegin(0, 0, 0, 0, 0, 0, 0);
    #endif
#endif
#endif
    #ifndef _RTT_WIFI_ESP8266_
    IPAddress ThingSpeakIpAddress(_RTT_ThingSpeak_IP1, _RTT_ThingSpeak_IP2, _RTT_ThingSpeak_IP3, _RTT_ThingSpeak_IP4);
    ThingSpeak.begin(client,ThingSpeakIpAddress,_RTT_ThingSpeak_Port);
    #endif /* _RTT_WIFI_ESP8266_ */

}

extern "C" uint32_T MW_getCurrentTimeInMillis(void)
{
    return ((uint32_T)millis());
}

// Adds a data field to the HTTP post string
extern "C" void MW_addField(const real_T data, const uint8_T fieldNo)
{
    #ifdef _RTT_WIFI_ESP8266_
    esp8266obj.setField(fieldNo,String(data,6));
    #else
    ThingSpeak.setField(fieldNo, String(data, 6));
    #endif
}

// Adds location information to the HTTP post string
extern "C" void MW_addLocation(const real_T *location)
{
    #ifdef _RTT_WIFI_ESP8266_
    esp8266obj.setLatitude(location[0]);
    esp8266obj.setLongitude(location[1]);
    esp8266obj.setElevation(location[2]);
    #else
    ThingSpeak.setLatitude(location[0]);
    ThingSpeak.setLongitude(location[1]);
    ThingSpeak.setElevation(location[2]);
    #endif
}

// Adds a status message to the HTTP post string
extern "C" void MW_addStatus(const char *statusMessage)
{
    #ifndef _RTT_WIFI_ESP8266_
    tsData += "&status=";
    tsData += statusMessage;
    #endif
}

// Send data to ThingSpeak
extern "C" void MW_postThingSpeak(unsigned long ChannelNumber,
        const char *writeAPIKey,
        const boolean_T printDiagnosticMessages)
{
    #ifdef _RTT_WIFI_ESP8266_
    esp8266obj.writeFields(ChannelNumber, writeAPIKey);
    #else
    ThingSpeak.writeFields(ChannelNumber, writeAPIKey);
    #endif
}

extern "C" void MW_updatelastConnected(){
    #ifndef _RTT_WIFI_ESP8266_
    lastConnected=client.connected();
    #endif
}

#ifndef _RTT_WIFI_ESP8266_
extern "C" void MW_readFloatField(unsigned long channelNumber, unsigned int fieldNumber, const char * readAPIKey, float *data, int16_t *status)
{
    float fieldData;
    int16_t tempStatus;

    fieldData = ThingSpeak.readFloatField(channelNumber, fieldNumber, readAPIKey);
    tempStatus = MW_lastreadStatus();
    *status = tempStatus;
    *data = fieldData;
}

extern "C" int16_t MW_lastreadStatus(void)
{
    int16_t resultCode;
    resultCode = ThingSpeak.getLastReadStatus();
    return (resultCode);
}
#endif /* _RTT_WIFI_ESP8266_ */

#endif
// [EOF]
