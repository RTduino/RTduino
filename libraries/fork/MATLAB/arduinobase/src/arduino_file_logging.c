/*
 *
 * Copyright 2019 The MathWorks, Inc.
 *
 * File: arduino_file_logging.c
 *
 *
 */

#include <stdio.h>
#include <stdint.h>
#include "rtwtypes.h"
#include "ert_targets_logging.h"
#include "MW_SdFat_Wrapper.h"

#if !defined(MAT_FILE) || (defined(MAT_FILE) && MAT_FILE == 1)

uint8_T fileID;

FILE *MW_fopen(const char *filename, const char *mode)
{
    fileID = 1;
    uint8_t alreadyOpen;
    return (FILE *)MW_SD_fopen(filename,mode, &fileID, &alreadyOpen);
}

size_t MW_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  return MW_SD_fwrite(ptr, size, nmemb, stream, fileID);
}

size_t MW_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return MW_SD_fread(ptr, size, nmemb, stream, fileID);
}

void MW_frewind(FILE *stream)
{
    MW_SD_frewind(stream, fileID);
}

int MW_fclose(FILE *stream)
{
    return MW_SD_fclose(stream, fileID);
}

int MW_fremove(const char *filename)
{
    return MW_SD_fremove(filename);
}

#endif
