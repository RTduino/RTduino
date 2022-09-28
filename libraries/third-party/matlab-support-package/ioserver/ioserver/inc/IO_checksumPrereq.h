/* Copyright 2018 The MathWorks, Inc. */

#if ESB_BETA2_IOSERVER
#include "datatypes.h"  //contains the definitions for datatypes like uint8_T.
// note the '_T', stdint.h appends '_t'
#else
#include "rtwtypes.h"
#endif


#define CRC_TABLE crc8_table_j1850

#ifdef __AVR_ARCH__
#include <avr/pgmspace.h>
#define FLASH_QUALIFIER PROGMEM
#define CRC8_TABLE_J1850(x) pgm_read_byte_near((uint16_T)CRC_TABLE + (uint16_T)(x))
#else
#define FLASH_QUALIFIER
#define CRC8_TABLE_J1850(x) CRC_TABLE[x]
#endif
