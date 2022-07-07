/**
 * @file IO_packet.c
 *
 * Contains declaration of functions used for receiving and sending the IO packets over transport layer.
 *
 * @Copyright 2017-2018 The MathWorks, Inc.
 *
 */

#include "IO_packet.h"


extern void disableGlobalInterrupt(void);
extern void enableGlobalInterrupt(void);

void blockingRead(void *dst, size_t sizeToRead, size_t *sizeRead);
// This is for checking if the header of any packet has arrived without blocking the streaming mode operation
void nonblockingReadForHeader(void *dst, size_t sizeToRead, size_t *sizeRead);

simIOpacket RcvdPacket;
static uint8_T RxPacketHeader;
const uint8_T TxPacketHeader = HEADER_TARGETTOHOST;

#if CHECKSUM_ENABLE
uint8_T crcMallocFailedStreamCon = 0;
#endif

void openConnection(void)
{
    rtIOStreamOpen(0,0);
}

uint8_T readFromMATLAB(void)
{
    size_t payloadSize=0;
    size_t sizeRecvd=0;
    uint8_T temp[2]={0,0};
    uint16_T packBytesInto16BitIntigerData = 0;
    // Only for Soft Real Time implementation
    nonblockingReadForHeader((uint8_T*)&RxPacketHeader, (size_t)HEADER_SIZE, &sizeRecvd);

    if((sizeRecvd == 1) && (RxPacketHeader == HEADER_HOSTTOTARGET))
    {
        blockingRead(temp, (size_t)(PAYLOADSIZE_SIZE), &sizeRecvd);
        packBytesInto16BitIntigerData = (uint16_T)packBytesInto16BitInteger(temp);
        memcpy((uint8_T*)&RcvdPacket.payloadSize,&packBytesInto16BitIntigerData,sizeof(uint16_T));
        payloadSize = RcvdPacket.payloadSize;

        blockingRead((uint8_T*)&RcvdPacket.uniqueId, (size_t)UNIQUEID_SIZE, &sizeRecvd);
        payloadSize-=(uint8_T)UNIQUEID_SIZE;        //uniqueId - read complete

        blockingRead(temp, (size_t)(REQUESTID_SIZE), &sizeRecvd);
        packBytesInto16BitIntigerData = (uint16_T)packBytesInto16BitInteger(temp);
        memcpy((uint8_T*)&RcvdPacket.requestId,&packBytesInto16BitIntigerData,sizeof(uint16_T));
        payloadSize-=(uint8_T)REQUESTID_SIZE;

        blockingRead((uint8_T*)&RcvdPacket.isRawRead, (size_t)ISRAWREAD_SIZE, &sizeRecvd);
        payloadSize-=(uint8_T)ISRAWREAD_SIZE;       //isRawRead - read complete

        if(RcvdPacket.isRawRead)
        {
            blockingRead((uint8_T*)&RcvdPacket.peripheralDataSizeResponse, (size_t)RESPONSEDATASIZE_SIZE, &sizeRecvd);
            payloadSize-=(uint8_T)RESPONSEDATASIZE_SIZE;        //isRawRead - read complete
        }
        else
        {
            RcvdPacket.peripheralDataSizeResponse = 0;
        }

        RcvdPacket.dataPayloadSize = payloadSize-CHECKSUM_SIZE;
        blockingRead((uint8_T*)RcvdPacket.ptrRxPayLoad, (size_t)(RcvdPacket.dataPayloadSize), &sizeRecvd);
    #if CHECKSUM_ENABLE
        blockingRead((uint8_T*)&RcvdPacket.checksum, (size_t)CHECKSUM_SIZE, &sizeRecvd);
    #endif
        /*
         * If received header for the packet
         * then read the rest of the content from the
         * packet, else wait until a header is received
         * if data is dropped on the serial line.  We
         * continue to search for the header.
         * If header is present in the payLoad then checksum
         * is a validation mechanism for correct data.
         * The protocol assumes that there is no data dropped
         * during communication.
         * Host is aware of the size of the target buffer, so the
         * data transfer from host is regulated
         * */
    #if CHECKSUM_ENABLE
        if ( RcvdPacket.checksum == calculateChecksum((simIOpacket*) &RcvdPacket,1) )
        {
            return 1;
        }
    #else
        return 1;
    #endif
    }
    return 0;

}

void blockingRead(void *dst, size_t sizeToRead, size_t *sizeRead)
{
    size_t size=0;
    uint8_T* dst_temp = (uint8_T*)dst;
    int8_T status = 0;
    *sizeRead = 0;
    while(*sizeRead < sizeToRead)
    {
        status = rtIOStreamRecv((int)0, &dst_temp[*sizeRead], (size_t)(sizeToRead-*sizeRead), &size);
        *sizeRead+=size;
        if(status == RTIOSTREAM_ERROR)
        {
            rtIOStreamOpen(0,0);
        }
    }

    //<C2000 change here> For C2000 byte pack the 2 eight bit header into a 16 bit header

}

/* This NonblockingRead function is called only for the header. If there is no data in the serial buffer,
 * the loop does not wait till the data comes. It rather goes back and resumes normal operation.
 * Required only for Soft Real Time implementation.
 */

