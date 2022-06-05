 /**
 * @file peripheralIncludes.h
 *
 * Contains flags for standard and custom peripherals with corresponding maximum module size
 *
 * @Copyright 2017 The MathWorks, Inc.
 *
 */

#ifndef PERIPHERALINCLUDES_H_
#define PERIPHERALINCLUDES_H_

/* Define IO_STANDARD_ENABLE as 1 to consider the following flags. */

#define IO_STANDARD_ENABLE 1

#if IO_STANDARD_ENABLE

/* Standard DIGITALIO Enable */
#define IO_STANDARD_DIGITALIO 1
/* Maximum DIGITALIO Module */
#define IO_DIGITALIO_MODULES_MAX 50
/* Standard I2C Enable */
#define IO_STANDARD_I2C 1
/* Maximum I2C Module */
#define IO_I2C_MODULES_MAX 2
/* Standard ADC Enable */
#define IO_STANDARD_ADC 1
/* Maximum ADC Module */
#define IO_ADC_MODULES_MAX 10
/* Standard PWM Enable */
#define IO_STANDARD_PWM 1
/* Maximum PWM Module */
#define IO_PWM_MODULES_MAX 10
/* Standard SPI Enable */
#define IO_STANDARD_SPI 1
/* Maximum SPI Module */
#define IO_SPI_MODULES_MAX 2

#endif

/* Define IO_CUSTOM_ENABLE as 1 to consider custom function. Also include customFunction.h and compile customFunction.c in make file */
#define IO_CUSTOM_ENABLE 1

#endif /* PERIPHERALINCLUDES_H_ */



