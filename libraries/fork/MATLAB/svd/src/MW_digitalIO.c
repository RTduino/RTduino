/* Copyright 2015-2017 The MathWorks, Inc. */
#include "MW_digitalIO.h"

/* Called for each GPIO pin */
MW_Handle_Type MW_digitalIO_open(uint32_T pin, uint8_T direction)
{
    return (MW_Handle_Type)NULL;
}

/* Read the logical state of a GPIO input pin */
boolean_T MW_digitalIO_read(MW_Handle_Type DigitalIOPinHandle)
{
}

/* Set the logical state of a GPIO output pin */
void MW_digitalIO_write(MW_Handle_Type DigitalIOPinHandle, boolean_T value)
{
}

/* Release resources used */
void MW_digitalIO_close(MW_Handle_Type DigitalIOPinHandle)
{
}