/* This function is exactly same as blockingRead except this does not wait in a while loop until desired bytes are collected
 */
void nonblockingReadForHeader(void *dst, size_t sizeToRead, size_t *sizeRead)
{
    rtIOStreamRecv((int)0, &((uint8_T*)dst)[*sizeRead], (size_t)sizeToRead, sizeRead);
}

void writeToMATLABImmediate(simIOpacket *packet)
{
    size_t sizeSent=0;
    /* Send the packet immediately */
    rtIOStreamSend8Bits((int)0, &TxPacketHeader, (size_t)HEADER_SIZE, &sizeSent);
    rtIOStreamSend((int)0, &packet->payloadSize, sizeof(uint16_T), &sizeSent);
    rtIOStreamSend8Bits((int)0, &packet->uniqueId, (size_t)UNIQUEID_SIZE, &sizeSent);
    rtIOStreamSend((int)0, &packet->requestId, sizeof(uint16_T), &sizeSent);
    rtIOStreamSend8Bits((int)0, &packet->isRawRead, (size_t)ISRAWREAD_SIZE, &sizeSent);
    rtIOStreamSend8Bits((int)0, &packet->status, (size_t)STATUS_SIZE, &sizeSent);
    while(packet->dataPayloadSize > (uint16_T)PAYLOAD_SIZE)
    {
        rtIOStreamSend8Bits((int)0, packet->ptrTxPayLoad, (size_t)PAYLOAD_SIZE, &sizeSent);
        packet->ptrTxPayLoad = packet->ptrTxPayLoad + (uint16_T)PAYLOAD_SIZE;
        packet->dataPayloadSize = packet->dataPayloadSize - (uint16_T)PAYLOAD_SIZE;
    }
    rtIOStreamSend8Bits((int)0, packet->ptrTxPayLoad, (size_t)packet->dataPayloadSize, &sizeSent);
#if CHECKSUM_ENABLE
    rtIOStreamSend8Bits((int)0, &packet->checksum, (size_t)CHECKSUM_SIZE, &sizeSent);
#endif
}

void writeToMATLAB(simIOpacket* packet)
{
    packet->payloadSize = packet->dataPayloadSize + UNIQUEID_SIZE + REQUESTID_SIZE + ISRAWREAD_SIZE + STATUS_SIZE + CHECKSUM_SIZE;

#if CHECKSUM_ENABLE
    /* Calculate the checksum */
    packet->checksum = calculateChecksum((simIOpacket*) packet,0);
#endif
    writeToMATLABImmediate((simIOpacket*) packet);
    // for targets with MAU other than 8bit, code change is required here
}


#if CHECKSUM_ENABLE
uint8_T calculateChecksum(simIOpacket* packet,uint8_T RxPacket)
{
    uint8_T checksumCalculated = 0;
    checksumCalculated = calculate_crc8_j1850_bytePacked((const uint8_T *)&packet->payloadSize,(uint16_T)0,(uint16_T)PAYLOADSIZE_SIZE); /* Calculate payload size checksum */
    checksumCalculated = calculate_crc8_j1850_withInit((const uint8_T *)&packet->uniqueId,(uint8_T)checksumCalculated,(uint16_T)0,(uint16_T)UNIQUEID_SIZE); /* Calculate uniqueID checksum */
    checksumCalculated = calculate_crc8_j1850_withInit_bytePacked((const uint8_T *)&packet->requestId,(uint8_T)checksumCalculated,(uint16_T)0,(uint16_T)REQUESTID_SIZE); /* Calculate request ID checksum */
    checksumCalculated = calculate_crc8_j1850_withInit((const uint8_T *)&packet->isRawRead,(uint8_T)checksumCalculated,(uint16_T)0,(uint16_T)ISRAWREAD_SIZE); /* Calculate is raw read checksum */

    if (RxPacket)
    {
        if(RxPacket && (packet->isRawRead & 0x01))
        {
            /* Calculate the checksum for response size which is only present in received isRawRead packets */
            checksumCalculated = calculate_crc8_j1850_withInit((const uint8_T *)&packet->peripheralDataSizeResponse,(uint8_T)checksumCalculated,(uint16_T)0,(uint16_T)(RESPONSEDATASIZE_SIZE)); /* Calculate response data size checksum */
        }
        checksumCalculated = calculate_crc8_j1850_withInit((const uint8_T *)packet->ptrRxPayLoad,(uint8_T)checksumCalculated,(uint16_T)0,(uint16_T)(packet->dataPayloadSize)); /* Calculate payload checksum */
    }
    else
    {
        /* Calculate the checksum for status which is only present in Tx packets */
        checksumCalculated = calculate_crc8_j1850_withInit((const uint8_T *)&packet->status,(uint8_T)checksumCalculated,(uint16_T)0,(uint16_T)(STATUS_SIZE)); /* Calculate status checksum */
        checksumCalculated = calculate_crc8_j1850_withInit((const uint8_T *)packet->ptrTxPayLoad,(uint8_T)checksumCalculated,(uint16_T)0,(uint16_T)(packet->dataPayloadSize)); /* Calculate payload checksum */
    }

    return checksumCalculated;
}
#endif
