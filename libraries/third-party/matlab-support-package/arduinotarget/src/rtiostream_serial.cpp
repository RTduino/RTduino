/*
 * File: rtiostream_serial.cpp
 * Copyright 2011-2019 The MathWorks, Inc.
 */

#include "Arduino.h"

#if defined (_ROTH_DUE_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_)
#define clearInt()
#define enableInt()
#else
#define clearInt() cli()
#define enableInt() sei()
#endif

#ifndef _rtiostream

extern "C" {
#include "rtiostream.h"
}
#define _rtiostream
#endif

extern "C" void __cxa_pure_virtual(void);

/* Function: rtIOStreamOpen =================================================
 * Abstract:
 *  Open the connection with the target.
 */
int rtIOStreamOpen(int argc, void * argv[])
{
    /* ASCII character a is 65 */
    #define RTIOSTREAM_OPEN_COMPLETE 65
    static const uint8_t init_complete = RTIOSTREAM_OPEN_COMPLETE;

    int result = RTIOSTREAM_NO_ERROR;
    int flushedData;

    #ifndef MW_PIL_ARUDINOSERIAL
     init();
    #endif

    #ifdef MW_PIL_ARUDINOSERIAL
      Serial.begin(MW_PIL_SERIAL_BAUDRATE);

     /* At high baud rates (i.e. 115200), the Arduino is receiving an
      * initial byte of spurious data (0xF0 / 240) when opening a connection
      * even though the host has not transmitted this data! This is causing
      * an issue for PIL to read wrong init bytes at the beginning and
      * loosing sync with host. Adding delay of 1 sec to wait for host to
      * open the connection and then flush the spurious data from receive
      * buffer. A delay of 5Sec(rtiostream postopenpause) is given on the
      * host between opening the connection and init bytes.
     */
      delay(1000);
    #else
        Serial.begin(115200);
    #endif

    /* Flush out the serial receive buffer when opening a connection. This
     * works around an issue we've noticed with Arduino at high baud rates.
     * At high baud rates (i.e. 115200), the Arduino is receiving an
     * initial byte of spurious data (0xF0 / 240) even though the host has
     * not transmitted this data! This may cause an issue for PIL and
     * External mode during the handshaking process.
     */
    while (Serial.available()) {
        flushedData = Serial.read();
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
        clearInt();
        Serial.write( (const uint8_t *)src, (int16_t)size);
        enableInt();

        *sizeSent = size;

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

    if (!Serial.available()) {
        return RTIOSTREAM_NO_ERROR;
    }

    while ((*sizeRecvd < size)) {
        data = Serial.read();
         if (data!=-1) {
             *ptr++ = (uint8_t) data;
            (*sizeRecvd)++;
         }
    }

    return RTIOSTREAM_NO_ERROR;
}

/* Function: rtIOStreamClose ================================================
 * Abstract: close the connection.
 *For External mode over serial, Arduino leonardo/MKR and its variats require
 *a flush out the serial receive buffer. This is done to get the last
 * acknowledgement
 *
 */
int rtIOStreamClose(int streamID)
{
    delay(1000);
#if defined(_ROTH_LEONARDO_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_)
    int flushedData;
    while (Serial.available()) {
        flushedData = Serial.read();
    }
#endif
    return RTIOSTREAM_NO_ERROR;
}
