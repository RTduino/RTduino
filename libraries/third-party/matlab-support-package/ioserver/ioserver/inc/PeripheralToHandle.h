/**
 * @file PeripheralToHandle.h
 *
 * Contains function declarations of setHanlde and getHandle functions to store and retrieve peripheral handles
 *
 * @copyright Copyright 2017 The MathWorks, Inc.
 *
 */

#ifndef PERIPHERALTOHANDLE_H
#define PERIPHERALTOHANDLE_H

#include "IO_include.h"
#include "PeripheralHandleMapType.h"

/* setHandle function to store the handle corresponding to a module in the peripheral to handle map */
void setHandle(uint32_T module, MW_Handle_Type handle, PeripheralHandleMapType *map);
/* getHandle function to retrieve the handle corresponding to a module from the peripheral to handle map */
MW_Handle_Type getHandle(uint32_T module, PeripheralHandleMapType *map);

#endif //PERIPHERALTOHANDLE_H
