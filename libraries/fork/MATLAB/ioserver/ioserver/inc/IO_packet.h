/**
 * @file IO_packet.h
 *
 * Header file for packet.c
 *
 * @Copyright 2017-2018 The MathWorks, Inc.
 *
 */

#ifndef IO_PACKET_H_
#define IO_PACKET_H_

#include "rtiostream.h"
#include "IO_requestID.h"
#include "IO_peripheralInclude.h"
#include "IO_utilities.h"
#if CHECKSUM_ENABLE
#include "IO_checksum.h"
#endif
/*
 *  Size representation in uint16_T to enable to scale the protocol to 16bit byte targets. (C2000)
 * */


#ifndef MEMORY_ADJUSTMENT
#define MEMORY_ADJUSTMENT 1
#define rtIOStreamSend8Bits rtIOStreamSend
#define calculate_crc8_j1850_bytePacked calculate_crc8_j1850
#define calculate_crc8_j1850_withInit_bytePacked calculate_crc8_j1850_withInit
#endif

#define RTIOSTREAM_ERROR (-1)

#define HEADER_HOSTTOTARGET  (0xAA)
#define HEADER_TARGETTOHOST  (0x55)


/*uint8_T is used for data, because every target with an MAU of 16bit or 32 bit uses
 * a 16bit/32bit container to hold a 8 bit data. Its the responsibility of the target author
 * to modify this layer(packeting) to take care of processors with different MAU.
 * I see the only location that will change is the payLoad, where a byte pack would be necessary.
 */
#define HEADER_SIZE        (sizeof(uint8_T))
#define PAYLOADSIZE_SIZE   (sizeof(uint16_T)*MEMORY_ADJUSTMENT)
#define UNIQUEID_SIZE      (sizeof(uint8_T))
#define REQUESTID_SIZE     (sizeof(uint16_T)*MEMORY_ADJUSTMENT)
#define ISRAWREAD_SIZE     (sizeof(uint8_T))
#define STATUS_SIZE        (sizeof(uint8_T))   //Only used for HOST to Target, Not used for Target to Host
#define RESPONSEDATASIZE_SIZE (sizeof(uint8_T))
#define TIMESTAMP_SIZE     (sizeof(float)*MEMORY_ADJUSTMENT)
#define BYTE_ALIGNMENT_FILLER              (sizeof(uint8_T))    // To ensure double alignment of buffer size. IOProtocol overhead = 8 bytes instead of 7.


#if CHECKSUM_ENABLE
#define CHECKSUM_SIZE      (sizeof(uint8_T))
#else
#define CHECKSUM_SIZE      (0)
#endif

#ifndef MAX_PACKET_SIZE
#define MAX_PACKET_SIZE (64*sizeof(uint8_T))            /* 64 8 bit bytes both in c2000 and other targets. -> corresponding size mentioned in the IOProtocol.m */
#endif

#define PAYLOAD_SIZE (MAX_PACKET_SIZE - (HEADER_SIZE + PAYLOADSIZE_SIZE + REQUESTID_SIZE + UNIQUEID_SIZE + ISRAWREAD_SIZE + BYTE_ALIGNMENT_FILLER))

#define FUNCTION_NOT_FOUND (1)
typedef struct SimIOpacket_tag{

    uint8_T     uniqueId;
    uint8_T     isRawRead;
    uint8_T     status;
    uint8_T     peripheralDataSizeResponse;
    uint16_T    payloadSize;
    uint16_T    requestId;
    uint16_T    dataPayloadSize;
#if CHECKSUM_ENABLE
    uint8_T     checksum;
#endif
    uint8_T     *ptrTxPayLoad;
    uint8_T     *ptrRxPayLoad;
    /*
     * Limiting complete packet size to 64 bytes for ondemand
     * The least size circular buffers for serial
     * are on Arduino and c2000. Limiting to 64 bytes
     * */
}simIOpacket;

struct packetNode {
    uint8_T *packet;
    struct  packetNode *pNextPacketNode;
} ;

struct packetLinkedList {
    struct packetNode *header;
    struct packetNode *tail;
};

#ifdef __cplusplus
extern "C" {
#endif
void writeToMATLAB(simIOpacket*);
uint8_T readFromMATLAB(void);
void openConnection(void);
uint8_T calculateChecksum(simIOpacket* packet,uint8_T RxPacket);
void writeToMATLABImmediate(simIOpacket*);
#ifdef __cplusplus
}
#endif
#endif /* IO_PACKET_H_ */
