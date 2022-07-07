#include "hardware.h"
#include "IO_include.h"


extern simIOpacket RcvdPacket;
extern uint8_T PayloadBufferTxBackground;


void sendResponseMsg(uint8_T commandID, uint8_T* dataOut, uint16_T payloadSize)
{
    RcvdPacket.ptrTxPayLoad = dataOut;
    RcvdPacket.dataPayloadSize = payloadSize;
    writeToMATLAB((simIOpacket*)&RcvdPacket);
}
