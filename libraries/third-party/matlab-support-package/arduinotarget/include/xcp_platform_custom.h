/*
* Copyright 2018 The MathWorks, Inc.
*
* File: xcp_platform_custom.h
* Designed for Arduino boards
* Abstract:
*   Implementation of XCP Platform Abstraction Layer interface
*   for Arduino boards
*/

#ifndef XCP_PLATFORM_CUSTOM_H
#define XCP_PLATFORM_CUSTOM_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <Arduino.h>

#define XCP_PRINTF(...)

#if (defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(_ROTH_LEONARDO_))

    #define XCP_MUTEX_DEFINE(lock)
    #define XCP_MUTEX_INIT(lock)
    #define XCP_MUTEX_LOCK(lock) cli()
    #define XCP_MUTEX_UNLOCK(lock) sei()

    /*Maximum bytes the target requests the host to send per XCP frame. */
    #define XCP_MAX_CTO_SIZE 32

    #define XCP_ADDRESS_GET(addressExtension, address)  (uint8_T*) ((uintptr_t) address)

    #define XCP_ADDRESS_GRANULARITY   XCP_ADDRESS_GRANULARITY_BYTE

    #define XCP_TIMESTAMP_GET         xcpGetTimestamp

    #define XCP_SLEEP(s, us) delay(1000*(s) + (us)/1000 )

    #define XCP_MEM_ALIGNMENT 4

    /* 2K RAM */
    /* Example:
     XCP_MEM_DAQ_RESERVED_POOLS_NUMBER( Number of Rates in the Model) = 4
     XCP_MAX_ODT_ENTRIES_COUNT - 10
     XCP_MEM_BLOCK_1_NUMBER - 4
     XCP_MEM_BLOCK_1_SIZE - 32 bytes * 4
     * Condition: 4*32 < 8*10 --> False (Else part)
     XCP_MEM_BLOCK_2_SIZE - 8 * 10 (ODT Entries)
     XCP_MEM_BLOCK_2_NUMBER - 4
     XCP_MEM_BLOCK_3_SIZE - 4 * 32
     XCP_MEM_BLOCK_3_NUMBER - 1
     Total = 336 bytes [32*4 + 8*10 + 4*32 = 128 + 80 + 128] */

    /* #define XCP_MEM_BLOCK_1_SIZE 32 [xcp_mem_config.h] */
    #define XCP_MEM_BLOCK_1_NUMBER XCP_MEM_DAQ_RESERVED_POOLS_NUMBER
    #define XCP_MAX_ODT_ENTRIES_COUNT 10
    #if ((XCP_MEM_DAQ_RESERVED_POOLS_NUMBER) * 32) < (8 * XCP_MAX_ODT_ENTRIES_COUNT)
            #define XCP_MEM_BLOCK_2_SIZE ((XCP_MEM_DAQ_RESERVED_POOLS_NUMBER) * 32)
         /* #define XCP_MEM_BLOCK_2_NUMBER 1  [xcp_mem_config.h] */

         /* #define XCP_MEM_BLOCK_3_SIZE (8 * XCP_MAX_ODT_ENTRIES_COUNT)  [xcp_mem_config.h] */
            #define XCP_MEM_BLOCK_3_NUMBER (XCP_MEM_DAQ_RESERVED_POOLS_NUMBER)
    #else
            #define XCP_MEM_BLOCK_2_SIZE (8 * XCP_MAX_ODT_ENTRIES_COUNT)
            #define XCP_MEM_BLOCK_2_NUMBER (XCP_MEM_DAQ_RESERVED_POOLS_NUMBER)

            #define XCP_MEM_BLOCK_3_SIZE ((XCP_MEM_DAQ_RESERVED_POOLS_NUMBER) * 32)
            #define XCP_MEM_BLOCK_3_NUMBER 1
    #endif
    #define XCP_MEM_DAQ_RESERVED_POOL_BLOCKS_NUMBER 2


#elif (defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_ADK))

    #define XCP_MUTEX_DEFINE(lock)
    #define XCP_MUTEX_INIT(lock)
    #define XCP_MUTEX_LOCK(lock) cli()
    #define XCP_MUTEX_UNLOCK(lock) sei()

    /*Maximum bytes the target requests the host to send per XCP frame. */
    #define XCP_MAX_CTO_SIZE 32
    #define XCP_ADDRESS_GET(addressExtension, address)  (uint8_T*) ((uintptr_t) address)
    #define XCP_ADDRESS_GRANULARITY   XCP_ADDRESS_GRANULARITY_BYTE
    #define XCP_TIMESTAMP_GET         xcpGetTimestamp
    #define XCP_SLEEP(s, us) delay(1000*(s) + (us)/1000 )
    #define XCP_MEM_ALIGNMENT 4

    /* 8K RAM */
    #define XCP_MEM_BLOCK_1_NUMBER XCP_MEM_DAQ_RESERVED_POOLS_NUMBER
    #define XCP_MEM_BLOCK_2_SIZE      ((XCP_MEM_DAQ_RESERVED_POOLS_NUMBER) * 32)
    #define XCP_MAX_ODT_ENTRIES_COUNT 40
    #define XCP_MEM_BLOCK_3_NUMBER     (XCP_MEM_DAQ_RESERVED_POOLS_NUMBER)

#elif defined(__SAMD21G18A__)

    #define XCP_MUTEX_DEFINE(lock)          uint32_t lock
    #define XCP_MUTEX_INIT(lock)            lock = 0
    #define XCP_MUTEX_LOCK(lock)            lock = sys_arch_protect()
    #define XCP_MUTEX_UNLOCK(lock)          sys_arch_unprotect(lock)

    #define XCP_ADDRESS_GET(addressExtension, address)  (uint8_T*) ((uintptr_t) address)

    #define XCP_ADDRESS_GRANULARITY   XCP_ADDRESS_GRANULARITY_BYTE

    #define XCP_TIMESTAMP_GET         xcpGetTimestamp

    #define XCP_SLEEP(s, us) delay(1000*(s) + (us)/1000 )

    #define XCP_MEM_ALIGNMENT 4

    /* 32K RAM */

#elif defined(ARDUINO_ARCH_SAM)

    #define XCP_MUTEX_DEFINE(lock)          uint32_t lock
    #define XCP_MUTEX_INIT(lock)            lock = 0
    #define XCP_MUTEX_LOCK(lock)            lock = sys_arch_protect()
    #define XCP_MUTEX_UNLOCK(lock)          sys_arch_unprotect(lock)

    #define XCP_ADDRESS_GET(addressExtension, address)  (uint8_T*) ((uintptr_t) address)

    #define XCP_ADDRESS_GRANULARITY   XCP_ADDRESS_GRANULARITY_BYTE

    #define XCP_TIMESTAMP_GET         xcpGetTimestamp

    #define XCP_SLEEP(s, us) delay(1000*(s) + (us)/1000 )

    #define XCP_MEM_ALIGNMENT 4

    /* 96K RAM */
#endif

#endif
