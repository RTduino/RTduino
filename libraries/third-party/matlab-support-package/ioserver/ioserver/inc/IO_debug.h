/**
 * @file IO_debug.h
 *
 * Header file for IO_debug.c
 *
 * @Copyright 2017-2019 The MathWorks, Inc.
 *
 */
// This header file should not be used individually as the DEBUG_FLAG is defined in peripheralIncludes.h //

#ifndef IO_DEBUG_H_
#define IO_DEBUG_H_
#ifdef __AVR_ARCH__
#include <avr/pgmspace.h>
#endif
#include <inttypes.h>

#if DEBUG_FLAG
#define MAX_DEBUG_LENGTH 100
#define STRINGIZE(tok) #tok
#define XSTRINGIZE(tok) STRINGIZE(tok)
#define DEBUGWARNING "Warning::Message size exceeds the maximum limit " XSTRINGIZE(MAX_DEBUG_LENGTH) ". Consider reducing the message length."
#define DEBUGWARNINGSTRLEN strlen(DEBUGWARNING)
#ifdef __AVR_ARCH__
#define PGMKEYWORD PROGMEM
#else
#define PGMKEYWORD
#endif
#endif
//Debug messages in SVD layer
#if DEBUG_FLAG == 2
#include "DebugMessageID.h"
#if defined(ARDUINO_ARCH_SAMD) // Enable inclusion of printf_float for Arduino SAMD
    #include <stdarg.h>
    asm(".global _printf_float");
#endif
#ifndef MAXNUM_DEBUG_ARGS
// maximum number of arguments each debug message can have
#define MAXNUM_DEBUG_ARGS     (12)
#endif

typedef struct __attribute__((packed)) debugMessagePacketTag
{
    uint8_T debugMsgID;
    uint8_T args[MAXNUM_DEBUG_ARGS];//structure is directly copied,adding in between debugMsgID and args,may require change in the code for payload calculation
    uint8_T argNum; // number of arguments for each message
}debugMessagePacket;

extern debugMessagePacket DebugMsg;
#endif

/* Functions to print debug information */
#ifdef __cplusplus
extern "C" {
#endif
void debugPrint(const char *fmt, ...);
void getProgramMemoryData(const char *strPGM, char *strRAM);
void sendDebugPackets();
#ifdef __cplusplus
}
#endif

#endif /* end of IO_debug.h file */
