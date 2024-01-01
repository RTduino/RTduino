/*
 * Copyright (c) 2021-2024, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-01     Meco Man     first version
 */

#ifndef WProgram_h
#define WProgram_h

/*
## Difference between `Arduino.h` and `WProgram.h`

In Arduino programming, both `Arduino.h` and `WProgram.h` are header files that are used to include
essential definitions and declarations for working with Arduino boards. However, there are some differences
in their usage and historical context.

1. **Arduino.h:**
   - `Arduino.h` is the modern header file used in Arduino sketches for recent versions of the Arduino IDE.
   - It includes all the necessary definitions for working with Arduino boards and peripherals.
   - `Arduino.h` is typically automatically included at the beginning of an Arduino sketch, so you don't explicitly need to add it.

2. **WProgram.h:**
   - `WProgram.h` was used in older versions of the Arduino IDE (pre Arduino 1.0).
   - It provided similar functionality as `Arduino.h` by including essential definitions and declarations.
   - In the Arduino 1.0 release and later, `WProgram.h` was replaced with `Arduino.h` for compatibility with the evolving Arduino ecosystem.

In summary, if you're using a recent version of the Arduino IDE, it's recommended to use `Arduino.h` in your sketches.
However, for older projects or if you encounter code that uses `WProgram.h`, it's likely that you're working with code
that was written for an older version of the Arduino IDE, and you may need to update it to use `Arduino.h` for compatibility with newer IDE versions.
*/

#include "Arduino.h"

#endif /* WProgram_h */
