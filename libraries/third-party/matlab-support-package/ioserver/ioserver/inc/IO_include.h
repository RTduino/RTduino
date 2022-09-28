/**
 * @file IO_include.h
 *
 * Default header file for IO Server files
 *
 * @Copyright 2017 The MathWorks, Inc.
 *
 */
#ifndef IO_INCLUDE_H_
#define IO_INCLUDE_H_

#ifdef ESB_BETA2_IOSERVER
#include "datatypes.h"
#else
#include "rtwtypes.h"
#endif

#include <string.h>
#include <stdlib.h>
/* Include stdint header file to get the definition of unsigned int data type. This is used in commandHandler method of an add-on class
 */
#include <stdint.h>


#endif /* IO_INCLUDE_H_ */
