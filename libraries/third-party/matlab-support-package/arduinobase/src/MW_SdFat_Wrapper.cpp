/*
 *
 * Copyright 2019 The MathWorks, Inc.
 *
 * File: MW_SdFat_Wrapper.cpp
 *
 * Abstract:
 *  Wrapper functions for SdFat Library
 *
 */

#if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
/* This will be run in Rapid Accelerator Mode */
/*do nothing*/
#else

#include <Arduino.h>
#include <SPI.h>
#include <inttypes.h>
#include <string.h>
#include "SdFat.h"
#include "MW_SdFat_Wrapper.h"

//Global variable declerations
int initialized;
SdFat sd;
int dummyFilePointer;
uint8_t currNFiles = 0;

SdFile check;

// MAT file not defined and number of SD read blocks is > 1
#if (defined(_RTT_NUMSDFILEREADBLOCKS_) && (_RTT_NUMSDFILEREADBLOCKS_ != 1)) && !defined(_RTT_MATFILE_LOG)
    SdFile logFile[_RTT_NUMSDFILEREADBLOCKS_];
    #define FILELOG(id) logFile[id]
// MAT file defined and SD read block is also present
#elif defined(_RTT_NUMSDFILEREADBLOCKS_) && (defined(_RTT_MATFILE_LOG) && _RTT_MATFILE_LOG == 1)
    SdFile logFile[_RTT_NUMSDFILEREADBLOCKS_+1];
    #define MAT_FILE_ID _RTT_NUMSDFILEREADBLOCKS_
    #define FILELOG(id) logFile[id]
#else
// MAT file not defined and number of SD read blocks is equal to 1
#if (defined(_RTT_NUMSDFILEREADBLOCKS_) && (_RTT_NUMSDFILEREADBLOCKS_ == 1)) && (!defined(_RTT_MATFILE_LOG) || (defined(_RTT_MATFILE_LOG) && (_RTT_MATFILE_LOG==0)))
    SdFile logFile;
    #define FILELOG(id) logFile
// MAT file is defined and no SD read block is present
#elif !defined(_RTT_NUMSDFILEREADBLOCKS_) && (defined(_RTT_MATFILE_LOG) && (_RTT_MATFILE_LOG==1))
    SdFile logFile;
    #define FILELOG(id) logFile
#endif

#endif

SdFile testFile;
//Function defnitions
int *tCreate(const char *filename, const char *mode, uint8_t *FileID, uint8_t *alreadyOpen){
#if defined(_RTT_NUMSDFILEREADBLOCKS_) && (defined(_RTT_MATFILE_LOG) && _RTT_MATFILE_LOG == 1)
    *FileID = _RTT_NUMSDFILEREADBLOCKS_;
#endif

    uint8_t tFileID = *FileID;
    *alreadyOpen = 0;


    if(!strncmp(mode,"w+",2)){
        if(!FILELOG(tFileID).open(filename, O_CREAT | O_TRUNC | O_RDWR)){
            FILELOG(tFileID).close();
            return NULL;
        }
    }else if((mode == 'r') || !strncmp(mode,"r",1)){
        if(!FILELOG(tFileID).isOpen()){
            if(!FILELOG(tFileID).open(filename, O_RDONLY)){
                FILELOG(tFileID).close();
                return NULL;
            }
        }else{
            *alreadyOpen = 1;
        }
    }else{
        return NULL;
    }

    return &dummyFilePointer;
}

void* MW_SD_fopen(const char *filename, const char *mode, uint8_t *FileID, uint8_t *alreadyOpen){

    if(!initialized){
        #if defined(_RTT_SPI_CLOCK_)
            if(!sd.begin(_RTT_SD_CARD_SPI_CS_,SD_SCK_HZ(_RTT_SPI_CLOCK_))){
               return NULL;
            }
        #else
            if(!sd.begin(4,SD_SCK_MHZ(50))){
                return NULL;
            }
        #endif //defined(_RTT_SPI_CLOCK_)
        initialized = 1;
    }

    return (void *)tCreate(filename, mode, FileID, alreadyOpen);
}

