/* Copyright 2015-2017 The MathWorks, Inc. */
#ifndef __MW_DIGITALIO_H__
#define __MW_DIGITALIO_H__

#include "MW_SVD.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Direction
 * 0 - Input
 * 1 - Output
 */
typedef enum {
    MW_DIGITALIO_INPUT = 0,
    MW_DIGITALIO_OUTPUT
} MW_DigitalIO_Direction;

/* Called for each GPIO pin */
MW_Handle_Type MW_digitalIO_open(uint32_T pin, uint8_T direction);

/* Read the logical state of a GPIO input pin */
boolean_T MW_digitalIO_read(MW_Handle_Type DigitalIOPinHandle);

/* Set the logical state of a GPIO output pin */
void MW_digitalIO_write(MW_Handle_Type DigitalIOPinHandle, boolean_T value);

/* Release resources used */
void MW_digitalIO_close(MW_Handle_Type DigitalIOPinHandle);

#ifdef __cplusplus
}
#endif

#endif    /* __MW_DIGITALIO_H__ */
