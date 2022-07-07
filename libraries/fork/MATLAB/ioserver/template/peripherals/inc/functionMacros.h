/**
 * @file functionMacros.h
 *
 * Contains the definition of target specific macros to get runtime data
 *
 * @Copyright 2018 The MathWorks, Inc.
 *
 */

/* Example shows how to customize the implementation of getRuntimeData function in IO_server.c by defining RUNTIME_DATA macro to get data at runtime */



/*   #ifndef FUNCTION_MACROS_H
  #define FUNCTION_MACROS_H

  // customData1 and customData2 are two dummy global variables defined in IO_server.c. To access global variables from other files, use 'extern' keyword in the variable declaration inside the macro.
  #define RUNTIME_DATA ({\
  customData1 += 5;\
  customData2 *= 3;\
  uint8_T size1 = sizeof(uint8_T);\
  uint8_T size2 = sizeof(uint8_T);\
  memcpy(payloadBufferTx, &customData1, size1);\
  memcpy(&payloadBufferTx[size1], &customData2, size2);\
  *peripheralDataSizeResponse += (size1+size2);\
  })

  #endif */

