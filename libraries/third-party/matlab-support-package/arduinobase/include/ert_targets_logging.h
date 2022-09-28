/* Wrapper header for ERT Targets Logging
 *
 * Copyright 2016 - 2017 The MathWorks, Inc.
 */

#ifndef _ERT_TARGETS_LOGGING_H_
#define _ERT_TARGETS_LOGGING_H_
#include "rtwtypes.h"
#include <stdio.h>
#include <stdlib.h>

#if (defined(MAT_FILE) && MAT_FILE == 1)
/*TODO:: Add a header to add this define */
extern volatile uint8_T bStopLogging;
FILE *MW_fopen(const char *filename, const char *mode);
size_t MW_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t MW_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
void MW_rewind(FILE *stream);
int MW_fclose(FILE *stream);
int MW_remove(const char *filename);
#endif

#endif
