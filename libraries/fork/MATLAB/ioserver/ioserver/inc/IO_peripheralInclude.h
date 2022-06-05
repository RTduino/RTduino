 /**
 * @file IO_peripheralInclude.h
 *
 * Contains declarations of default header files needed by IO Wrapper peripherals
 *
 * @copyright Copyright 2017 The MathWorks, Inc.
 *
 */

#ifndef IO_PERIPHERALINCLUDE_H_
#define IO_PERIPHERALINCLUDE_H_

// Do not change the order of these header files. The Debug flag is defined in "peripheralIncludes.h" //

#include "peripheralIncludes.h"
#include "PeripheralToHandle.h"
#include "IO_debug.h"

// To avoid the redefinition warning //

#ifndef BOARDNAME
#define BOARDNAME ""
#endif

#ifndef LIBNAMES
#define LIBNAMES ""
#endif

#ifndef CUSTOMDATA
#define CUSTOMDATA ""
#endif

#ifndef BOARD_IO_VERSION
#define BOARD_IO_VERSION ""
#endif

#ifndef DEBUG_FLAG
#define DEBUG_FLAG 0
#endif

#ifndef FLOAT_ENABLED
#define FLOAT_ENABLED 0
#endif

#ifdef INCLUDE_FUNCTION_MACROS
#include "functionMacros.h"
#endif

#endif /* IO_PERIPHERALINCLUDE_H_ */



