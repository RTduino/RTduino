/**
 * @file PeripheralHandleMapType.h
 *
 * Defines the data type for the Peripheral to handle map array
 *
 * @copyright Copyright 2017-18 The MathWorks, Inc.
 *
 */

#ifndef PERIPHERALHANTDLEMAPTYPE_H
#define PERIPHERALHANTDLEMAPTYPE_H

#include "IO_peripheralInclude.h"
#ifdef ESB_BETA2_IOSERVER
#include "mw_driver_basetypes.h"
#else
#include "MW_SVD.h"
#endif



#ifdef USE_BIT_FOR_HANDLE
/*
Use bits for peripheral on targets with less memory resources (Arduino)
For this we use an array of bytes
*/
typedef uint8_T PeripheralHandleMapType;
#else
/* The data Type of the peripheral to handle map array is the same
   as the handle data type defined in the SVD file*/
typedef MW_Handle_Type * PeripheralHandleMapType;

#endif

#endif
