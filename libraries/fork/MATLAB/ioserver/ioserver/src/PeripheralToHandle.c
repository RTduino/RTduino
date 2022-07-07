/**
 * @file PeripheralToHandle.c
 *
 * Defines the setHandle and getHandle functions to store and retrieve peripheral handles
 *
 * @copyright Copyright 2017-18 The MathWorks, Inc.
 *
 */

#include "PeripheralToHandle.h"

/* setHandle function stores the handle corresponding to a module or DigitalIO pin in the peripheral to handle map
   module is the Peripheral module number
   handle is the Peripheral handle returned by the open function in SVD
   map is the array that stores handles corresponding to each module*/
void setHandle(uint32_T module, MW_Handle_Type handle, PeripheralHandleMapType *map)
{
    #ifdef USE_BIT_FOR_HANDLE
    /*
    Assumes pin number from count zero
    */
        uint16_T ByteNo=0;
        uint8_T BitPositionNo=0;
        PeripheralHandleMapType tempVar=0;
        /*
        Integer division rounds off to value providing the abs byte no
        */
        ByteNo = module/8;
        BitPositionNo = module%8;

        tempVar = (1<<BitPositionNo);
        /*
        The assumption is module number is the handle,
        but when handle is Null, reset the handle
        */
        if(!handle)
        {
            /*reset the handle*/
            tempVar = ~tempVar;
            map[ByteNo] = map[ByteNo] & tempVar;
        }
        else
        {
            /*set the handle*/
            map[ByteNo] = map[ByteNo] | tempVar;
        }



    #else
        map[module] = handle;
    #endif
}

/* getHandle function retrieves the handle corresponding to a module or DigitalIO pin from the peripheral to handle map */
MW_Handle_Type getHandle(uint32_T module, PeripheralHandleMapType *map)
{
    #ifdef USE_BIT_FOR_HANDLE
        uint16_T ByteNo=0, handleValue=0;

        uint8_T BitPositionNo=0;

        ByteNo = module/8;
        BitPositionNo = module%8;

        handleValue = map[ByteNo] & (PeripheralHandleMapType)(1<<BitPositionNo);

        if(handleValue)
        {
            /*
            Returns the value of the module if the pin is opened earlier
            */
            handleValue = module + 1;
            /*
            This allows to differentiate between pin zero and a Null condition
            Inline with Arduinos strategy of handling pin map
            Having this code here is enough. (the SVD open functions can avoid doing this)

            Another option - return true, and then remove the decrement in the SVD of all functions in Arduino
            */
        }
        else
        {
            handleValue = 0;
        }
        return (MW_Handle_Type)handleValue;

    #else
        return map[module];
    #endif
}