size_t MW_SD_fwrite(const void *ptr, size_t size, size_t nmemb, void *stream, uint8_t FileID){
    int ret;
    ret = FILELOG(FileID).write((uint8_t *)ptr,(size*nmemb));
    if(ret != (size*nmemb)){
        ret = FILELOG(FileID).write((uint8_t *)ptr,(size*nmemb));
    }
    return ret;
}

int MW_SD_fread(void *ptr, size_t size, size_t nmemb, void *stream, uint8_t FileID){
    int ret;

    ret = FILELOG(FileID).read((uint8_t *)ptr,(size*nmemb));
    return ret;
}

int MW_SD_fread_delim(char *ptr, char delim, uint8_t FileID, uint8_t floatStatus){
    char ch;
    uint8_t nBytes=0U;
    int keepReading=1;
    uint8_t compare = 0U;

    while(keepReading==1){
        if(!FILELOG(FileID).available()){
            keepReading = -1;
            continue;
        }
        if(FILELOG(FileID).read((uint8_t *)&ch,1) != 1){
            keepReading = -2;
            continue;
        }


        // Ignore carriage return
        if(ch=='\r'){
            continue;
        }

        if(ch==delim || ch == '\n'){
            keepReading = 0;
        }else{
            compare = ch - 47U;
            if((compare < 0U) || (compare > 11U)){
                if((ch == 46U) && floatStatus){
                    keepReading = 1;
                }else if(((ch == 43U) || (ch == 45U)) && nBytes == 0){
                    keepReading = 1;
                }else{
                    keepReading = -3;
                    continue;
                }
            }
            if(ch == -1){
                keepReading = 1;
            }else{
                ptr[nBytes] = ch;
                nBytes++;
            }
        }
    }
    ptr[nBytes] = '\0';

    return keepReading;
}

void MW_SD_frewind(void *stream, uint8_t FileID){
    FILELOG(FileID).rewind();
}

int MW_SD_fclose(void *stream, uint8_t FileID){
    FILELOG(FileID).close();
    return 0;
}

int MW_SD_fremove(const char *filename){
    return 1;
}

int MW_SD_fseekset(uint8_t FileID, uint32_t pos){
    return FILELOG(FileID).seekSet(pos);
}

int MW_SD_fseekcur(uint8_t FileID, int pos){
    return FILELOG(FileID).seekCur(pos);
}

uint32_t MW_SD_fcurpos(uint8_t FileID){
    return FILELOG(FileID).curPosition();
}

uint32_t MW_SD_favailable(uint8_t FileID){
    return FILELOG(FileID).available();
}

uint32_t MW_SD_audio_favailable(uint8_t FileID, uint8_t *headerEnd){
    uint8_t header[151];
    uint8_t *ret;
    uint8_t i=0;

    FILELOG(FileID).seekSet(0);
    FILELOG(FileID).read(header,150);

    // Header might have 0 as a byte value in that case strstr() cannot be used
    // So add 1 to each character. And check for "ebub" instead of "data"
    for(i=0;i<150;i++)
        header[i] += 1;

    header[150] = '\0';

    // Set position to (address+8) where in, address is the place where
    // "data"(ebub) string in the header was found
    ret = (uint8_t *)strstr((char *)header,"ebub");
    if(((uint32_t)ret-(uint32_t)header) > 0){
        FILELOG(FileID).seekSet((uint32_t)ret-(uint32_t)header+8);
        *headerEnd = (uint32_t)ret-(uint32_t)header+8;
    }else{
        // Set Position to minimum header size, i.e., 44
        FILELOG(FileID).seekSet(44);
        *headerEnd = 44;
    }
    return FILELOG(FileID).available();
}

long MW_SD_strtol(char *ptr){
    long val;
    char *check;
    val = strtol(ptr,&check,10);
    return val;
}

unsigned long MW_SD_strtoul(char *ptr){
    unsigned long val;
    char *check;
    val = strtoul(ptr,&check,10);
    return val;
}

double MW_SD_strtod(char *ptr){
    double val;
    char *check;
    val = strtod(ptr,&check);
    return val;
}


#endif //Rapid Accel
