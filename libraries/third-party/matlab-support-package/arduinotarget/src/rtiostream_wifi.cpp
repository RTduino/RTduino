/*
 * File: rtiostream_serial.cpp
 * Copyright 2011-2019 The MathWorks, Inc.
 */

#include "Arduino.h"
#include <inttypes.h>
#include <SPI.h>
#if (defined(_ROTH_MKR1000_))
    #include <WiFi101.h>
#elif (defined(_ROTH_MKRWIFI1010_))
    #include <WiFiNINA.h>
#else
    #include <WiFi.h>
#endif
/* Uncomment to use clearInt() or enableInt() for interrupts
#if defined (_ROTH_DUE_) || defined (_ROTH_MKR1000_)
    #define clearInt() DISABLE_SCHEDULER_INT()
    #define enableInt() ENABLE_SCHEDULER_INT()
    #if defined (_ROTH_DUE_)
        #include "arduinoARMScheduler.h"
    #else
        #include "arduinoARM_M0plusScheduler.h"
    #endif
#else
    #define clearInt() cli()
    #define enableInt() sei()
    #include "arduinoAVRScheduler.h"
#endif
*/

#ifndef _rtiostream

//#define that helps to stringify build flags
//Double evaluation is needed so that the double quotes can be derived out
//of the build flag and can be assigned to a character array
#define RTT_StringifyBuildFlag(x) RTT_StringParamExpanded(x)
#define RTT_StringParamExpanded(x)  #x

extern "C" {
#include "rtiostream.h"
}
#define _rtiostream
#endif

extern "C" void __cxa_pure_virtual(void);

/* The variable below has no use in this file. Used to preserve
 * compatibility with rtiostream_interface
 */
volatile boolean receivedSyncByteE = false;

WiFiServer extmode_wifi_server(17725);
WiFiClient extmode_wifi_client;

int extmodewifistatus = WL_IDLE_STATUS;     // the Wifi radio's extmodewifistatus


/* Function: rtIOStreamOpen =================================================
 * Abstract:
 *  Open the connection with the target.
 */
int rtIOStreamOpen(int argc, void * argv[])
{
    int result = RTIOSTREAM_NO_ERROR;
    char ssid[] = RTT_StringifyBuildFlag(_RTT_WIFI_SSID);
    IPAddress wifiLocalIpAddress(_RTT_WIFI_Local_IP1, _RTT_WIFI_Local_IP2, _RTT_WIFI_Local_IP3, _RTT_WIFI_Local_IP4);


#ifdef _RTT_WIFI_WEP
char key[] = RTT_StringifyBuildFlag(_RTT_WIFI_KEY);
int keyIndex = _RTT_WIFI_KEY_INDEX;
#endif

#ifdef _RTT_WIFI_WPA
char wpapass[] = RTT_StringifyBuildFlag(_RTT_WIFI_WPA_PASSWORD);
#endif

#if _RTT_DISABLE_Wifi_DHCP_ !=0
    WiFi.config(wifiLocalIpAddress);
#endif

    Serial.begin( _RTT_BAUDRATE_SERIAL0_);
    while ( extmodewifistatus != WL_CONNECTED) {

#ifdef _RTT_WIFI_WEP
       extmodewifistatus = WiFi.begin(ssid,keyIndex,key);
#endif

#ifdef _RTT_WIFI_WPA
        extmodewifistatus = WiFi.begin(ssid,wpapass);
#endif

#ifdef _RTT_WIFI_NONE
        extmodewifistatus = WiFi.begin(ssid);
#endif
        delay(10000);
    }
    extmode_wifi_server.begin();
    if (extmodewifistatus==WL_CONNECTED)
    {
        // If the Configuration is successful, relay back the assigned IP address.
        IPAddress ip = WiFi.localIP();
        Serial.print("<<<IP address: ");
        Serial.print(ip);
        Serial.println(">>>");
    }
    else
    {
        // If the Configuration failed,relay back the error message.
        Serial.println("<<< IP address :Failed to configure. >>>");
    }
    return result;

}

/* Function: rtIOStreamSend =====================================================
 * Abstract:
 *  Sends the specified number of bytes on the serial line. Returns the number of
 *  bytes sent (if successful) or a negative value if an error occurred.
 */
int rtIOStreamSend(
        int          streamID,
        const void * src,
        size_t       size,
        size_t     * sizeSent)
{
  #if defined(_ROTH_MKR1000_) || defined(_ROTH_MKRWIFI1010_)
     /*Check for available clients only when not connected*/
    if(!extmode_wifi_client.connected()){
        extmode_wifi_client = extmode_wifi_server.available();
    }

    if (!extmode_wifi_client) {
        return RTIOSTREAM_ERROR;
    }
    extmode_wifi_client.write((const uint8_t *) src, (int16_t) size);
    *sizeSent = size;
  #else
    /*Writing byte by byte else WiFi Shield gives ExtPktPending() error
     * in external mode for 2nd EXT_CONNECT_RESPONSE (g1626221)
     */
    *sizeSent = 0U;
    uint8_t data;
    while (((*sizeSent) < size)) {
        data = *((uint8_t *) src + *sizeSent);
        extmode_wifi_server.write(data);
        (*sizeSent)++;
    }
  #endif
  return RTIOSTREAM_NO_ERROR;
}

/* Function: rtIOStreamRecv ================================================
 * Abstract: receive data
 *
 */
int rtIOStreamRecv(
        int      streamID,
        void   * dst,
        size_t   size,
        size_t * sizeRecvd)
{
    int data;
    uint8_t * ptr = (uint8_t *)dst;

    *sizeRecvd = 0U;
    /*Check for available clients only when not connected*/
    if(!extmode_wifi_client.connected()){
        extmode_wifi_client = extmode_wifi_server.available();
    }

    if (!extmode_wifi_client) {
        return RTIOSTREAM_ERROR;
    }
    int availableBytes = extmode_wifi_client.available();
    while ((*sizeRecvd < size) && (availableBytes > 0)) {
        data = extmode_wifi_client.read();
        if (data!=-1) {
            *ptr++ = (uint8_t) data;
            (*sizeRecvd)++;
        }
    }
    return RTIOSTREAM_NO_ERROR;
}

/* Function: rtIOStreamClose ================================================
 * Abstract: close the connection.
 * For Arduino Leonardo + its variants and MKR 1000, the Virtual COM port is handled
 * by the controller. In case the code running on the target exits main,
 * the COM port cannot be accessed until a hard reset is performed.
 * To over come this issue, a while loop is added to make sure that
 * upon getting a stop command from external mode, the code running on
 * the target stops but the code will not exit the main.
 * This will ensure that the COM port is accessible even after the
 * external mode has been stopped.
 *
 */
int rtIOStreamClose(int streamID)
{
#if defined(_ROTH_LEONARDO_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRWIFI1010_)
    while(1){
        //wait to process any USB requests.
    }
#endif
    return RTIOSTREAM_NO_ERROR;
}
