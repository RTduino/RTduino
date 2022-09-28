/**
 * @file IO_server.h
 *
 * Header file for IO_server.c
 *
 * @Copyright 2017-2019 The MathWorks, Inc.
 *
 */

/* Updates made in 19.1.0 - g1838210, g1832593
 */
#ifndef IO_SERVER_H_
#define IO_SERVER_H_

#include "IO_include.h"
#include "IO_packet.h"

/* Define Version number for the IO server code */
#define Major 19 /* 8 bit field */
#define Minor 2  /* 1 bit (out of 8 bits) field. 1 for 'a' and 2 for 'b' */
#define Patch 1 /* 8 bit field. 0 to 255 updates */

/* Buffer status type */
typedef enum {
    BUFFER_EMPTY = 0,
    BUFFER_FILLING,
    BUFFER_FULL
} bufferStatusType;

/* Buffer Node */
struct bufferNode {
    uint8_T *data;
    bufferStatusType status;
    struct  bufferNode *pNextBufferNode;
} ;

/* Buffer Linklist */
struct bufferLinkedList {
    struct bufferNode *header;
    struct bufferNode *tail;
};

struct streamNode {
    uint16_T    samplesperFrame;
    uint16_T    numberOfSamplesFilled;
    simIOpacket packet;
    struct      bufferLinkedList *buffer;
    struct      streamNode *pNextStreamNode;
    uint16_T    executionRate;
} ;

struct streamLinkedList{
    struct streamNode *header;
    struct streamNode *tail;
    struct streamNode *parseNode;

};

typedef struct tagStreamMode{
    uint8_T streamModeOn;   /* Set this flag to set the target in Stream mode*/
    uint8_T configureStreamMode; /* Configuration of Stream mode. Setup the Linked list for Stream mode*/
    uint8_T timeStampEnable; /* Set this flag to enable timestamp data point while streaming */
    uint8_T exitStreamingMode; /* Set once exit streaming is called */

}streamMode;


typedef struct tagDistressStatus
{
    /* 4 status values: 2 bits
     0. Not set
     1. Set
     2. Already reported and sent to the host
     3. Reserved */
    uint16_T transmissionOverrun: 2;
    uint16_T schedulerOverrun: 2;
    uint16_T esbRecordingCompleted: 2;

}distressStatus;

void ioServerInit(void);
void server(uint8_T*,uint8_T*, uint8_T);
void serverScheduler(void);


#endif /* IO_SERVER_H_ */
