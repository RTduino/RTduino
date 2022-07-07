/**
 * @file IO_debug.c
 *
 * contains function to print debug messages
 *
 * @Copyright 2017-2019 The MathWorks, Inc.
 *
 */
#include "IO_peripheralInclude.h"
#include "IO_packet.h"
#include "IO_requestID.h"
#if DEBUG_FLAG
#include <stdarg.h>
#include <stdio.h>
#ifdef __AVR_ARCH__
const char MSG_DEBUGWARNING[] PROGMEM = DEBUGWARNING;
#else
const char MSG_DEBUGWARNING[] = DEBUGWARNING;
#endif
#endif
/* In the default debug comments we have used PRIu16 instead of PRIu8 for 8 bit data type.
 * This is because the newlib-nano is already compiled without a definition for 'hhu' (PRIu8).
 */
#if DEBUG_FLAG == 2
debugMessagePacket DebugMsg = {0,{0},0};
#endif

void debugPrint(const char *fmt, ...)
{
#if DEBUG_FLAG
    {
    char str[MAX_DEBUG_LENGTH];
    char tmp[MAX_DEBUG_LENGTH]; // destination for vsnprintf
    uint8_t i = 0;
    simIOpacket packet;
    va_list vl;
    va_start(vl, fmt);
    packet.dataPayloadSize = 0;
    packet.uniqueId = 0;
    packet.requestId = DEBUGINFORMATION;
    packet.isRawRead = 1;
    packet.status = 0;

    /* Different implementation for AVR architecture since constants are stored in program memory and they need to be fetched from there */
    while(1)
    {
#ifdef __AVR_ARCH__
        str[i] = pgm_read_byte(fmt++);
#else
        str[i] = (char) *(fmt++);
#endif
        if((str[i] == '\0') || ((MAX_DEBUG_LENGTH-1) == i))
        {
            break;
        }
        i++;
    }
    if(str[i] == '\0')
    {
        packet.dataPayloadSize = vsnprintf(tmp, sizeof(tmp), str, vl);
    }
    if((packet.dataPayloadSize > sizeof(tmp)) || (str[i] != '\0'))
    {
        // length after adding the variables to the string exceeds max limit. print warning message instead.
        getProgramMemoryData(MSG_DEBUGWARNING, str);
        packet.dataPayloadSize = DEBUGWARNINGSTRLEN;
        packet.ptrTxPayLoad = (uint8_T*)str;
    }
    else
    {
        packet.ptrTxPayLoad = (uint8_T*)tmp;
    }
    writeToMATLAB((simIOpacket *)&packet);
    va_end(vl);
    }
#endif
}

void getProgramMemoryData(const char *strPGM, char *strRAM)
{
    int strLen = 0;
    while(1)
    {
#ifdef __AVR_ARCH__
        strRAM[strLen] = (char) pgm_read_byte(strPGM+strLen);
#else
        strRAM[strLen] = (char) *(strPGM+strLen);
#endif
        if(strRAM[strLen] == '\0')
        {
            break;
        }
        strLen++;
    }
}

void sendDebugPackets()
{
#if DEBUG_FLAG == 2
    simIOpacket packet;
    packet.uniqueId = 0;
    packet.requestId = INTERNALDEBUGINFORMATION;
    packet.isRawRead = 1;
    packet.status = 0;
    packet.dataPayloadSize =  sizeof(DebugMsg.debugMsgID)+(DebugMsg.argNum)*sizeof(DebugMsg.args[0]);
    packet.ptrTxPayLoad = (uint8_T*)&DebugMsg;
    writeToMATLAB((simIOpacket *)&packet);
#endif
}
