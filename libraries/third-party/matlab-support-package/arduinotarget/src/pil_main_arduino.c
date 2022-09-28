/* Copyright 2006-2019 The MathWorks, Inc. */

/*
 * File: pil_main.c
 *
 * PIL main
 *
 */

#include "xil_interface_lib.h"
#include "Arduino.h"
#include "MW_ArduinoHWInit.h"

//#define PIL_TEST
void serial_simple_step(void);

int main(void) {
   XIL_INTERFACE_LIB_ERROR_CODE errorCode = XIL_INTERFACE_LIB_SUCCESS;
   /* avoid warnings about infinite loops */
   volatile int loop = 1;
   /* XIL initialization */
   const int argc = 0;
#ifdef PIL_TEST
   unsigned long previous = 0;
#endif

   /* Initialize system */
   init();

#if defined(_ROTH_MKR1000_) || defined(_ROTH_MKRWIFI1010_) || defined(_ROTH_MKRZERO_)
   MW_usbattach();
#endif

   MW_Arduino_Init();

   /* XIL initialization */
   void * argv = (void *) 0;
   errorCode = xilInit(argc, argv);

#ifdef PIL_TEST
   while(1)
   {
      if (millis()-previous >= 10)
      {
        previous = millis();
        serial_simple_step();
      }
   }
#endif

   if (errorCode != XIL_INTERFACE_LIB_SUCCESS) {
      /* trap error with infinite loop */
      while (loop) {
      }
   }
   /* main XIL loop */
   while(loop) {
      errorCode = xilRun();
      if ( (errorCode != XIL_INTERFACE_LIB_SUCCESS) &&
           (errorCode != XIL_INTERFACE_LIB_TERMINATE) ) {
          /* trap error with infinite loop */
          while (loop) {
          }
      }
   }

   return errorCode;
}

void serial_simple_step(void)
{

#define MAX_PACKET_SIZE 50

    char data[MAX_PACKET_SIZE];

    static unsigned int packetSize=1;
    int i;

    if (packetSize <= MAX_PACKET_SIZE) {
        size_t sizeRecvd;
        unsigned int packetIdx=0;
        rtIOStreamRecv(0, data, (size_t) packetSize, &sizeRecvd);
        packetIdx=(unsigned int) sizeRecvd;
        while (packetIdx < packetSize) {
            rtIOStreamRecv(0, &data[packetIdx], (size_t) (packetSize-packetIdx), &sizeRecvd);
            packetIdx=packetIdx+(unsigned int) sizeRecvd;
        }
        for (i=0; i<packetSize; i++) {
            /* Expected return packet is received data + 1 */
            data[i]=data[i]+1;
        }
        {
            size_t tmp;
            rtIOStreamSend( 0, data, packetSize, &tmp);
        }
        packetSize++;
    }
}
