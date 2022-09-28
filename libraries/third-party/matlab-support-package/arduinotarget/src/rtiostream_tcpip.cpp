/*
 * File: rtiostream_tcpip.cpp
 * Copyright 2013-2017 The MathWorks, Inc.
 */

#include "Arduino.h"
#include <inttypes.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#if defined (_ROTH_DUE_)
#define clearInt() DISABLE_SCHEDULER_INT()
#define enableInt() ENABLE_SCHEDULER_INT()
#include "arduinoARMScheduler.h"
#else
#define clearInt() cli()
#define enableInt() sei()
#include "arduinoAVRScheduler.h"
#endif

#ifndef _rtiostream
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

EthernetServer extmode_server(17725);

/* Function: rtIOStreamOpen =================================================
 * Abstract:
 *  Open the connection with the target.
 */
int rtIOStreamOpen(int argc, void * argv[])
{

    int result = RTIOSTREAM_NO_ERROR;
    int configureSuccess;
    init();

     Serial.begin( _RTT_BAUDRATE_SERIAL0_);

    IPAddress localIpAddress(_RTT_Local_IP1, _RTT_Local_IP2, _RTT_Local_IP3, _RTT_Local_IP4);
    byte mac[] = { _RTT_Local_MAC1, _RTT_Local_MAC2, _RTT_Local_MAC3, _RTT_Local_MAC4, _RTT_Local_MAC5, _RTT_Local_MAC6 };

    #if _RTT_DISABLE_Ethernet_DHCP_ !=0
        Ethernet.begin(mac, localIpAddress);
        configureSuccess = 1;
    #else
       configureSuccess = Ethernet.begin(mac);
    #endif
    if (configureSuccess==1)
    {
        // If the Configuration is successful, relay back the assigned IP address.
        Serial.print("<<< IP address :");
        for (byte thisByte = 0; thisByte < 4; thisByte++)
        {
            // Print the value of each byte of the IP address:
            Serial.print(Ethernet.localIP()[thisByte], DEC);
            if (thisByte<3)
            {
                Serial.print(".");
            }
        }
        Serial.println(" >>>");
    }
    else
    {
        // If the Configuration failed,relay back the error message.
        Serial.println("<<< IP address :Failed to configure. >>>");
    }
    extmode_server.begin();

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
    *sizeSent = 0U;
    clearInt(); //Disable base rate timer interrupt
    *sizeSent = extmode_server.write((const uint8_t *)src, size);
    enableInt(); //Enable base rate timer interrupts
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

   clearInt(); //Disable base rate timer interrupt
    EthernetClient client = extmode_server.available();
    enableInt(); //Enable base rate timer interrupts

    if (int(client) == false) {
        return RTIOSTREAM_NO_ERROR;
    }

    while ((*sizeRecvd < size)) {
       clearInt(); //Disable base rate timer interrupt
        data = client.read();
        enableInt(); //Enable base rate timer interrupts
        if (data!=-1) {
            *ptr++ = (uint8_t) data;
            (*sizeRecvd)++;
        }
    }

    return RTIOSTREAM_NO_ERROR;
}

/* Function: rtIOStreamClose ================================================
 * Abstract: close the connection.
 * For Arduino Leonardo and its variants, the Virtual COM port is handled
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
    #if defined(_ROTH_LEONARDO_) || defined(_ROTH_MKR1000_)
    while(1){
        //wait to process any USB requests.
    }
    #endif
    return RTIOSTREAM_NO_ERROR;
}
