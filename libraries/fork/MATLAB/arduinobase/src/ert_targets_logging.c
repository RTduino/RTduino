/*
 *
 * Copyright 2019 The MathWorks, Inc.
 *
 * File: ert_targets_logging.c
 *
 * Abstract:
 *  Real-Time Workshop data logging routines using circular buffers of
 *      fixed size.  The buffers are allocated at start, filled in at each
 *      major time step and finally written to a MAT-file at the end of the
 *      simulation.
 *
 *      This file handles redefining the following standard MathWorks types
 *      (see tmwtypes.h):
 *         [u]int8_T     to be int32_T (logged as Matlab [u]int32)
 *         [u]int16_T    to be int32_T (logged as Matlab [u]int32)
 *         real_T        to be real32_T (logged as Matlab single)
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <Arduino.h>

#if !defined(MAT_FILE) || (defined(MAT_FILE) && MAT_FILE == 1)

#include <stddef.h>                     /* size_t */
#include "rt_logging.h"
#include "rt_mxclassid.h"
#include "rtw_matlogging.h"
#include "MW_target_hardware_resources.h"
#include "ert_targets_logging.h"

#ifndef TMW_NAME_LENGTH_MAX
#define TMW_NAME_LENGTH_MAX 64
#endif
#define mxMAXNAM  TMW_NAME_LENGTH_MAX   /* maximum name length */
#define matUNKNOWN                  0
#define matINT8                     1
#define matUINT8                    2
#define matINT16                    3
#define matUINT16                   4
#define matINT32                    5
#define matUINT32                   6
#define matFLOAT                    7
#define matDOUBLE                   9
#define matINT64                   12
#define matUINT64                  13
#define matMATRIX                  14

#define matLOGICAL_BIT          0x200
#define matCOMPLEX_BIT          0x800

#define matKEY                 0x4D49
#define matVERSION             0x0100
#define matVERSION_INFO_OFFSET   124L

#define matINT64_ALIGN(e)      ( ( ((unsigned)(e))+7 ) & (~7) )
#define matTAG_SIZE            (sizeof(int32_T) << 1)

#ifndef DEFAULT_BUFFER_SIZE
#define DEFAULT_BUFFER_SIZE      1024  /* used if maxRows=0 and Tfinal=0.0    */
#endif

#define FREE(m) if (m != NULL) free(m)

/* Logical definitions */
#if (!defined(__cplusplus))
#  ifndef false
#   define false                       (0U)
#  endif
#  ifndef true
#   define true                        (1U)
#  endif
#endif

#define NO_LOGGING_REALLOC 1
/*==========*
 * typedefs *
 *==========*/

typedef struct LogInfo_Tag {
    LogVar       *t;                   /* Time log variable                   */
    void         *x;                   /* State log variable                  */
    int_T        ny;                   /* Length of "y" log variables         */
    void         **y;                  /* Output log vars                     */
    void         *xFinal;              /* Final state log variable            */

    LogVar       *logVarsList;         /* Linked list of all LogVars          */
    StructLogVar *structLogVarsList;   /* Linked list of all StructLogVars    */

    boolean_T   haveLogVars;           /* Are logging one or more vars?       */
} LogInfo;

typedef struct MatItem_tag {
    int32_T    type;
    uint32_T    nbytes;
    const void *data;
} MatItem;

typedef enum {
    DATA_ITEM,
    MATRIX_ITEM,
    STRUCT_LOG_VAR_ITEM,
    SIGNALS_STRUCT_ITEM
} ItemDataKind;

FILE  *checkFP;

/*===========*
 * Constants *
 *===========*/

static const char_T rtMemAllocError[] PROGMEM = "Memory allocation error";

#define ZEROS32 "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"

#if mxMAXNAM==32

#define ZERO_PAD

#elif mxMAXNAM==64

#define ZERO_PAD ZEROS32

#elif mxMAXNAM==128

#define ZERO_PAD   ZEROS32  ZEROS32  ZEROS32

#else

#error "Cannot Handle mxMAXNAM other than 32,64, and 128"

#endif
/* field names: for variable-size signal logging */
static const char_T rtStructLogVarFieldNames[] PROGMEM =
        "time\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "signals\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "blockName\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD;

#define RT_STRUCT_LOG_NAME_SIZE (3*mxMAXNAM)

static const char_T rtLocalLoggingSignalsStructFieldNames[] PROGMEM =
        "values\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "valueDimensions\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "dimensions\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "label\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "title\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "plotStyle\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD;

#define RT_LOCAL_LOG_NAME_SIZE (6*mxMAXNAM)

static const char_T rtGlobalLoggingSignalsStructFieldNames[] PROGMEM =
        "values\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "valueDimensions\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "dimensions\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "label\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "blockName\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "stateName\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "inReferencedModel\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD;

#define RT_GLOBAL_LOG_NAME_SIZE (7*mxMAXNAM)

char progmemBuffer[mxMAXNAM];

#define TIME_FIELD_NAME      ((char*)memcpy_P(progmemBuffer,rtStructLogVarFieldNames[0*mxMAXNAM],mxMAXNAM))
#define SIGNALS_FIELD_NAME   ((char*)memcpy_P(progmemBuffer,rtStructLogVarFieldNames[1*mxMAXNAM],mxMAXNAM))
#define BLOCKNAME_FIELD_NAME ((char*)memcpy_P(progmemBuffer,rtStructLogVarFieldNames[2*mxMAXNAM],mxMAXNAM))

#define VALUES_FIELD_NAME    ((char*)memcpy_P(progmemBuffer,rtLocalLoggingSignalsStructFieldNames[0*mxMAXNAM],mxMAXNAM))
#define VALUEDIMENSIONS_FIELD_NAME ((char*)memcpy_P(progmemBuffer,rtLocalLoggingSignalsStructFieldNames[1*mxMAXNAM],mxMAXNAM))
#define DIMENSION_FIELD_NAME ((char*)memcpy_P(progmemBuffer,rtLocalLoggingSignalsStructFieldNames[2*mxMAXNAM],mxMAXNAM))
#define LABEL_FIELD_NAME     ((char*)memcpy_P(progmemBuffer,rtLocalLoggingSignalsStructFieldNames[3*mxMAXNAM],mxMAXNAM))
#define TITLE_FIELD_NAME     ((char*)memcpy_P(progmemBuffer,rtLocalLoggingSignalsStructFieldNames[4*mxMAXNAM],mxMAXNAM))
#define PLOTSTYLE_FIELD_NAME ((char*)memcpy_P(progmemBuffer,rtLocalLoggingSignalsStructFieldNames[5*mxMAXNAM],mxMAXNAM))

#define STATENAME_FIELD_NAME ((char*)memcpy_P(progmemBuffer,rtLocalLoggingSignalsStructFieldNames[5*mxMAXNAM],mxMAXNAM))
#define CROSS_MDL_REF_FIELD_NAME ((char*)memcpy_P(progmemBuffer,rtLocalLoggingSignalsStructFieldNames[6*mxMAXNAM],mxMAXNAM))

/* field names: for fixed-size signal logging */
static const char_T rtLocalLoggingSignalsStructFieldNames_noValDims[] PROGMEM =
        "values\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "dimensions\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "label\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "title\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "plotStyle\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD;

#define RT_LOCAL_LOG_NAME_NOVALDIM_SIZE (5*mxMAXNAM)

static const char_T rtGlobalLoggingSignalsStructFieldNames_noValDims[] PROGMEM =
        "values\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "dimensions\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "label\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "blockName\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "stateName\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD
        "inReferencedModel\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" ZERO_PAD;

#define RT_GLOBAL_LOG_NAME_NOVALDIM_SIZE (6*mxMAXNAM)


//char progmemBuffTemp[RT_GLOBAL_LOG_NAME_SIZE];


extern real_T rtInf; /* declared by rt_nonfinite.c */
extern real_T rtNaN;
extern real32_T rtNaNF;
/* bStopLogging is for stopping the logging indicator */
volatile uint8_T bStopLogging=0;
/* bDataReady is for PING-PONG buffer full indicator */
volatile uint8_T bDataReady =0;
volatile uint32_T dataSizeToWrite =0, wrappedSize=0;
/* stopRequested - Extern variable which denotes the simulation time is completed */
extern volatile boolean_T stopRequested;

/*================*
 * Local routines *
 *================*/

/* Function: rt_GetSizeofDataType ==============================================
 * Abstract:
 *      Get the element size in bytes given the data type id.
 */
static size_t rt_GetSizeofDataType(BuiltInDTypeId dTypeID)
{
    size_t elSz = 0; /* unknown */

    switch (dTypeID) {
        case SS_DOUBLE:
            elSz = sizeof(real_T);
            break;
        case SS_SINGLE:
            elSz = sizeof(real32_T);
            break;
        case SS_INT8:
            elSz = sizeof(int8_T);
            break;
        case SS_UINT8:
            elSz = sizeof(uint8_T);
            break;
        case SS_INT16:
            elSz = sizeof(int16_T);
            break;
        case SS_UINT16:
            elSz = sizeof(uint16_T);
            break;
        case SS_INT32:
            elSz = sizeof(int32_T);
            break;
        case SS_UINT32:
            elSz = sizeof(uint32_T);
            break;
        case SS_BOOLEAN:
            elSz = sizeof(boolean_T);
            break;
    }
    return(elSz);

} /* end rt_GetSizeofDataType */


/* Function: rt_GetSizeofComplexType ===========================================
 * Abstract:
 *      Get the element size in bytes given the data type id.
 */
static size_t rt_GetSizeofComplexType(BuiltInDTypeId dTypeID)
{
    size_t elSz = 2*rt_GetSizeofDataType(dTypeID);

    switch (dTypeID) {
        case SS_DOUBLE:
#ifdef CREAL_T
            elSz = sizeof(creal_T);
#endif
            break;
        case SS_SINGLE:
#ifdef CREAL_T
            elSz = sizeof(creal32_T);
#endif
            break;
        case SS_INT8:
#ifdef CINT8_T
            elSz = sizeof(cint8_T);
#endif
            break;
        case SS_UINT8:
#ifdef CUINT8_T
            elSz = sizeof(cuint8_T);
#endif
            break;
        case SS_INT16:
#ifdef CINT16_T
            elSz = sizeof(cint16_T);
#endif
            break;
        case SS_UINT16:
#ifdef CUINT16_T
            elSz = sizeof(cuint16_T);
#endif
            break;
        case SS_INT32:
#ifdef CINT32_T
            elSz = sizeof(cint32_T);
#endif
            break;
        case SS_UINT32:
#ifdef CUINT32_T
            elSz = sizeof(cuint32_T);
#endif
            break;
        case SS_BOOLEAN:
            elSz = sizeof(boolean_T);
            break;
    }

    return(elSz);

} /* end rt_GetSizeofComplexType */


/* Function: rt_GetDataTypeConvertInfo =========================================
 * Abstract:
 *      Directly copy if pointer to structure is non-NULL, otherwise set to
 *      default.
 */
static RTWLogDataTypeConvert rt_GetDataTypeConvertInfo(
        const RTWLogDataTypeConvert *pDataTypeConvertInfo,
        BuiltInDTypeId dTypeID
        )
{
    RTWLogDataTypeConvert dataTypeConvertInfoCopy;

    if (pDataTypeConvertInfo == NULL) {
        dataTypeConvertInfoCopy.conversionNeeded = 0;
        dataTypeConvertInfoCopy.dataTypeIdLoggingTo = dTypeID;
        dataTypeConvertInfoCopy.dataTypeIdOriginal  = (DTypeId)dTypeID;
        dataTypeConvertInfoCopy.bitsPerChunk = 0;
        dataTypeConvertInfoCopy.numOfChunk = 0;
        dataTypeConvertInfoCopy.isSigned = 0;
        dataTypeConvertInfoCopy.fracSlope = 1.0;
        dataTypeConvertInfoCopy.fixedExp = 0;
        dataTypeConvertInfoCopy.bias = 0.0;
    } else {
        dataTypeConvertInfoCopy = *pDataTypeConvertInfo;
    }

    return dataTypeConvertInfoCopy;

} /* end rt_GetDataTypeConvertInfo */


/* Function: rt_GetDblValueFromOverSizedData ===================================
 * Abstract:
 */
static double rt_GetDblValueFromOverSizedData(
        const void *pVoid,
        int bitsPerChunk,
        int numOfChunk,
        unsigned int isSigned,
        double fracSlope,
        int fixedExp,
        double bias)
{
    double retValue = 0;

    double *dblValue = (double *) calloc(numOfChunk, sizeof(double));

    int i;
    double isSignedNeg;

    if(isSigned) {
        const chunk_T *pData = (const chunk_T *) (pVoid);
        for (i = 0; i <numOfChunk; i++) {
            dblValue[i] = (double)(pData[i]);
        }
    } else  {
        const uchunk_T *pData = (const uchunk_T *) (pVoid);
        for (i = 0; i <numOfChunk; i++) {
            dblValue[i] = (double)(pData[i]);
        }
    }

    /*
     * Assuming multi chunks b_n ... b_2 b_1 b_0, and the length of each chunk is N.
     * Suppose b_i is the i-th chunk's value.
     * Then for unsigned data or data with one chunk: we have
     * retValue = b_n * 2^(n*N) + ... + b_1 * 2^N + b_0 * 2^0;
     * But for signed data, we have
     * retValue = b_n * 2^(n*N) + ... + b_1 * 2^N + b_0 * 2^0+ (b_0<0) * 2^N +
     * ... (b_(n-1) <0) * 2^(n*N)
     * = (b_n + (b_(n-1)<0)) * 2^(n*N) +... + (b_1 + (b_0<0)) * 2^N + b_0 * 2^0;
     * Together:
     * retValue =
     * (b_n + isSigned * (b_(n-1)<0)) * 2^(n*N) +... + (b_1 + isSigned * (b_0<0)) * 2^N + b_0 * 2^0;
     */

    retValue = dblValue[numOfChunk - 1];

    for(i = numOfChunk - 1; i > 0; i--) {
        isSignedNeg = dblValue[i - 1] < 0 ? (double)isSigned : 0;
        retValue = retValue + isSignedNeg;

        retValue = ldexp(retValue, bitsPerChunk)+ dblValue[i-1];
    }
    retValue = ldexp( fracSlope * retValue, fixedExp ) + bias;

    FREE(dblValue);
    return (retValue);

} /* end rt_GetDblValueFromOverSizedData */


/* Function: rt_GetNonBoolMxIdFromDTypeId ======================================
 * Abstract:
 *      Get the mx???_CLASS given the simulink builtin data type id.
 */
static mxClassID rt_GetNonBoolMxIdFromDTypeId(BuiltInDTypeId dTypeID)
{
    mxClassID mxID;

    switch (dTypeID) {
        case SS_DOUBLE:
            mxID = (sizeof(real_T)==4? mxSINGLE_CLASS: mxDOUBLE_CLASS);
            break;
        case SS_SINGLE:
            mxID = mxSINGLE_CLASS;
            break;
        case SS_INT8:
            switch (sizeof(int8_T)) {
                case 4: /* PWS - int8_T mapped to 32-bits */
                    mxID = mxINT32_CLASS;
                    break;
                case 2: /* PWS - int8_T mapped to 16-bits */
                    mxID = mxINT16_CLASS;
                    break;
                case 1:
                    mxID = mxINT8_CLASS;
                    break;
                default:
                    mxID = mxUNKNOWN_CLASS;
                    break;
            }
            break;
        case SS_UINT8:
            switch (sizeof(uint8_T)) {
                case 4: /* PWS - uint8_T mapped to 32-bits */
                    mxID = mxUINT32_CLASS;
                    break;
                case 2: /* PWS - uint8_T mapped to 16-bits */
                    mxID = mxUINT16_CLASS;
                    break;
                case 1:
                    mxID = mxUINT8_CLASS;
                    break;
                default:
                    mxID = mxUNKNOWN_CLASS;
                    break;
            }
            break;
        case SS_INT16:
            mxID = (sizeof(int16_T)==4? mxINT32_CLASS: mxINT16_CLASS);
            break;
        case SS_UINT16:
            mxID = (sizeof(uint16_T)==4? mxUINT32_CLASS: mxUINT16_CLASS);
            break;
        case SS_INT32:
            mxID = mxINT32_CLASS;
            break;
        case SS_UINT32:
            mxID = mxUINT32_CLASS;
            break;
            /*case SS_BOOLEAN:
             * mxID = (sizeof(boolean_T)==4? mxUINT32_CLASS: mxLOGICAL_CLASS);
             * break;*/
        default:
            mxID = mxUNKNOWN_CLASS;
            break;
    }

    return(mxID);

} /* end rt_GetNonBoolMxIdFromDTypeId */



#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_GetMxIdFromDTypeIdForRSim ======================================
     * Abstract:
     *      Get the mx???_CLASS given the simulink builtin data type id.
     */
    mxClassID rt_GetMxIdFromDTypeIdForRSim(BuiltInDTypeId dTypeID)
    {
        mxClassID mxID;

        if (dTypeID == SS_BOOLEAN) {
            switch (sizeof(boolean_T)) {
                case 4:
                    mxID = mxUINT32_CLASS;
                    break;
                case 2:
                    mxID = mxUINT16_CLASS;
                    break;
                default:
                    mxID = mxLOGICAL_CLASS;
                    break;
            }
        } else {
            mxID = rt_GetNonBoolMxIdFromDTypeId(dTypeID);
        }

        return(mxID);

    } /* end rt_GetMxIdFromDTypeIdForRSim */


#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_GetMxIdFromDTypeId =============================================
     * Abstract:
     *      Get the mx???_CLASS given the simulink builtin data type id.
     */
    mxClassID rt_GetMxIdFromDTypeId(BuiltInDTypeId dTypeID)
    {
        mxClassID mxID;

        if (dTypeID == SS_BOOLEAN) {
            switch (sizeof(boolean_T)) {
                case 4:
                    mxID = mxUINT32_CLASS;
                    break;
                case 2:
                    mxID = mxUINT16_CLASS;
                    break;
                default:
                    mxID = mxUINT8_CLASS;
                    break;
            }
        } else {
            mxID = rt_GetNonBoolMxIdFromDTypeId(dTypeID);
        }
        return(mxID);

    } /* end rt_GetMxIdFromDTypeId */


#ifdef __cplusplus
}
#endif



/* Function: rt_GetMatIdFromMxId ===============================================
 * Abstract:
 *      Get the MatId given the mxClassID.
 */
static int_T rt_GetMatIdFromMxId(mxClassID mxID)
{
    int_T matID;

    switch (mxID) {
        case mxCELL_CLASS:
        case mxSTRUCT_CLASS:
        case mxOBJECT_CLASS:
            matID = -1;
            break;
        case mxCHAR_CLASS:
            matID = matUINT16;
            break;
        case mxDOUBLE_CLASS:
            matID = matDOUBLE;
            break;
        case mxSINGLE_CLASS:
            matID = matFLOAT;
            break;
        case mxINT8_CLASS:
            matID = matINT8;
            break;
        case mxUINT8_CLASS:
            matID = matUINT8;
            break;
        case mxINT16_CLASS:
            matID = matINT16;
            break;
        case mxUINT16_CLASS:
            matID = matUINT16;
            break;
        case mxINT32_CLASS:
            matID = matINT32;
            break;
        case mxUINT32_CLASS:
            matID = matUINT32;
            break;
        case mxINT64_CLASS:
            matID = matINT64;
            break;
        case mxUINT64_CLASS:
            matID = matUINT64;
            break;
        default:
            matID = matUNKNOWN;
            break;
    }
    return(matID);

} /* end rt_GetMatIdFromMxId */


/* Forward declaration */
static int_T rt_WriteItemToMatFile(FILE          *fp,
        MatItem      *pItem,
        ItemDataKind dataKind);


/* Function: rt_ProcessMatItem =================================================
 * Abstract:
 *      This routine along with rt_WriteItemToMatFile() write out a specified
 *      mat-item the .mat file. Note that if the input argument
 *          cmd == 0, then this function just calculates the size of the item.
 *          cmd <> 0, this function writes the mat-item to the file.
 *      Return values is
 *           -1 : coding/logic error
 *            0 : upon success
 *          > 0 : upon write failure (1)
 */
static int_T rt_ProcessMatItem(FILE         *fp,
        MatItem      *pItem,
        ItemDataKind itemKind,
        int_T        cmd)
{
    mxClassID    mxID          = mxUNKNOWN_CLASS;
    uint32_T     arrayFlags[2] = {0, 0};
    int32_T      *dims         = NULL;
    int32_T      _dims[3]      = {0, 0, 0};
    int32_T      nDims         = 2;
    int32_T      nBytesInItem  = 0;
    const char_T *itemName;
    MatItem      item;
    int_T        retStat       = 0;

    char *progmemBuffTemp;
    progmemBuffTemp = (char *)malloc(RT_STRUCT_LOG_NAME_SIZE);

    switch (itemKind) {
        case DATA_ITEM: {
            /*(void)fprintf(stderr,"Unexpected itemKind = DATA_ITEM in "
                    "rt_ProcessMatItem @A\n");*/
            retStat = -1;
            goto EXIT_POINT;
        }
        case MATRIX_ITEM: {
            const MatrixData *var = (const MatrixData *) pItem->data;

            mxID           = var->mxID;
            arrayFlags[0]  = mxID;
            arrayFlags[0] |= var->logical;
            arrayFlags[0] |= var->complex;
            if (var->nDims < 2) {
                dims         = _dims;
                dims[0]      = var->nRows;
                dims[1]      = var->nCols;
                nDims        = 2;
            } else if (var->nDims >= 2) {
                int32_T k;
                dims = (int32_T*)malloc(sizeof(int32_T)*(var->nDims+1));
                for (k = 0; k < var->nDims; k++) {
                    dims[k] = var->dims[k];
                }
                dims[var->nDims] = var->nRows;
                nDims = var->nDims + 1;
            }
            itemName = var->name;
            break;
        }
        case STRUCT_LOG_VAR_ITEM: {
            const StructLogVar *var = (const StructLogVar *) pItem->data;

            mxID          = mxSTRUCT_CLASS;
            arrayFlags[0] = mxID;
            dims          = _dims;
            dims[0]       = 1;
            dims[1]       = 1;
            itemName      = var->name;
            break;
        }
        case SIGNALS_STRUCT_ITEM: {
            const SignalsStruct *var = (const SignalsStruct *) pItem->data;

            mxID          = mxSTRUCT_CLASS;
            arrayFlags[0] = mxID;
            dims          = _dims;
            dims[0]       = 1;
            dims[1]       = var->numSignals;
            itemName      = SIGNALS_FIELD_NAME;
            break;
        }
        default:
            /*(void)fprintf(stderr,"Unexpected itemKind=%d in rt_ProcessMatItem @B\n",
                    itemKind);*/
            retStat = -1;
            goto EXIT_POINT;
    }

    /* array flags */
    item.nbytes = 2*sizeof(uint32_T);
    if (cmd) {
        item.type = matUINT32;
        item.data = arrayFlags;
        if (rt_WriteItemToMatFile(fp,&item, DATA_ITEM)) {
            retStat = 1;
            goto EXIT_POINT;
        }
    } else {
        /*LINTED E_CAST_INT_TO_SMALL_INT*/
        nBytesInItem += matINT64_ALIGN(matTAG_SIZE + item.nbytes);
    }
    /* dimensions */
    /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
    item.nbytes = nDims*sizeof(int32_T);
    if (cmd) {
        item.type = matINT32;
        item.data = dims;
        if (rt_WriteItemToMatFile(fp,&item, DATA_ITEM)) {
            retStat = 1;
            goto EXIT_POINT;
        }
    } else {
        /*LINTED E_CAST_INT_TO_SMALL_INT*/
        nBytesInItem += matINT64_ALIGN(matTAG_SIZE + item.nbytes);
    }
    /* name */
    item.nbytes = (int32_T)strlen(itemName);
    if (cmd) {
        item.type = matINT8;
        item.data = (const char_T*) itemName;
        if (rt_WriteItemToMatFile(fp,&item, DATA_ITEM)) {
            retStat = 1;
            goto EXIT_POINT;
        }
    } else {
        nBytesInItem += (item.nbytes <= 4) ? /*LINTED E_CAST_INT_TO_SMALL_INT*/
            matTAG_SIZE : matINT64_ALIGN(matTAG_SIZE + item.nbytes);
    }

    if (itemKind == MATRIX_ITEM) {
        const MatrixData *var   = (const MatrixData*) pItem->data;
        int_T            matID  = rt_GetMatIdFromMxId(mxID);
        size_t           elSize = var->elSize;

        /* data */
        item.nbytes = (int32_T)(var->nRows * var->nCols * elSize);
        if (cmd) {
            item.type = matID;
            item.data = var->re;
            if (rt_WriteItemToMatFile(fp, &item, DATA_ITEM)) {
                retStat = 1;
                goto EXIT_POINT;
            }
        } else {
            nBytesInItem += (item.nbytes <= 4) ? /*LINTED*/
                matTAG_SIZE : matINT64_ALIGN(matTAG_SIZE + item.nbytes);
        }
        /* imaginary part */
        if (var->complex) {
            item.nbytes = (int32_T)(var->nRows * var->nCols * elSize);
            if (cmd) {
                item.type = matID;
                item.data = var->im;
                if (rt_WriteItemToMatFile(fp, &item, DATA_ITEM)) {
                    retStat = 1;
                    goto EXIT_POINT;
                }
            } else {
                nBytesInItem += (item.nbytes <= 4) ? /*LINTED*/
                    matTAG_SIZE : matINT64_ALIGN(matTAG_SIZE + item.nbytes);
            }
        }
    } else {  /* some type of structure item */
        const char_T *fieldNames;
        int_T        sizeofFieldNames;

        /* field names */
        switch (itemKind) {
            case STRUCT_LOG_VAR_ITEM: {
                const StructLogVar *var = (const StructLogVar *) pItem->data;
                fieldNames        = (char *)memcpy_P((void *)progmemBuffTemp,(void *)rtStructLogVarFieldNames,RT_STRUCT_LOG_NAME_SIZE);
                sizeofFieldNames  = var->numActiveFields * mxMAXNAM;
                break;
            }
            case SIGNALS_STRUCT_ITEM: {
                const SignalsStruct *var = (const SignalsStruct *) pItem->data;
                fieldNames        = var->fieldNames;
                sizeofFieldNames  = var->numActiveFields * mxMAXNAM;
                break;
            }
            default:
                /*(void)fprintf(stderr, "Unexpected itemKind=%d in "
                        "rt_ProcessMatItem @C\n", itemKind);*/
                retStat = -1;
                goto EXIT_POINT;
        }

        /* write field names */
        if (cmd) {
            int32_T tmpInt = mxMAXNAM;

            item.nbytes = sizeof(int32_T);
            item.type   = matINT32;
            item.data   = &tmpInt;
            if (rt_WriteItemToMatFile(fp,&item, DATA_ITEM)) {
                retStat = 1;
                goto EXIT_POINT;
            }

            item.nbytes = sizeofFieldNames;
            item.type   = matINT8;
            item.data   = (const char_T*) fieldNames;
            if (rt_WriteItemToMatFile(fp,&item, DATA_ITEM)) {
                retStat = 1;
                goto EXIT_POINT;
            }
        } else {
            /*LINTED E_CAST_INT_TO_SMALL_INT*/
            nBytesInItem += matINT64_ALIGN( matTAG_SIZE + matTAG_SIZE +
                    sizeofFieldNames );
        }

        /* process each field of the structure */
        switch (itemKind) {
            case STRUCT_LOG_VAR_ITEM: {
                const StructLogVar *var = pItem->data;

                /* time */
                {
                    const void *data = var->time;
                    LogVar* dataptr;
                    /* copy with offset */
                    MatrixData  tempData;

                    if (var->logTime) { /* time is a LogVar, get the MatrixData */
                        data = &(((const LogVar*) (var->time))->data);
                        dataptr = ((LogVar*) (var->time));

                        (void)memcpy(tempData.name, dataptr->data.name, mxMAXNAM);
                        tempData.nRows = dataSizeToWrite; //dataptr->valDims->nRows;
                        tempData.nCols = dataptr->data.nCols;
                        tempData.nDims = dataptr->data.nDims;
                        tempData.dims = dataptr->data.dims;
                        //tempData._dims = dataptr->data._dims;
                        tempData._dims[0] = dataptr->data._dims[0];
                        tempData._dims[1] = dataptr->data._dims[1];
                        if(wrappedSize < dataptr->data.nRows/2)
                        {
                            tempData.re = dataptr->data.re;
                        }
                        else
                        {
                            tempData.re = dataptr->data.re + ((dataptr->data.elSize * (dataptr->data.nRows/2) * dataptr->data.nCols) );
                        }
                        //item.nbytes = (int32_T)(dataSizeToWrite * var->nCols * elSize);
                        tempData.im = dataptr->data.im;
                        tempData.dTypeID = dataptr->data.dTypeID;
                        tempData.elSize =  dataptr->data.elSize;
                        tempData.mxID = dataptr->data.mxID;
                        tempData.logical = dataptr->data.logical;
                        tempData.complex = dataptr->data.complex;
                        tempData.frameData = dataptr->data.frameData;
                        tempData.frameSize = dataptr->data.frameSize;

                        item.data = &tempData; /*dataptr->valDims;*/
                    }
                    else
                    {
                        item.data = data;
                    }

                    item.type = matMATRIX;
                    if (cmd) {
                        if (rt_WriteItemToMatFile(fp,&item,MATRIX_ITEM)){
                            retStat = 1;
                            goto EXIT_POINT;
                        }
                    } else {
                        if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM,0)){
                            retStat = 1;
                            goto EXIT_POINT;
                        }
                        nBytesInItem += item.nbytes + matTAG_SIZE;
                    }
                }

                /* signals */
                item.type = matMATRIX;
                item.data = &(var->signals);
                if (cmd) {
                    if (rt_WriteItemToMatFile(fp,&item,SIGNALS_STRUCT_ITEM)) {
                        retStat = 1;
                        goto EXIT_POINT;
                    }
                } else {
                    if (rt_ProcessMatItem(fp, &item, SIGNALS_STRUCT_ITEM,0)) {
                        retStat = 1;
                        goto EXIT_POINT;
                    }
                    nBytesInItem += item.nbytes + matTAG_SIZE;
                }

                /* block name */
                if (var->blockName != NULL) {
                    item.type = matMATRIX;
                    item.data = var->blockName;
                    if (cmd) {
                        if (rt_WriteItemToMatFile(fp, &item, MATRIX_ITEM)) {
                            retStat = 1;
                            goto EXIT_POINT;
                        }
                    } else {
                        if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM, 0)) {
                            retStat = 1;
                            goto EXIT_POINT;
                        }
                        nBytesInItem += item.nbytes + matTAG_SIZE;
                    }
                }
                break;
            }
            case SIGNALS_STRUCT_ITEM: {
                const SignalsStruct *var        = pItem->data;
                const LogVar        *values     = var->values;
                const MatrixData    *dimensions = var->dimensions;
                const MatrixData    *labels     = var->labels;
                const MatrixData    *plotStyles = var->plotStyles;
                const MatrixData    *titles     = var->titles;
                const MatrixData    *blockNames = var->blockNames;
                const MatrixData    *stateNames = var->stateNames;
                const MatrixData    *crossMdlRef = var->crossMdlRef;
                const boolean_T logValueDimensions = var->logValueDimensions;
                int_T               i;

                for (i = 0; i < var->numSignals; i++) {
                    /* values */

                    /* copy with offset */
                    MatrixData  tempData;
                    (void)memcpy(tempData.name, values->data.name, mxMAXNAM);
                    tempData.nRows = dataSizeToWrite; //values->valDims->nRows;
                    tempData.nCols = values->data.nCols;
                    tempData.nDims = values->data.nDims;
                    tempData.dims = values->data.dims;
                    tempData._dims[0] = values->data._dims[0];
                    tempData._dims[1] = values->data._dims[1];
                    if(wrappedSize < values->data.nRows/2)
                    {
                        tempData.re = values->data.re;
                    }
                    else
                    {
                        tempData.re = values->data.re + ((values->data.elSize * (values->data.nRows/2) * values->data.nCols) );
                    }
                    tempData.im = values->data.im;
                    tempData.dTypeID = values->data.dTypeID;
                    tempData.elSize =  values->data.elSize;
                    tempData.mxID = values->data.mxID;
                    tempData.logical = values->data.logical;
                    tempData.complex = values->data.complex;
                    tempData.frameData = values->data.frameData;
                    tempData.frameSize = values->data.frameSize;

                    item.type = matMATRIX;
                    item.data = &tempData; /*values->valDims;*/
                    if (cmd) {
                        if (rt_WriteItemToMatFile(fp, &item,MATRIX_ITEM)) {
                            retStat = 1;
                            goto EXIT_POINT;
                        }
                    } else {
                        if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM, 0)) {
                            retStat = 1;
                            goto EXIT_POINT;
                        }
                        nBytesInItem += item.nbytes + matTAG_SIZE;
                    }

                    if(logValueDimensions)
                    {
                        /* valueDimensions */
                        /* Since the functions rt_WriteItemToMatFile and
                         * rt_ProcessMatItem deal with MatrixData,
                         * convert valDims to tempData, and fill up the
                         * necessary fields.
                         */
                        MatrixData  tempData;
                        (void)memcpy(tempData.name, VALUEDIMENSIONS_FIELD_NAME, mxMAXNAM);
                        tempData.nRows = values->valDims->nRows;
                        tempData.nCols = values->valDims->nCols;
                        tempData.nDims = 1;
                        tempData._dims[0] = values->valDims->nCols;
                        tempData.re = values->valDims->dimsData;
                        tempData.im = NULL;
                        tempData.dTypeID = SS_DOUBLE;
                        tempData.elSize =  sizeof(real_T);
                        tempData.mxID = mxDOUBLE_CLASS;
                        tempData.logical = 0;
                        tempData.complex = 0;
                        tempData.frameData = 0;
                        tempData.frameSize = 1;

                        item.type = matMATRIX;
                        item.data = &tempData; /*values->valDims;*/

                        if (cmd) {
                            if (rt_WriteItemToMatFile(fp, &item,MATRIX_ITEM)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                        } else {
                            if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM, 0)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                            nBytesInItem += item.nbytes + matTAG_SIZE;
                        }
                    }
                    values = values->next;

                    /* dimensions */
                    if (dimensions != NULL) {
                        item.type = matMATRIX;
                        item.data = &(dimensions[i]);
                        if (cmd) {
                            if (rt_WriteItemToMatFile(fp,&item, MATRIX_ITEM)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                        } else {
                            if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM, 0)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                            nBytesInItem += item.nbytes + matTAG_SIZE;
                        }
                    }

                    /* label */
                    item.type = matMATRIX;
                    item.data = &(labels[i]);
                    if (cmd) {
                        if (rt_WriteItemToMatFile(fp, &item,MATRIX_ITEM)) {
                            retStat = 1;
                            goto EXIT_POINT;
                        }
                    } else {
                        if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM, 0)) {
                            retStat = 1;
                            goto EXIT_POINT;
                        }
                        nBytesInItem += item.nbytes + matTAG_SIZE;
                    }
                    /* title */
                    if (titles != NULL) {
                        item.type = matMATRIX;
                        item.data = &(titles[i]);
                        if (cmd) {
                            if (rt_WriteItemToMatFile(fp, &item, MATRIX_ITEM)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                        } else {
                            if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM, 0)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                            nBytesInItem += item.nbytes + matTAG_SIZE;
                        }
                    }
                    /* plot style */
                    if (plotStyles != NULL) {
                        item.type = matMATRIX;
                        item.data = &(plotStyles[i]);
                        if (cmd) {
                            if (rt_WriteItemToMatFile(fp,&item, MATRIX_ITEM)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                        } else {
                            if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM, 0)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                            nBytesInItem += item.nbytes + matTAG_SIZE;
                        }
                    }
                    /* block name */
                    if (blockNames != NULL) {
                        item.type = matMATRIX;
                        item.data = &(blockNames[i]);
                        if (cmd) {
                            if (rt_WriteItemToMatFile(fp, &item, MATRIX_ITEM)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                        } else {
                            if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM, 0)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                            nBytesInItem += item.nbytes + matTAG_SIZE;
                        }
                    }
                    /* state name */
                    if (stateNames != NULL) {
                        item.type = matMATRIX;
                        item.data = &(stateNames[i]);
                        if (cmd) {
                            if (rt_WriteItemToMatFile(fp, &item, MATRIX_ITEM)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                        } else {
                            if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM, 0)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                            nBytesInItem += item.nbytes + matTAG_SIZE;
                        }
                    }
                    /* crossMdlRef */
                    if (crossMdlRef != NULL) {
                        item.type = matMATRIX;
                        item.data = &(crossMdlRef[i]);
                        if (cmd) {
                            if (rt_WriteItemToMatFile(fp, &item, MATRIX_ITEM)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                        } else {
                            if (rt_ProcessMatItem(fp, &item, MATRIX_ITEM, 0)) {
                                retStat = 1;
                                goto EXIT_POINT;
                            }
                            nBytesInItem += item.nbytes + matTAG_SIZE;
                        }
                    }
                } /* for i=1:numSignals */
                break;
            }
            default:
                /*(void)fprintf(stderr, "Unexpected itemKind=%d in "
                        "rt_ProcessMatItem @D\n", itemKind);*/
                retStat = -1;
                goto EXIT_POINT;
        }
    } /* end struct item */

    if (!cmd) {
        pItem->nbytes = nBytesInItem;
    }

    EXIT_POINT:
        if (dims != _dims) {
            FREE(dims);
        }
        FREE(progmemBuffTemp);
        return(retStat);

} /* end rt_ProcessMatItem */


/* Function: rt_WriteItemToMatFile =============================================
 * Abstract:
 *      Entry function for writing out a mat item to the mat file.
 *
 *      Return values is
 *          == 0 : upon success
 *          <> 0 : upon failure
 */
static int_T rt_WriteItemToMatFile(FILE         *fp,
        MatItem      *pItem,
        ItemDataKind itemKind)
{
    /* Determine the item size */
    if (pItem->type == matMATRIX) {
        if (rt_ProcessMatItem(fp, pItem, itemKind, 0)) return(1);
    }

    /* Write the item tag and data */
    if (pItem->nbytes > 4) {
        int32_T nAlignBytes;
        if (MW_fwrite(pItem, 1, matTAG_SIZE, fp) != matTAG_SIZE)
        {
            bStopLogging = 1;
            return(1);
        }

        if (pItem->type == matMATRIX) {
            if (rt_ProcessMatItem(fp, pItem, itemKind, 1))
            {
                bStopLogging =1;
                return(1);
            }
        } else {
            if ( MW_fwrite(pItem->data, 1, pItem->nbytes, fp) !=
                    ((size_t) pItem->nbytes) ) {
                bStopLogging = 1;
                return(1);
            }
        }

        /* Add offset for 8-byte alignment */
        nAlignBytes = matINT64_ALIGN(pItem->nbytes) - pItem->nbytes;
        if (nAlignBytes > 0) {
            int pad[2] = {0, 0};
            if ( MW_fwrite(pad,1,nAlignBytes,fp) != ((size_t) nAlignBytes) ) {
                bStopLogging = 1;
                return(1);
            }
        }
    } else {
        MatItem item = {0, 0, NULL};
        item.type = ((uint32_T)(pItem->type))|(((uint32_T)(pItem->nbytes))<<16);
        (void)memcpy(&item.nbytes, pItem->data, pItem->nbytes);
        if (MW_fwrite(&item, 1, matTAG_SIZE, fp) != matTAG_SIZE)
        {
            bStopLogging = 1;
            return(1);
        }
    }

    return(0);

} /* end rt_WriteItemToMatFile */


/* Function: rt_WriteMat5FileHeader ============================================
 * Abstract:
 *      Function to write the mat file header.
 *      Return values is
 *          == 0 : upon success
 *          <> 0 : upon failure
 */
static int_T rt_WriteMat5FileHeader(FILE *fp)
{
    int_T        nbytes;
    int_T        nspaces;
    int_T        i, n;
    unsigned short ver[2];
    char_T       spaces[16];
    const char_T *matversion = "MATLAB 5.0 MAT-file";

    (void)memset(spaces, ' ', sizeof(spaces));

    n = (int_T)strlen(matversion);
    nbytes = (int_T)MW_fwrite(matversion, 1, n, fp);
    /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
    nspaces = matVERSION_INFO_OFFSET - nbytes;
    /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
    n = nspaces % sizeof(spaces);
    nbytes += (int_T)MW_fwrite(spaces, 1, n, fp);
    /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
    n = nspaces / sizeof(spaces);
    for (i = 0; i < n; ++i) {
        nbytes += (int_T)MW_fwrite(spaces, 1, sizeof(spaces), fp);
    }
    if (nbytes == matVERSION_INFO_OFFSET) {
        ver[0] = matVERSION;
        ver[1] = matKEY;
        nbytes += (int_T)MW_fwrite(ver, 1, sizeof(ver), fp);
    }
    return(nbytes != matVERSION_INFO_OFFSET + sizeof(ver));

} /* end rt_WriteMat5FileHeader */


/* Function: rt_FixupLogVar ====================================================
 * Abstract:
 *  Make the logged variable suitable for MATLAB.
 */
static const char_T *rt_FixupLogVar(LogVar *var,int verbose)
{
    int_T  nCols   = var->data.nCols;
    int_T  maxRows = var->data.nRows;
    int_T  nDims   = var->data.nDims;
    size_t elSize  = var->data.elSize;
    int_T  nRows;

    if ((var->wrapped == -1) || (var->wrapped == -2) || (var->wrapped == (maxRows + 1))) {
        nRows = 0;
    }
    else if (var->wrapped >= (maxRows/2)) {
        nRows = ((var->wrapped)-(maxRows/2) + 1);
    }
    else{
        nRows = (var->wrapped + 1);
    }

    var->nDataPoints = nRows;

    if (nDims < 2 && nCols > 1) {  /* Transpose? */
        /* Don't need to transpose valueDimensions */
        int_T  nEl    = nRows*nCols;
        char   *src   = var->data.re;
        char   *pmT;
        int_T  k;
        if(var->wrapped < (var->data.nRows/2))
        {
            src   = var->data.re;
        }
        else
        {
            src = ((char *)(var->data.re) + ((var->data.elSize * (var->data.nRows/2) * var->data.nCols) ));
        }

        /**********************************
         * If memory cannot be allocated, *
         * write to a temporary buffer    *
         **********************************/
        if ((pmT = malloc(nEl*elSize)) == NULL) {
            FILE  *fptr;
            char  fName[mxMAXNAM];

            (void)sprintf(fName, "%s%s", var->data.name, "_rtw_tmw.tmw");
            if ((fptr=MW_fopen(fName,"w+b")) == NULL) {
                bStopLogging = 1;
                return("unable to open data file\n");
            }

            /****************************
             * Write the data to a file *
             ****************************/
            for (k=0; k<nEl; k++) {
                int_T kT   = nCols*(k%nRows) + (k/nRows);
                char  *dst = src + kT*elSize;
                (void)MW_fwrite(dst, elSize, 1, fptr);
            }
            if (var->data.complex) {
                char *pmiT = var->data.re;
                src  = var->data.im;
                for (k=0; k<nEl; k++) {
                    int_T kT   = nRows*(k%nCols) + (k/nCols);
                    char  *dst = pmiT + kT*elSize;
                    (void)memcpy(dst, src, elSize);
                    src += elSize;
                }
                var->data.re = var->data.im;
                var->data.im = pmiT;
            }

            /*******************************
             * Read the data from the file *
             *******************************/
            (void)MW_frewind(fptr);
            (void)MW_fread(var->data.re, elSize, nEl, fptr);
            (void)MW_fclose(fptr);
            (void)MW_fremove(fName);
        } else {
            char   *origsrc = src;
            for (k=0; k<nEl; k++) {
                int_T kT   = nRows*(k%nCols) + (k/nCols);
                char  *dst = pmT + kT*elSize;
                (void)memcpy(dst, src, elSize);
                src += elSize;
            }
            if (var->data.complex) {
                char *pmiT = var->data.re;
                src  = var->data.im;
                for (k=0; k<nEl; k++) {
                    int_T kT   = nRows*(k%nCols) + (k/nCols);
                    char  *dst = pmiT + kT*elSize;
                    (void)memcpy(dst, src, elSize);
                    src += elSize;
                }
                var->data.re = var->data.im;
                var->data.im = pmiT;
            }
            memcpy(origsrc,pmT,(nEl*elSize));
            //FREE(var->data.re);
            FREE(pmT);
            //var->data.re = pmT;
        }
    } /* Transpose? */

#if 0
    if (var->wrapped > 0 && var->rowIdx != 0 ) {  /* Rotate? */
        char_T *buffer = var->data.re;
        int_T  done       = 0; /* done: 0 (1) rotate real (imag) part. */

        do {
            char_T *col       = buffer;
            int_T  rowOffset  = (int_T)((nDims == 1) ? (elSize) : (elSize * nCols));
            int_T  colOffset  = (int_T)((nDims == 1)?  (nRows*elSize) : elSize);
            int_T  zeroIdx    = var->rowIdx;
            int_T  j;

            for (j = 0 ; j < nCols; ++j, col += colOffset) {
                int_T   swapCount;
                int_T   srcIdx;
                int_T   dstIdx;
                int_T   tmpIdx;
                MatReal tmp;

                for (tmpIdx=0, swapCount=0; swapCount < nRows; tmpIdx++) {
                    (void)memcpy(&tmp, col + tmpIdx*rowOffset, elSize);

                    dstIdx=tmpIdx;
                    srcIdx = ((dstIdx + zeroIdx) % nRows);
                    while (srcIdx != tmpIdx) {
                        (void)memcpy(col + dstIdx*rowOffset,
                                col + srcIdx*rowOffset,
                                elSize);
                        ++swapCount;
                        dstIdx = srcIdx;
                        srcIdx = ((dstIdx + zeroIdx) % nRows);

                    }
                    (void)memcpy(col + dstIdx*rowOffset, &tmp, elSize);
                    ++swapCount;
                }
            }
            done ++;
            /* need to rotate the imaginary part */
        } while ((done == 1) && ((buffer = var->data.im) != NULL));

        var->rowIdx = 0;
    } /* Rotate? */
#endif
    /*
     * We might have allocated more number of rows than the number of data
     * points that have been logged, in which case set nRows to nDataPoints
     * so that only these values get saved.
     */
#if 0
    if (var->nDataPoints < var->data.nRows) {
        var->data.nRows = var->nDataPoints;
        if(var->valDims != NULL){
            size_t elSizeValDims = sizeof(real_T);
            int_T  k;
            real_T *dimsData = var->valDims->dimsData + nRows;
            /*
             * Keep nRows of values and that of valueDimensions consistent
             * for variable-size signals.
             */
            var->valDims->nRows = var->data.nRows;
            /*
             * Also need to move data when shrinking the array size,
             * because valueDimensions data is stored in array format.
             * e.g. maxRows = 4; nRows = 2; nDims = 3;
             * Before fixing up the logVar, the locations of data are as below:
             * (x, y, z -- useful data / o -- junk)
             * a[0] = x    a[4] = y    a[8] = z
             * a[1] = x    a[5] = y    a[9] = z
             * a[2] = o    a[6] = o    a[10]= o
             * a[3] = o    a[7] = o    a[11]= o
             * After fixing up the logVar, we want the data to be stored as:
             * a[0] = x    a[4] = z    a[8] = o
             * a[1] = x    a[5] = z    a[9] = o
             * a[2] = y    a[6] = o    a[10]= o
             * a[3] = y    a[7] = o    a[11]= o
             */
            for(k = 1; k < nDims; k++){
                (void) memmove(dimsData,
                        var->valDims->dimsData + k*maxRows,
                        elSizeValDims * nRows);
                dimsData += nRows;
            }
        }
    }
#endif
    return(NULL);

} /* end rt_FixupLogVar */


/* Function: rt_LoadModifiedLogVarName =========================================
 * Abstract:
 *      The name of the logged variable is obtained from the input argument
 *      varName and the nameModifier which is obtained from the simstruct. If
 *      the nameModifier begins with an '_', then nameModifier is post-pended to
 *      varName to obtain the name of the logged variable. If the first
 *      character does not begin with an '_', then the nameModifier is
 *      pre-pended to varName.
 *
 * Examples:
 *     a)  varName = "tout" & nameModifier = "_rt"  => logVarName = "tout_rt"
 *     b)  varName = "tout" & nameModifier = "rt_"  => logVarName = "rt_tout"
 *     c)  varName = "tout" & nameModifier = "none" => logVarName = "tout"
 */
static void rt_LoadModifiedLogVarName(const RTWLogInfo *li,         /* in  */
        const char       *varName,    /* in  */
        char             *logVarName) /* out */
{
    int_T        nameLen;
    const char_T *nameModifier = rtliGetLogVarNameModifier(li);

    if (nameModifier != NULL && strcmp(nameModifier,"none")==0) {
        nameModifier = NULL;
    }

    logVarName[mxMAXNAM-1] = '\0';
    if (nameModifier == NULL) {
        (void)strncpy(logVarName, varName, mxMAXNAM-1);
    } else if (nameModifier[0] == '_') {
        (void)strncpy(logVarName, varName, mxMAXNAM-1);
        nameLen = (int_T)strlen(logVarName);
        (void)strncat(logVarName, nameModifier, mxMAXNAM-1-nameLen);
    } else {
        (void)strncpy(logVarName, nameModifier, mxMAXNAM-1);
        nameLen = (int_T)strlen(logVarName);
        (void)strncat(logVarName, varName, mxMAXNAM-1-nameLen);
    }

} /* end rt_LoadModifiedLogVarName */


/* Function: rt_GetActualDTypeID ===============================================
 * Abstract:
 *  Given a built-in data type id, return the actual data type id.
 *  The only time these are different is when real_T has been mapped
 *  to a single.
 */
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4127)
#endif
static BuiltInDTypeId rt_GetActualDTypeID(BuiltInDTypeId dTypeID)
{
    /*LINTED E_FALSE_LOGICAL_EXPR*/
    if (dTypeID == SS_DOUBLE && sizeof(real_T) != 8) {
        return(SS_SINGLE);
    } else {
        return(dTypeID);
    }

} /* end rt_GetActualDTypeID */
#if defined(_MSC_VER)
#pragma warning(pop)
#endif


/* Function: rt_DestroyLogVar ==================================================
 * Abstract:
 *      Destroy the log var linked list.
 */
static void rt_DestroyLogVar(LogVar *head)
{
#if 0
    while(head) {
        LogVar *var = head;
        head = var->next;
        FREE(var->data.re);
        FREE(var->data.im);
        if (var->data.dims != var->data._dims) {
            FREE(var->data.dims);
        }
        /* free valDims if necessary */
        if(var->valDims != NULL) {
            FREE(var->valDims->dimsData);
            FREE(var->valDims);
        }
        /* free coords, strides and currStrides if necessary */
        FREE(var->coords);
        FREE(var->strides);
        FREE(var->currStrides);

        FREE(var);
    }
#endif
} /* end rt_DestroyLogVar */


/* Function: rt_DestroyStructLogVar ============================================
 * Abstract:
 *      Destroy the struct log var linked list.
 */
static void rt_DestroyStructLogVar(StructLogVar *head)
{
#if 0
    while(head) {
        StructLogVar *var = head;

        head = var->next;

        if (var->logTime) { /* time is LogVar */
            rt_DestroyLogVar(var->time);
        } else {        /* time is MatrixData */
            FREE(var->time);
        }
        rt_DestroyLogVar(var->signals.values);
        FREE(var->signals.labels);
        FREE(var->signals.plotStyles);
        FREE(var->signals.dimensions);
        FREE(var->signals.titles);
        FREE(var->signals.blockNames);
        FREE(var->signals.stateNames);
        FREE(var->signals.crossMdlRef);
        FREE(var->blockName);
        FREE(var);
    }
#endif

} /* end rt_DestroyStructLogVar */


/* Function: rt_InitSignalsStruct ==============================================
 * Abstract:
 *      Initialize the signals structure in the struct log variable.
 *
 * Returns:
 *  == NULL  => success.
 *  ~= NULL  => failure, the return value is a pointer to the error
 *                           message, which is also set in the simstruct.
 */
static const char_T *rt_InitSignalsStruct(RTWLogInfo             *li,
        const real_T           startTime,
        const real_T           finalTime,
        const real_T           inStepSize,
        const char_T           **errStatus,
        StructLogVar           *var,
        int_T                  maxRows,
        int_T                  decimation,
        real_T                 sampleTime,
        const RTWLogSignalInfo *sigInfo)
{
    int_T                i, sigIdx;
    SignalsStruct        *sig          = &(var->signals);
    int_T                nSignals      = sigInfo->numSignals;
    const int_T          *numCols      = sigInfo->numCols;
    const int_T          *numDims      = sigInfo->numDims;
    const int_T          *dims         = sigInfo->dims;
    const BuiltInDTypeId *dTypes       = sigInfo->dataTypes;
    const int_T          *cSgnls       = sigInfo->complexSignals;
    const int_T          *fData        = sigInfo->frameData;
    const char_T         **labels      = sigInfo->labels.cptr;
    const int_T          *plotStyles   = sigInfo->plotStyles;
    const char_T         *titles       = sigInfo->titles;
    const int_T          *titleLen     = sigInfo->titleLengths;
    const char_T         **blockNames  = sigInfo->blockNames.cptr;
    const char_T         **stateNames  = sigInfo->stateNames.cptr;
    const boolean_T      *crossMdlRef  = sigInfo->crossMdlRef;

    void                 **currSigDims = sigInfo->currSigDims;
    int_T                *currSigDimsSize = sigInfo->currSigDimsSize;
    LogVar               *prevValues   = NULL;
    int_T                dimsOffset    = 0;
    boolean_T            *isVarDims    = sigInfo->isVarDims;
    /* if any signal is variable-size, the field 'valueDimensions' is needed */
    boolean_T            logValueDimensions = false;
    const RTWLogDataTypeConvert *pDTConvInfo = sigInfo->dataTypeConvert;
    /* reset error status */
    *errStatus = NULL;

    char *progmemBuffTemp;
    progmemBuffTemp = (char *)malloc(RT_GLOBAL_LOG_NAME_SIZE);

    sig->numActiveFields = 1;
    sig->numSignals      = nSignals;

    sig->isVarDims = isVarDims;
    /* check whether we need valueDimensions field*/
    for (i=0; i<nSignals; i++){
        if(isVarDims[i]){
            logValueDimensions = true;
            break;
        }
    }

    /* values */
    dimsOffset = 0;
    for (i = 0; i < nSignals; i++) {
        BuiltInDTypeId dt = (dTypes) ? dTypes[i] : SS_DOUBLE;
        int_T          cs = (cSgnls) ? cSgnls[i] : 0;
        int_T          fd = (fData)  ? fData[i]  : 0;
        int_T          nd = (numDims) ? numDims[i] : 1;

        const RTWLogDataTypeConvert *pDTConvInfoCur =
                (pDTConvInfo)  ? (pDTConvInfo+i)  : 0;

                LogVar *values = NULL;
                LogValDimsStat logValDimsStat;

                if(!logValueDimensions){
                    logValDimsStat = NO_LOGVALDIMS;
                }
                else{
                    logValDimsStat = isVarDims[i] ? LOGVALDIMS_VARDIMS :
                        LOGVALDIMS_EMPTYMX;
                }

                values = rt_CreateLogVarWithConvert(li, startTime, finalTime,
                        inStepSize, errStatus,
                        VALUES_FIELD_NAME,
                        dt,
                        pDTConvInfoCur,
                        0, cs, fd,
                        numCols[i],nd,
                        dims + dimsOffset,
                        logValDimsStat,
                        currSigDims + dimsOffset,
                        currSigDimsSize + dimsOffset,
                        maxRows,decimation,sampleTime, 0);

                if (values == NULL) goto ERROR_EXIT;

                if (sig->values == NULL) {
                    sig->values = values;
                } else {
                    prevValues->next = values;
                }
                prevValues = values;
                dimsOffset += nd;
    }

    if(logValueDimensions){
        ++sig->numActiveFields;
        sig->logValueDimensions = true;
    }
    else{
        sig->logValueDimensions = false;
    }

    /* Dimensions */
    {
        real_T         *data;
        size_t         nbytes;
        int_T          dataLen    = 0;
        BuiltInDTypeId dTypeId    = rt_GetActualDTypeID(SS_DOUBLE);
        size_t         dataOffset = nSignals*sizeof(MatrixData);
        uint_T         overhang   = (uint_T)(dataOffset % sizeof(real_T));

        if (overhang) {
            dataOffset += (sizeof(real_T) - overhang);
        }
        for (i=0; i< nSignals; i++) {
            int_T nd = (numDims) ? numDims[i] : 1;
            dataLen += nd;
        }
        nbytes = dataOffset + dataLen*sizeof(real_T);

        if ( (sig->dimensions = calloc(nbytes, 1)) == NULL ) goto ERROR_EXIT;

        data = (real_T*) (((char_T*) (sig->dimensions)) + dataOffset);

        for (i = 0; i < dataLen; i++) {
            data[i] = dims[i]; /* cannot memcpy double <- int */
        }

        for (i = 0; i < nSignals; i++) {
            MatrixData *mtxData = &(sig->dimensions[i]);
            int_T nd = (numDims) ? numDims[i] : 1;

            (void)memcpy(mtxData->name, DIMENSION_FIELD_NAME, mxMAXNAM);

            mtxData->nRows   = 1;
            mtxData->nCols   = nd;

            mtxData->nDims   = 1; /* assume */
            mtxData->dims    = mtxData->_dims;
            mtxData->dims[0] = mtxData->nCols;

            mtxData->re      = data;
            mtxData->im      = NULL;
            mtxData->dTypeID = dTypeId;
            mtxData->mxID    = rt_GetMxIdFromDTypeId(dTypeId);
            mtxData->elSize  = rt_GetSizeofDataType(dTypeId);
            mtxData->logical = 0U;
            mtxData->complex = 0U;

            data += nd;
        }
        ++sig->numActiveFields;
    }

    /* labels */
    if (labels != NULL) {
        unsigned short   *data;
        size_t  nbytes;
        int_T   dataLen    = 0;
        size_t  dataOffset = nSignals * sizeof(MatrixData);
        uint_T  overhang   = (uint_T)(dataOffset % sizeof(short));
        int_T dataIdx = 0;

        for (i=0;i<nSignals;i++) {
            if (labels[i] != NULL){
                dataLen = dataLen + (int_T)strlen(labels[i]);
            }
        }

        if (overhang) {
            dataOffset += (sizeof(short) - overhang);
        }
        nbytes = dataOffset + dataLen*sizeof(short);

        if ( (sig->labels = calloc(nbytes, 1)) == NULL ) goto ERROR_EXIT;

        data = (unsigned short*) (((char_T*) (sig->labels)) + dataOffset);
        for(sigIdx=0;sigIdx<nSignals;sigIdx++) {
            int_T labelLen = (labels[sigIdx]==NULL) ? 0 : (int_T)strlen(labels[sigIdx]);
            for (i = 0; i < labelLen; i++) {
                data[dataIdx++] = (uint8_T)labels[sigIdx][i];
            }
        }

        for (i = 0; i < nSignals; i++) {
            MatrixData *mtxData = &(sig->labels[i]);
            int_T labelLen = (int_T)strlen(labels[i]);

            (void)memcpy(mtxData->name, LABEL_FIELD_NAME, mxMAXNAM);
            mtxData->nRows   = (labelLen) ? 1 : 0;
            mtxData->nCols   = labelLen;

            mtxData->re      = data;
            mtxData->im      = NULL;

            mtxData->nDims   = 1; /* assume */
            mtxData->dims    = mtxData->_dims;
            mtxData->dims[0] = mtxData->nCols;

            mtxData->dTypeID = SS_INT16;
            mtxData->mxID    = mxCHAR_CLASS;
            mtxData->elSize  = sizeof(short);
            mtxData->logical = 0U;
            mtxData->complex = 0U;

            data += labelLen;
        }
        ++sig->numActiveFields;
    }

    /* plot styles */
    if (plotStyles != NULL) {
        real_T         *data;
        size_t         nbytes;
        int_T          dataLen    = 0;
        BuiltInDTypeId dTypeId    = rt_GetActualDTypeID(SS_DOUBLE);
        /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
        size_t         dataOffset = nSignals*sizeof(MatrixData);
        /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
        uint_T         overhang   = (uint_T)(dataOffset % sizeof(real_T));

        if (overhang) {
            dataOffset += (sizeof(real_T) - overhang);
        }
        for (i=0; i< nSignals; i++) {
            dataLen += numCols[i];
        }
        /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
        nbytes = dataOffset + dataLen*sizeof(real_T);

        if ( (sig->plotStyles = calloc(nbytes, 1)) == NULL ) goto ERROR_EXIT;

        /*LINTED E_BAD_PTR_CAST_ALIGN*/
        data = (real_T*) (((char_T*) (sig->plotStyles)) + dataOffset);

        for (i = 0; i < dataLen; i++) {
            data[i] = plotStyles[i];
        }

        dimsOffset = 0;
        for (i = 0; i < nSignals; i++) {
            MatrixData *mtxData = &(sig->plotStyles[i]);

            (void)memcpy(mtxData->name, PLOTSTYLE_FIELD_NAME, mxMAXNAM);

            mtxData->nRows   = (numCols[i]) ? 1 : 0;
            mtxData->nCols   = numCols[i];

            mtxData->nDims   = numDims[i];

            if(mtxData->nDims > 2) {
                if ((mtxData->dims = calloc(mtxData->nDims, sizeof(int_T))) == NULL) goto ERROR_EXIT;
            } else {
                mtxData->dims    = mtxData->_dims;
            }

            mtxData->dims[0] = *(dims + dimsOffset);
            if(mtxData->nDims >= 2) {
                int32_T j;
                for (j=1; j<mtxData->nDims; j++) {
                    mtxData->dims[j] = *(dims + dimsOffset + j);
                }
            }

            mtxData->re      = data;
            mtxData->im      = NULL;
            mtxData->dTypeID = dTypeId;
            mtxData->mxID    = rt_GetMxIdFromDTypeId(dTypeId);
            mtxData->elSize  = rt_GetSizeofDataType(dTypeId);
            mtxData->logical = 0U;
            mtxData->complex = 0U;

            data       += numCols[i];
            dimsOffset += numDims[i];
        }
        ++sig->numActiveFields;
    }

    /* titles */
    if (titles != NULL) {
        unsigned short  *data;
        size_t nbytes;
        int_T  dataLen    = (int_T)strlen(titles);
        /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
        size_t  dataOffset = nSignals * sizeof(MatrixData);
        /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
        uint_T  overhang   = (uint_T)(dataOffset % sizeof(short));

        if (overhang) {
            dataOffset += (sizeof(short) - overhang);
        }
        /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
        nbytes = dataOffset + dataLen*sizeof(short);

        if ( (sig->titles = calloc(nbytes, 1)) == NULL ) goto ERROR_EXIT;

        /*LINTED E_BAD_PTR_CAST_ALIGN*/
        data = (unsigned short*) (((char_T*) (sig->titles)) + dataOffset);
        for (i = 0; i < dataLen; i++) {
            data[i] = titles[i];
        }

        for (i = 0; i < nSignals; i++) {
            MatrixData *mtxData = &(sig->titles[i]);

            (void)memcpy(mtxData->name, TITLE_FIELD_NAME, mxMAXNAM);
            if (titleLen) {
                mtxData->nRows   = (titleLen[i]) ? 1 : 0;
                mtxData->nCols   = titleLen[i];
            } else {
                mtxData->nRows   = (dataLen) ? 1 : 0;
                mtxData->nCols   = dataLen;
            }

            mtxData->nDims   = 1; /* assume */
            mtxData->dims    = mtxData->_dims;
            mtxData->dims[0] = mtxData->nCols;

            mtxData->re      = data;
            mtxData->im      = NULL;
            mtxData->dTypeID = SS_INT16;
            mtxData->mxID    = mxCHAR_CLASS;
            mtxData->elSize  = sizeof(short);
            mtxData->logical = 0U;
            mtxData->complex = 0U;

            data += ((titleLen) ? titleLen[i] : dataLen);
        }
        ++sig->numActiveFields;
    }

    /* block names */
    if (blockNames != NULL)     {
        unsigned short  *data;
        size_t nbytes;
        int_T  dataLen = 0;
        size_t dataOffset = nSignals * sizeof(MatrixData);
        uint_T overhang   = (uint_T)(dataOffset % sizeof(short));
        int_T  dataIdx = 0;

        for (i=0;i<nSignals;i++) {
            if (blockNames[i] != NULL) {
                dataLen = dataLen + (int_T)strlen(blockNames[i]);
            }
        }

        if (overhang) {
            dataOffset += (sizeof(short) - overhang);
        }

        nbytes = dataOffset + dataLen*sizeof(short);

        if ( (sig->blockNames = calloc(nbytes, 1)) == NULL ) goto ERROR_EXIT;

        data = (unsigned short*) (((char_T*) (sig->blockNames)) + dataOffset);

        for(sigIdx=0;sigIdx<nSignals;sigIdx++) {
            int_T nameLen = (blockNames[sigIdx]==NULL) ? 0 :
                (int_T)strlen(blockNames[sigIdx]);
                for (i = 0; i < nameLen; i++) {
                    data[dataIdx++] = (uint8_T)blockNames[sigIdx][i];
                }
        }

        for (i = 0; i < nSignals; i++) {
            MatrixData *mtxData = &(sig->blockNames[i]);
            int_T blockNameLen = (int_T)strlen(blockNames[i]);

            (void)memcpy(mtxData->name, BLOCKNAME_FIELD_NAME, mxMAXNAM);
            mtxData->nRows   = (blockNameLen) ? 1 : 0;
            mtxData->nCols   = blockNameLen;

            mtxData->nDims   = 1; /* assume */
            mtxData->dims    = mtxData->_dims;
            mtxData->dims[0] = mtxData->nCols;

            mtxData->re      = data;
            mtxData->im      = NULL;
            mtxData->dTypeID = SS_INT16;
            mtxData->mxID    = mxCHAR_CLASS;
            mtxData->elSize  = sizeof(short);
            mtxData->logical = 0U;
            mtxData->complex = 0U;

            data += blockNameLen;
        }
        ++sig->numActiveFields;
        if(logValueDimensions){
            sig->fieldNames = (char *)memcpy_P((void *)progmemBuffTemp, (void *)rtGlobalLoggingSignalsStructFieldNames, RT_GLOBAL_LOG_NAME_SIZE);
        }
        else{
            sig->fieldNames = (char *)memcpy_P((void *)progmemBuffTemp, (void *)rtGlobalLoggingSignalsStructFieldNames_noValDims, RT_GLOBAL_LOG_NAME_NOVALDIM_SIZE);
        }

    } else {
        if(logValueDimensions){
            sig->fieldNames = (char *)memcpy_P((void *)progmemBuffTemp, (void *)rtLocalLoggingSignalsStructFieldNames, RT_LOCAL_LOG_NAME_SIZE);
        }
        else{
            memcpy_P((void *)progmemBuffTemp, (void *)rtLocalLoggingSignalsStructFieldNames_noValDims, RT_LOCAL_LOG_NAME_NOVALDIM_SIZE);
            sig->fieldNames = progmemBuffTemp;
        }
    }

    /* state names */
    if (stateNames != NULL) {
        unsigned short  *data;
        size_t nbytes;
        int_T  dataLen = 0;
        size_t dataOffset = nSignals * sizeof(MatrixData);
        uint_T overhang   = (uint_T)(dataOffset % sizeof(short));
        int_T  dataIdx = 0;

        for (i=0;i<nSignals;i++) {
            if (stateNames[i] != NULL) {
                dataLen = dataLen + (int_T)strlen(stateNames[i]);
            }
        }

        if (overhang) {
            dataOffset += (sizeof(short) - overhang);
        }

        nbytes = dataOffset + dataLen*sizeof(short);

        if ( (sig->stateNames = calloc(nbytes, 1)) == NULL ) goto ERROR_EXIT;

        data = (unsigned short*) (((char_T*) (sig->stateNames)) + dataOffset);

        for(sigIdx=0;sigIdx<nSignals;sigIdx++) {
            int_T nameLen = (stateNames[sigIdx]==NULL) ? 0 :
                (int_T)strlen(stateNames[sigIdx]);
                for (i = 0; i < nameLen; i++) {
                    data[dataIdx++] = (uint8_T)stateNames[sigIdx][i];
                }
        }

        for (i = 0; i < nSignals; i++) {
            MatrixData *mtxData = &(sig->stateNames[i]);
            int_T stateNameLen = (int_T)strlen(stateNames[i]);

            (void)memcpy(mtxData->name, STATENAME_FIELD_NAME, mxMAXNAM);
            mtxData->nRows   = (stateNameLen) ? 1 : 0;
            mtxData->nCols   = stateNameLen;

            mtxData->nDims   = 1; /* assume */
            mtxData->dims    = mtxData->_dims;
            mtxData->dims[0] = mtxData->nCols;

            mtxData->re      = data;
            mtxData->im      = NULL;
            mtxData->dTypeID = SS_INT16;
            mtxData->mxID    = mxCHAR_CLASS;
            mtxData->elSize  = sizeof(short);
            mtxData->logical = 0U;
            mtxData->complex = 0U;

            data += stateNameLen;
        }
        ++sig->numActiveFields;

        if(logValueDimensions){
            sig->fieldNames = (char *)memcpy_P((void *)progmemBuffTemp,(void *)rtGlobalLoggingSignalsStructFieldNames, RT_GLOBAL_LOG_NAME_SIZE);
        }
        else{
            sig->fieldNames = (char *)memcpy_P((void *)progmemBuffTemp, (void *)rtGlobalLoggingSignalsStructFieldNames_noValDims, RT_GLOBAL_LOG_NAME_NOVALDIM_SIZE);
        }

    }

    /* CrossMdlRef */
    if (crossMdlRef != NULL) {
        real_T  *data;
        size_t nbytes;
        size_t dataOffset = nSignals * sizeof(MatrixData);
        uint_T overhang   = (uint_T)(dataOffset % sizeof(real_T));

        if (overhang) {
            dataOffset += (sizeof(real_T) - overhang);
        }

        nbytes = dataOffset + nSignals*sizeof(real_T);

        if ( (sig->crossMdlRef = calloc(nbytes, 1)) == NULL ) goto ERROR_EXIT;

        data = (real_T*) (((char_T*) (sig->crossMdlRef)) + dataOffset);

        for(sigIdx=0;sigIdx<nSignals;sigIdx++) {
            data[sigIdx] = crossMdlRef[sigIdx];
        }

        for (i = 0; i < nSignals; i++) {
            MatrixData *mtxData = &(sig->crossMdlRef[i]);

            (void)memcpy(mtxData->name, CROSS_MDL_REF_FIELD_NAME, mxMAXNAM);
            mtxData->nRows   = 1;
            mtxData->nCols   = 1;
            mtxData->nDims   = 1; /* => matlab scalar */

            mtxData->re      = &data[i];
            mtxData->im      = NULL;
            mtxData->dTypeID = SS_DOUBLE;
            mtxData->mxID    = rt_GetMxIdFromDTypeId(SS_DOUBLE);
            mtxData->elSize  = sizeof(real_T);
            mtxData->logical = matLOGICAL_BIT;
            mtxData->complex = 0U;
            mtxData->frameData = 0;
            mtxData->frameSize = 1;
        }
        ++sig->numActiveFields;
    }

    return(NULL); /* NORMAL_EXIT */

    ERROR_EXIT:

        /*(void)fprintf(stderr, "*** Error creating signals structure "
                "in the struct log variable %s\n", var->name);*/
        if (*errStatus == NULL) {
            char progmemBuffMsg[strlen_P(rtMemAllocError)];
            *errStatus = (char*)memcpy_P((void *)progmemBuffMsg, (void *)rtMemAllocError, strlen_P(rtMemAllocError));
        }
        rt_DestroyLogVar(sig->values);
        FREE(sig->labels);
        FREE(sig->plotStyles);
        FREE(sig->dimensions);
        FREE(sig->titles);
        FREE(sig->blockNames);
        FREE(sig->stateNames);
        FREE(sig->crossMdlRef);
        FREE(progmemBuffTemp);

        return(*errStatus);

} /* end rt_InitSignalsStruct */


/* Function: local_CreateStructLogVar ==========================================
 * Abstract:
 *      Create a logging variable in the structure format.
 *
 * Returns:
 *      ~= NULL  => success, returns the log variable created.
 *      == NULL  => failure, error message set in the simstruct.
 */
static StructLogVar *local_CreateStructLogVar(
        RTWLogInfo              *li,
        const real_T            startTime,
        const real_T            finalTime,
        const real_T            inStepSize,
        const char_T            **errStatus,
        const char_T            *varName,
        boolean_T               logTime,
        int_T                   maxRows,
        int_T                   decimation,
        real_T                  sampleTime,
        const RTWLogSignalInfo  *sigInfo,
        const char_T            *blockName)
{
    StructLogVar *var;
    LogInfo      *logInfo = rtliGetLogInfo(li);

    /* reset error status */
    *errStatus = NULL;

    if ( (var = calloc(1, sizeof(StructLogVar))) == NULL ) goto ERROR_EXIT;

    var->numActiveFields = 2;

    /* Setup the structure name using varName and nameModifier */
    rt_LoadModifiedLogVarName(li,varName,var->name);

    /* time field */
    if (logTime) {
        /* need to create a LogVar to log time */
        int_T dims = 1;
        var->time = rt_CreateLogVarWithConvert(li, startTime, finalTime,
                inStepSize, errStatus,
                TIME_FIELD_NAME, SS_DOUBLE,
                NULL,
                0, 0, 0, 1,
                1, &dims, NO_LOGVALDIMS,
                NULL, NULL, maxRows,
                decimation, sampleTime, 0);
        if (var->time == NULL) goto ERROR_EXIT;
    } else {
        /* create a dummy MatrixData to write out time as an empty matrix */
        BuiltInDTypeId dt     = rt_GetActualDTypeID(SS_DOUBLE);
        size_t         nbytes = sizeof(MatrixData);
        MatrixData     *time;

        if ( (var->time = calloc(nbytes, 1)) == NULL ) goto ERROR_EXIT;
        time = var->time;

        (void)memcpy(time->name, TIME_FIELD_NAME, mxMAXNAM);
        time->nRows   = 0;
        time->nCols   = 0;
        time->nDims   = 0;
        time->re      = NULL;
        time->im      = NULL;
        time->dTypeID = dt;
        time->mxID    = rt_GetMxIdFromDTypeId(dt);
        time->elSize  = rt_GetSizeofDataType(dt);
        time->logical = 0U;
        time->complex = 0U;
    }
    var->logTime = logTime;

    /* signals field */
    if (sigInfo) {
        if (rt_InitSignalsStruct(li,startTime,finalTime,inStepSize,errStatus,
                var,maxRows,decimation,sampleTime,sigInfo)) {
            goto ERROR_EXIT;
        }
    }

    /* blockName Field */
    if (blockName != NULL) {
        int_T  dataLen = (int_T)strlen(blockName);
        size_t nbytes;
        size_t dataOffset = sizeof(MatrixData);
        /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
        uint_T overhang   = (uint_T)(dataOffset % sizeof(short));

        if (overhang) {
            dataOffset += (sizeof(short) - overhang);
        }
        /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
        nbytes = dataOffset + dataLen*sizeof(short);

        if ( (var->blockName = calloc(nbytes, 1)) == NULL ) goto ERROR_EXIT;

        (void)memcpy(var->blockName->name, BLOCKNAME_FIELD_NAME, mxMAXNAM);
        var->blockName->nRows   = (dataLen) ? 1 : 0;
        var->blockName->nCols   = dataLen;

        var->blockName->nDims   = 1;
        var->blockName->dims    = var->blockName->_dims;
        var->blockName->dims[0] = dataLen;
        {
            /*LINTED E_BAD_PTR_CAST_ALIGN*/
            unsigned short *data = (unsigned short*)(((char_T*) (var->blockName))+dataOffset);
            int_T   i;

            for (i=0; i<dataLen; i++) {
                data[i] = (uint8_T)blockName[i];
            }
            var->blockName->re  = data;
        }
        var->blockName->im      = NULL;
        var->blockName->dTypeID = SS_INT16;
        var->blockName->mxID    = mxCHAR_CLASS;
        var->blockName->elSize  = sizeof(short);
        var->blockName->logical = 0U;
        var->blockName->complex = 0U;

        ++var->numActiveFields;
    }

    /* Add this struct log var to the linked list in log info */
    {
        StructLogVar *list = logInfo->structLogVarsList;

        if (list != NULL) {
            while (list->next != NULL) {
                list = list->next;
            }
            list->next = var;
        } else {
            logInfo->structLogVarsList = var;
        }
    }

    return(var); /* NORMAL_EXIT */

    ERROR_EXIT:
        //(void)fprintf(stderr, "*** Error creating log variable %s\n", varName);
        if (*errStatus == NULL) {
            char progmemBuffMsg[strlen_P(rtMemAllocError)];
            *errStatus = (char*)memcpy_P((void *)progmemBuffMsg, (void *)rtMemAllocError, strlen_P(rtMemAllocError));
        }
        rt_DestroyStructLogVar(var);
        return(NULL);

} /* end local_CreateStructLogVar */


/* Function: rt_StartDataLoggingForOutput ======================================
 * Abstract:
 */
static const char_T *rt_StartDataLoggingForOutput(RTWLogInfo   *li,
        const real_T startTime,
        const real_T finalTime,
        const real_T stepSize,
        const char_T **errStatus)
{
    const char_T   *varName;
    real_T         sampleTime = stepSize;
    int_T          maxRows    = rtliGetLogMaxRows(li);
    int_T          decimation = rtliGetLogDecimation(li);
    int_T          logFormat  = rtliGetLogFormat(li);
    boolean_T      logTime    = (logFormat==2) ? 1 : 0;

    LogInfo *       logInfo;
    logInfo = rtliGetLogInfo(li);

    /* reset error status */
    *errStatus = NULL;

    /* outputs */
    varName = rtliGetLogY(li);
    if (varName[0] != '\0') {
        int_T                  i;
        int_T                  ny;
        int_T                  yIdx;
        char_T                 name[mxMAXNAM];
        const char_T           *cp        = strchr(varName,',');
        LogSignalPtrsType      ySigPtrs   = rtliGetLogYSignalPtrs(li);
        const RTWLogSignalInfo *yInfo     = rtliGetLogYSignalInfo(li);

        /* count the number of variables (matrices or structures) to create */
        for (ny=1; cp != NULL; ny++) {
            cp = strchr(cp+1,',');
        }
        logInfo->ny = ny;

        if (logFormat==0) {
            if ( (logInfo->y = calloc(ny,sizeof(LogVar*))) == NULL ) {
                char progmemBuffMsg[strlen_P(rtMemAllocError)];
                *errStatus = (char*)memcpy_P((void *)progmemBuffMsg, (void *)rtMemAllocError, strlen_P(rtMemAllocError));
                goto ERROR_EXIT;
            }
        } else {
            if ( (logInfo->y = calloc(ny,sizeof(StructLogVar*))) == NULL ) {
                char progmemBuffMsg[strlen_P(rtMemAllocError)];
                *errStatus = (char*)memcpy_P((void *)progmemBuffMsg, (void *)rtMemAllocError, strlen_P(rtMemAllocError));
                goto ERROR_EXIT;
            }
        }

        for (i = yIdx = 0, cp = varName; i < ny; i++) {
            int_T        len;
            const char_T *cp1 = strchr(cp+1,',');

            if (cp1 != NULL) {
                /*LINTED E_ASSIGN_INT_TO_SMALL_INT*/
                len = (int_T)(cp1 - cp);
                if (len >= mxMAXNAM) len = mxMAXNAM - 1;
            } else {
                len = mxMAXNAM - 1;
            }
            (void)strncpy(name, cp, len);
            name[len] = '\0';

            if (ny > 1 && ySigPtrs[i] == NULL) {
                goto NEXT_NAME;
            }

            if (logFormat == 0) {
                int            numCols;
                int            nDims;
                const int      *dims;
                BuiltInDTypeId dataType;
                int            isComplex;

                if (ny == 1) {
                    int_T op;

                    numCols = yInfo[0].numCols[0];
                    for (op = 1; op < yInfo[0].numSignals; op++) {
                        numCols += yInfo[0].numCols[op];
                    }
                    /*
                     * If we have only one "matrix" outport,
                     * we can still log it as a matrix
                     */
                    if (yInfo[0].numSignals == 1) {
                        nDims = yInfo[0].numDims ? yInfo[0].numDims[0] : 1;
                        dims  = yInfo[0].dims;
                    } else {
                        nDims = 1;
                        dims  = &numCols;
                    }

                    dataType  = yInfo[0].dataTypes[0];
                    isComplex = yInfo[0].complexSignals[0];
                } else {
                    numCols   = yInfo[yIdx].numCols[0];
                    nDims     = yInfo[yIdx].numDims ? yInfo[yIdx].numDims[0] : 1;
                    dims      = yInfo[yIdx].dims;
                    dataType  = yInfo[yIdx].dataTypes[0];
                    isComplex = yInfo[yIdx].complexSignals[0];
                }

                logInfo->y[yIdx] = rt_CreateLogVarWithConvert(
                        li, startTime, finalTime,
                        stepSize, errStatus,
                        name,
                        dataType,
                        yInfo[yIdx].dataTypeConvert,
                        0,isComplex,
                        0,numCols,nDims,dims,
                        NO_LOGVALDIMS, NULL, NULL,
                        maxRows,decimation,
                        sampleTime,1);
                if (logInfo->y[yIdx] == NULL)  goto ERROR_EXIT;
            } else {
                logInfo->y[yIdx] = local_CreateStructLogVar(li, startTime,
                        finalTime, stepSize,
                        errStatus, name,
                        logTime, maxRows,
                        decimation, sampleTime,
                        &yInfo[yIdx], NULL);
                if (logInfo->y[yIdx] == NULL) goto ERROR_EXIT;
            }
            ++yIdx;
            NEXT_NAME:
                        cp = cp1;
                        if (cp != NULL && *cp == ',') cp++;
        }
    }

    return(NULL); /* NORMAL_EXIT */

    ERROR_EXIT:
                        //(void)fprintf(stderr, "*** Errors occurred when starting data logging.\n");
                        if (*errStatus == NULL) {
                            char progmemBuffMsg[strlen_P(rtMemAllocError)];
                            *errStatus = (char*)memcpy_P((void *)progmemBuffMsg, (void *)rtMemAllocError, strlen_P(rtMemAllocError));
                        }
                        if (logInfo) {
                            rt_DestroyLogVar(logInfo->logVarsList);
                            logInfo->logVarsList = NULL;
                            rt_DestroyStructLogVar(logInfo->structLogVarsList);
                            logInfo->structLogVarsList = NULL;
                            FREE(logInfo->y);
                            logInfo->y = NULL;
                        }
                        return(*errStatus);

} /* end rt_StartDataLoggingForOutput */


/* Function: rt_ReallocLogVar ==================================================
 * Abstract:
 *   Allocate more memory for the data buffers in the log variable.
 *   Exit if unable to allocate more memory.
 */
static void rt_ReallocLogVar(LogVar *var, boolean_T isVarDims)
{
    void *tmp;
    int_T nCols = var->data.nCols;
    int_T nRows = var->data.nRows + DEFAULT_BUFFER_SIZE;
    size_t elSize = var->data.elSize;

    tmp = realloc(var->data.re, nRows*nCols*elSize);
    if (tmp == NULL) {
        /*(void)fprintf(stderr,
                "*** Memory allocation error.\n");
        (void)fprintf(stderr, ""
                "    varName          = %s%s\n"
                "    nRows            = %d\n"
                "    nCols            = %d\n"
                "    elementSize      = %lu\n"
                "    Current Size     = %.16g\n"
                "    Failed resize    = %.16g\n\n",
                var->data.name,
                var->data.complex ? " (real part)" : "",
                        var->data.nRows,
                        var->data.nCols,
                        (long)  var->data.elSize,
                        (double)nRows*nCols*elSize,
                        (double)(nRows+DEFAULT_BUFFER_SIZE)*nCols*elSize);*/
                exit(1);
    }
    var->data.re = tmp;

    if (var->data.complex) {
        tmp = realloc(var->data.im, nRows*nCols*elSize);
        if (tmp == NULL) {
            /*(void)fprintf(stderr,
                    "*** Memory allocation error.\n");
            (void)fprintf(stderr, ""
                    "    varName          = %s (complex part)\n"
                    "    nRows            = %d\n"
                    "    nCols            = %d\n"
                    "    elementSize      = %lu\n"
                    "    Current Size     = %.16g\n"
                    "    Failed resize    = %.16g\n\n",
                    var->data.name,
                    var->data.nRows,
                    var->data.nCols,
                    (long)  var->data.elSize,
                    (double)nRows*nCols*elSize,
                    (double)(nRows+DEFAULT_BUFFER_SIZE)*nCols*elSize);*/
            exit(1);
        }
        var->data.im = tmp;
    }
    var->data.nRows = nRows;

    /* Also reallocate memory for "valueDimensions"
     * when logging the variable-size signal
     */
    if(isVarDims){
        int_T k;

        nCols = var->valDims->nCols;
        nRows = var->valDims->nRows + DEFAULT_BUFFER_SIZE;
        elSize = sizeof(real_T);
        tmp = realloc(var->valDims->dimsData, nRows*nCols*elSize);
        if (tmp == NULL) {
            /*(void)fprintf(stderr,
                    F("*** Memory allocation error.\n"));
            (void)fprintf(stderr, ""
                    "    varName          = %s\n"
                    "    nRows            = %d\n"
                    "    nCols            = %d\n"
                    "    elementSize      = %lu\n"
                    "    Current Size     = %.16g\n"
                    "    Failed resize    = %.16g\n\n",
                    var->valDims->name,
                    var->valDims->nRows,
                    var->valDims->nCols,
                    (long)  elSize,
                    (double)nRows*nCols*elSize,
                    (double)(nRows+DEFAULT_BUFFER_SIZE)*nCols*elSize);*/
            exit(1);
        }

        /*
         * valueDimensions data is stored in array format and must be
         * adjusted after reallocation (see also rt_FixupLogVar())
         *
         * Example: maxRows = 4; nRows = 4; nDims = 3;
         * Before realloc of the logVar, the locations of data are as below:
         * (x, y, z -- useful data / o -- junk, don't care)
         * a[0] = x    a[4] = y    a[8] = z
         * a[1] = x    a[5] = y    a[9] = z
         * a[2] = x    a[6] = y    a[10]= z
         * a[3] = x    a[7] = y    a[11]= z
         *
         * After realloc of the logVar (suppose 2 extra rows are added),
         * the locations of data are as below:
         * a[0] = x    a[6] = y    a[12]= o
         * a[1] = x    a[7] = y    a[13]= o
         * a[2] = x    a[8] = z    a[14]= o
         * a[3] = x    a[9] = z    a[15]= o
         * a[4] = y    a[10]= z    a[16]= o
         * a[5] = y    a[11]= z    a[17]= o
         *
         * The data must be adjusted as below:
         * a[0] = x    a[6] = y    a[12]= z
         * a[1] = x    a[7] = y    a[13]= z
         * a[2] = x    a[8] = y    a[14]= z
         * a[3] = x    a[9] = y    a[15]= z
         * a[4] = o    a[10]= o    a[16]= o
         * a[5] = o    a[11]= o    a[17]= o
         */
        for(k = var->data.nDims-1; k > 0; k--){
            (void) memcpy((real_T*)tmp + k*nRows,
                    (real_T*)tmp + k*var->valDims->nRows,
                    elSize * var->valDims->nRows);
        }

        var->valDims->dimsData = tmp;
        var->valDims->nRows = nRows;
    }

} /* end rt_ReallocLogVar */


/* Function: rt_UpdateLogVarWithDiscontiguousData ==============================
 * Abstract:
 *      Log one row of the LogVar with data that is not contiguous.
 */
static void rt_UpdateLogVarWithDiscontiguousData(LogVar            *var,
        LogSignalPtrsType data,
        const int_T       *segmentLengths,
        int_T             nSegments)
{
    size_t elSize = 0;
    size_t offset = 0;
    int    segIdx = 0;

    if(bStopLogging ==0)
    {
        if (++var->numHits % var->decimation) return;
        var->numHits = 0;

        /*
         * Reallocate or wrap the LogVar
         */
        if ((var->rowIdx == ((var->data.nRows)/2)) || (var->rowIdx == var->data.nRows)) {
            bDataReady = 1;
        }

        if ((var->rowIdx == ((var->data.nRows)/2)) || (var->rowIdx == var->data.nRows)) {
            bDataReady = 1;
        }

        if(bDataReady ==1) /* switching to the other half of the ping-pong */
        {
            if (var->wrapped == -2)
            {
                var->rowIdx = 0; /* Needs to switch to ping, this var didn't see bDataReady */
                var->wrapped = var->rowIdx; //Store the row Index before wrapped
            }
            else if ((!((var->wrapped == -1) || (var->wrapped == (var->data.nRows + 1)) ))&& ((var->wrapped < (var->data.nRows)/2) && (var->rowIdx < (var->data.nRows)/2))) /* wrapped and rowIdx are in the ping */
            {
                var->rowIdx = ((var->data.nRows)/2); /* switching to pong */
            }
            else if ((!((var->wrapped == -1) || (var->wrapped == (var->data.nRows + 1)) )) && ((var->wrapped >= (var->data.nRows)/2) && (var->rowIdx >= (var->data.nRows)/2))) /* wrapped and rowIdx are in the pong */
            {
                var->rowIdx = 0; /* switching to ping */
            }
            else if (((var->wrapped < (var->data.nRows)/2)  || (var->wrapped == (var->data.nRows + 1))) && (var->rowIdx == var->data.nRows)) /* check if the rowIdx is not crossing boundaries */
            {
                var->rowIdx = (var->data.nRows)/2;
            }
            else if (((var->wrapped >= (var->data.nRows)/2) || (var->wrapped == -1)) && (var->rowIdx == var->data.nRows/2)) /*check if the rowIdx is not crossing boundaries */
            {
                var->rowIdx = 0;
            }
        }
        else if (var->wrapped == -2)
        {
            var->rowIdx = 0; /* Needs to switch to ping, this var didn't see bDataReady */
            var->wrapped = var->rowIdx; //Store the row Index before wrapped
        }
        else if ((var->wrapped < (var->data.nRows)/2) && (var->rowIdx >= (var->data.nRows)/2)) /* wrapped is in the ping, rowIdx in the pong */
        {
            var->rowIdx = 0; /* Needs to switch to ping, this var didn't see bDataReady */
            var->wrapped = var->rowIdx; //Store the row Index before wrapped
        }
        else if ((var->wrapped >= (var->data.nRows)/2) && (var->rowIdx < (var->data.nRows)/2)) /* wrapped is in the pong, rowIdx in the ping */
        {
            var->rowIdx = ((var->data.nRows)/2); /* Needs to switch to pong, this var didn't see bDataReady */
            var->wrapped = var->rowIdx; //Store the row Index before wrapped
        }
        else
        {
            var->wrapped = var->rowIdx; //Store the row Index before wrapped
        }

        /* This function is only used to log states, there's no var-dims issue. */
        elSize = var->data.elSize;
        offset = (size_t)(elSize * var->rowIdx * var->data.nCols);

        if (var->data.complex) {
            char_T *dstRe = (char_T*)(var->data.re) + offset;
            char_T *dstIm = (char_T*)(var->data.im) + offset;

            for (segIdx = 0; segIdx < nSegments; segIdx++) {
                int_T         nEl  = segmentLengths[segIdx];
                const  char_T *src = (const void *)data[segIdx];
                int_T         el;

                for (el = 0; el < nEl; el++) {
                    (void)memcpy(dstRe, src, elSize);
                    dstRe += elSize;   src += elSize;
                    (void)memcpy(dstIm, src, elSize);
                    dstIm += elSize;   src += elSize;
                }
            }
        } else {
            char_T *dst = (char_T*)(var->data.re) + offset;

            for (segIdx = 0; segIdx < nSegments; segIdx++) {
                size_t      segSize = elSize*segmentLengths[segIdx];
                const  void *src    = data[segIdx];

                (void)memcpy(dst, src, segSize);
                dst += segSize;
            }
        }

        ++var->rowIdx;
    }
    return;

} /* end rt_UpdateLogVarWithDiscontiguousData */


/*==================*
 * Visible routines *
 *==================*/



#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_CreateLogVarWithConvert ========================================
     * Abstract:
     *  Create a logging variable.
     *
     * Returns:
     *  ~= NULL  => success, returns the log variable created.
     *  == NULL  => failure, error message set in the simstruct.
     */
    LogVar *rt_CreateLogVarWithConvert(
            RTWLogInfo        *li,
            const real_T      startTime,
            const real_T      finalTime,
            const real_T      inStepSize,
            const char_T      **errStatus,
            const char_T      *varName,
            BuiltInDTypeId    inpDataTypeID,
            const RTWLogDataTypeConvert *pDataTypeConvertInfo,
            int_T             logical,
            int_T             complex,
            int_T             frameData,
            int_T             nCols,
            int_T             nDims,
            const int_T       *dims,
            LogValDimsStat    logValDimsStat,
            void              **currSigDims,
            int_T             *currSigDimsSize,
            int_T             maxRows,
            int_T             decimation,
            real_T            sampleTime,
            int_T             appendToLogVarsList)
    {
        int_T          usingDefaultBufSize = 0;
#ifdef NO_LOGGING_REALLOC
        int_T          okayToRealloc       = 0;
#else
        int_T          okayToRealloc       = 1;
#endif
        LogVar         *var                = NULL;
        /*inpDataTypeID is the rt_LoggedOutputDataTypeId*/
        BuiltInDTypeId dTypeID             = (BuiltInDTypeId)inpDataTypeID;
        size_t         elementSize         = rt_GetSizeofDataType(dTypeID);
        int_T          frameSize;
        int_T          nRows;
        int_T          nColumns;
        char progmemBuffMsg[strlen_P(rtMemAllocError)];

        /*===================================================================*
         * Determine the frame size if the data is frame based               *
         *===================================================================*/
        frameSize = frameData ? dims[0] : 1;

        /*===================================================================*
         * Calculate maximum number of rows needed in the buffer             *
         *===================================================================*/

        if (finalTime > startTime && finalTime != rtInf) {
            real_T nPoints;            /* Tfinal is finite  ===>  nRows can be  */
            real_T stepSize;           /* computed since the StepSize is fixed  */

            if (sampleTime == -2.0) {  /* The signal being logged is constant,  *
             * Hence, only one data point is logged. */
                stepSize = finalTime;
            } else if (sampleTime == -1.0 || sampleTime == 0.0) {
                /* Signal being logged is either inside a    *
                 * triggered sub-system or it is continuous. */
                stepSize = inStepSize;
            } else {                /* Discrete signal */
                stepSize = sampleTime;
            }

            if (stepSize == 0.0) {
                /* small initial value, so as to exercise the realloc code */
                nRows = maxRows+1;
                okayToRealloc = 1;
            } else {
                nPoints = 1.0 + floor((finalTime-startTime)/stepSize);

                /*
                 * Add one more data point if needed.
                 */
                if ( stepSize*(nPoints-1.0) < (finalTime-startTime) ) {
                    nPoints += 1.0;
                }

                /*
                 * Actual number of points to log = nPoints * size of
                 * each frame if data is frame-based
                 */
                nPoints = frameData ? (nPoints * frameSize) : nPoints;

                nPoints /= decimation;
                if (nPoints != floor(nPoints)) {
                    nPoints += 1.0;
                }
                nRows = (nPoints <= INT_MAX) ? ((int_T) nPoints) : INT_MAX;
            }
            /*
             * If maxRows is specified, and if this number is less
             * than the number we computed (nRows) then use maxRows.
             */
            if ((maxRows > 0) && (maxRows < nRows)) {
                nRows = maxRows;
                okayToRealloc = 0;
            }
        } else if (finalTime == startTime) {
            /*
             * Number of rows to log is equal to 1 if not frame-based and
             * equal to frame size if frame-based
             */
            nRows = frameData ? frameSize : 1;

            /*
             * If maxRows is specified, and if this number is less
             * than the number we computed (nRows) then use maxRows.
             */
            if ((maxRows > 0) && (maxRows < nRows)) {
                nRows = maxRows;
                okayToRealloc = 0;
            }
        } else if (maxRows > 0) {     /* maxRows is specified => nRows=maxRows  */
            nRows = maxRows;
            okayToRealloc = 0;
        } else {

            if (inStepSize == 0) {
                /* small initial value, so as to exercise the realloc code */
                nRows = maxRows+1;
                okayToRealloc = 1;
            } else {                    /* Use a default value for nRows          */
                usingDefaultBufSize = 1;
                nRows = DEFAULT_BUFFER_SIZE;
                okayToRealloc = 0;  /* No realloc with infinite stop time */
                /*(void)fprintf(stdout, "*** Using a default buffer of size %d for "
                        "logging variable %s\n", nRows, varName);*/
            }
        }

        /*
         * Figure out the number of columns that the log variable should have.
         * If the data is not frame based, then number of columns should equal
         * nCols that is provided as input to the function. If the data is
         * frame-based, then the number of columns should be equal to the
         * number of channels = nCols/frameSize = dims[1];
         */
        nColumns = frameData ? dims[1] : nCols;

        /*
         * Error out if the size of the circular buffer is absurdly large, this
         * error message is more informative than the one we get when we try to
         * malloc this many number of bytes in one fell swoop.
         */
        {
            double tmpDbl = ((double)elementSize)*((double)nRows)*
                    ((double)nColumns);

            if (tmpDbl >= UINT_MAX) {
                /*(void)fprintf(stderr,
                        "\n*** Memory required to log variable '%s' is too"
                        "\n    big. Use the 'Limit rows to last:' and (or)"
                        "\n    'Decimation:' options to reduce the required"
                        "\n    memory size.\n", varName);
                (void)fprintf(stderr, "*** Details:\n"
                        "       varName         = %s\n"
                        "       nRows           = %d\n"
                        "       nCols           = %d\n"
                        "       elementSize     = %lu\n"
                        "       Bytes Required  = %.16g\n\n",
                        varName, nRows, nColumns, (unsigned long)
                        elementSize, tmpDbl);*/
                goto ERROR_EXIT;
            }
        }

        /* Allocate memory for the log variable */
        if ( (var = calloc(1, sizeof(LogVar))) == NULL ) {
            /*(void)fprintf(stderr, "*** Error allocating memory for logging %s\n",
                    varName);*/
            goto ERROR_EXIT;
        }

        /* Allocate memory for the circular buffer (real part) */
        if ( (var->data.re = malloc(nRows*nColumns*elementSize)) == NULL ) {
            /*(void)fprintf(stderr,
                    "*** Error allocating memory for the circular buffer\n");
            (void)fprintf(stderr, "*** Details:\n"
                    "       varName         = %s\n"
                    "       nRows           = %d\n"
                    "       nCols           = %d\n"
                    "       elementSize     = %lu\n"
                    "       Bytes Requested = %.16g\n\n",
                    varName, nRows, nColumns, (unsigned long) elementSize,
                    ((double)elementSize)*((double)nRows)*((double)nColumns));*/
            goto ERROR_EXIT;
        }

        /* Allocate memory for the circular buffer for the imaginary part */
        if (complex) {
            if ( (var->data.im = malloc(nRows*nColumns*elementSize)) == NULL ) {
                /*(void)fprintf(stderr,
                        "*** Error allocating memory for the circular buffer "
                        "for logging the imaginary part of %s\n", varName);
                (void)fprintf(stderr, "*** Details:\n"
                        "       varName         = %s\n"
                        "       nRows           = %d\n"
                        "       nCols           = %d\n"
                        "       elementSize     = %lu\n"
                        "       Bytes Requested = %.16g\n\n",
                        varName, nRows, nColumns, (unsigned long) elementSize,
                        ((double)elementSize)*((double)nRows)*
                        ((double)nColumns));*/
                goto ERROR_EXIT;
            }
        }
        /*
         * Initialize the fields in LogVar structure.
         */
        if (appendToLogVarsList) {
            rt_LoadModifiedLogVarName(li,varName,var->data.name);
        } else {
            var->data.name[mxMAXNAM-1] = '\0';
            (void)strncpy(var->data.name,varName,mxMAXNAM-1);
        }
        var->data.nCols           = nColumns;
        var->data.nRows           = nRows;

        var->data.nDims           = frameData ? 1 : nDims;
        if (var->data.nDims > 2) {
            var->data.dims = (int_T*)malloc(sizeof(int_T)*var->data.nDims);
        } else {
            var->data.dims = var->data._dims;
        }
        if (frameData) {
            var->data.dims[0] = nColumns;
        } else {
            /*LINTED E_CAST_INT_TO_SMALL_INT*/
            (void)memcpy(var->data.dims, dims, (size_t)(nDims*sizeof(int_T)));
        }

        var->data.dTypeID         = dTypeID;
        var->data.elSize          = elementSize;

        var->data.dataTypeConvertInfo = rt_GetDataTypeConvertInfo(
                pDataTypeConvertInfo, dTypeID);

        var->data.mxID            = rt_GetMxIdFromDTypeId(dTypeID);
        /* over-ride logical bit if data type is boolean */
        logical = dTypeID == SS_BOOLEAN ? 1 : 0;
        var->data.logical         = (logical)   ? matLOGICAL_BIT : 0x0;
        var->data.complex         = (complex)   ? matCOMPLEX_BIT : 0x0;
        var->data.frameData       = frameData;
        var->data.frameSize       = (frameData) ? frameSize : 1;

        /* fill up valDims field */
        if(logValDimsStat == NO_LOGVALDIMS){
            /* All signals are fixed-size, no need to log valueDimensions field */
            var->valDims     = NULL;
            /* Set these pointers to NULLs in this case */
            var->coords      = NULL;
            var->strides     = NULL;
            var->currStrides = NULL;
        }
        else{
            if ( (var->valDims = calloc(1, sizeof(ValDimsData))) == NULL ) {
                goto ERROR_EXIT;
            }

            (void)memcpy(var->valDims->name, VALUEDIMENSIONS_FIELD_NAME, mxMAXNAM);

            if (logValDimsStat == LOGVALDIMS_EMPTYMX) {
                /* At least one signal is variable-size,
                 * but the current signal is fixed-size.
                 * Therefore, create a dummy MatrixData to write out valueDimensions
                 * as an empty matrix.
                 */
                var->valDims->nRows = 0;
                var->valDims->nCols = 0;
                var->valDims->currSigDims = NULL;
                var->valDims->currSigDimsSize = NULL;
                var->valDims->dimsData = NULL;
                /* Set these pointers to NULLs in this case */
                var->coords      = NULL;
                var->strides     = NULL;
                var->currStrides = NULL;
            } else { /* The current signal is a variable-size signal. */
                /* The "valueDimensions" must be double, so re-assign element size */
                elementSize = sizeof(real_T);

                /* When signals are frame-based, 'valueDimensions' has 1 column */
                if(frameData){
                    /* When signal is frame-based, the first dimension is always fixed,
                     * so we only need to record the second dimension.
                     * e.g. Two frame-based signals [10x4] and [10x3],
                     * 'valueDimensions' and 'currSigDims'
                     * only record 4 or 3.
                     */
                    nColumns = 1;
                    var->valDims->currSigDims = (void**) (currSigDims + 1);
                    var->valDims->currSigDimsSize = (int_T*) (currSigDimsSize + 1);
                } else { /* non-frame based */
                    nColumns = nDims;
                    var->valDims->currSigDims = (void**) currSigDims;
                    var->valDims->currSigDimsSize = (int_T*) currSigDimsSize;
                }

                /* Allocate memory for the circular buffer */
                if ( (var->valDims->dimsData = malloc(nRows*nColumns*elementSize)) == NULL ) {
                    /*(void)fprintf(stderr,
                            "*** Error allocating memory for the circular buffer\n");
                    (void)fprintf(stderr, "*** Details:\n"
                            "       varName         = %s\n"
                            "       nRows           = %d\n"
                            "       nCols           = %d\n"
                            "       elementSize     = %lu\n"
                            "       Bytes Requested = %.16g\n\n",
                            var->valDims->name, nRows, nColumns, (unsigned long) elementSize,
                            ((double)elementSize)*((double)nRows)*((double)nColumns));*/
                    goto ERROR_EXIT;
                }
                var->valDims->nRows = nRows;
                var->valDims->nCols = nColumns;

                /* Allocate memory for these dynamic arrays */
                {
                    size_t nbytes = var->data.nDims*sizeof(int_T);
                    if( ((var->coords = calloc(nbytes, 1)) == NULL)
                    ||((var->strides = calloc(nbytes, 1)) == NULL)
                    ||((var->currStrides = calloc(nbytes, 1)) == NULL) )
                        goto ERROR_EXIT;
                }
            }
        }

        var->rowIdx               = 0;
        var->wrapped              = 0;
        var->nDataPoints          = 0;
        var->usingDefaultBufSize  = usingDefaultBufSize;
        var->okayToRealloc        = okayToRealloc;
        var->decimation           = decimation;
        var->numHits              = -1;  /* so first point gets logged */

        /* Add this log var to list in log info, if necessary */
        if (appendToLogVarsList) {
            LogInfo *logInfo = (LogInfo*) rtliGetLogInfo(li);
            LogVar  *varList = logInfo->logVarsList;

            if (varList != NULL) {
                while (varList->next != NULL) {
                    varList = varList->next;
                }
                varList->next = var;
            } else {
                logInfo->logVarsList = var;
            }
        }

        return(var); /* NORMAL_EXIT */

        ERROR_EXIT:
            *errStatus = (char*)memcpy_P((void *)progmemBuffMsg, (void *)rtMemAllocError, strlen_P(rtMemAllocError));
            rt_DestroyLogVar(var);
            return(NULL);

    } /* end rt_CreateLogVarWithConvert */


#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_CreateLogVar ===================================================
     * Abstract:
     *  Create a logging variable.
     *
     * Returns:
     *  ~= NULL  => success, returns the log variable created.
     *  == NULL  => failure, error message set in the simstruct.
     */
    LogVar *rt_CreateLogVar(RTWLogInfo        *li,
            const real_T      startTime,
            const real_T      finalTime,
            const real_T      inStepSize,
            const char_T      **errStatus,
            const char_T      *varName,
            BuiltInDTypeId    inpDataTypeID,
            int_T             logical,
            int_T             complex,
            int_T             frameData,
            int_T             nCols,
            int_T             nDims,
            const int_T       *dims,
            LogValDimsStat    logValDimsStat,
            void              **currSigDims,
            int_T             *currSigDimsSize,
            int_T             maxRows,
            int_T             decimation,
            real_T            sampleTime,
            int_T             appendToLogVarsList)
    {
        const RTWLogDataTypeConvert *pDataTypeConvertInfo = NULL;

        return rt_CreateLogVarWithConvert(li,
                startTime,
                finalTime,
                inStepSize,
                errStatus,
                varName,
                inpDataTypeID,
                pDataTypeConvertInfo,
                logical,
                complex,
                frameData,
                nCols,
                nDims,
                dims,
                logValDimsStat,
                currSigDims,
                currSigDimsSize,
                maxRows,
                decimation,
                sampleTime,
                appendToLogVarsList);

    } /* end rt_CreateLogVar */


#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_CreateStructLogVar =============================================
     * Abstract:
     *  Create a logging variable in the structure format.
     *
     * Returns:
     *  ~= NULL  => success, returns the log variable created.
     *  == NULL  => failure, error message set in the simstruct.
     */
    StructLogVar *rt_CreateStructLogVar(RTWLogInfo              *li,
            const real_T            startTime,
            const real_T            finalTime,
            const real_T            inStepSize,
            const char_T            **errStatus,
            const char_T            *varName,
            boolean_T               logTime,
            int_T                   maxRows,
            int_T                   decimation,
            real_T                  sampleTime,
            const RTWLogSignalInfo  *sigInfo,
            const char_T            *blockName)
    {

        return( local_CreateStructLogVar(li,
                startTime,
                finalTime,
                inStepSize,
                errStatus,
                varName,
                logTime,
                maxRows,
                decimation,
                sampleTime,
                sigInfo,
                blockName));

    } /* end rt_CreateStructLogVar */


#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_StartDataLoggingWithStartTime ==================================
     * Abstract:
     *      Initialize data logging info based upon the following settings cached
     *      in the RTWLogging data structure of the SimStruct.
     *
     * Return value is:
     *  == NULL  => success
     *  != NULL  => failure (the return value is a pointer that points to the
     *                           error message, which is also set in the simstruct)
     */
    const char_T *rt_StartDataLoggingWithStartTime(RTWLogInfo   *li,
            const real_T startTime,
            const real_T finalTime,
            const real_T stepSize,
            const char_T **errStatus)
    {
        const char_T   *varName;
        LogInfo        *logInfo;
        real_T         sampleTime = stepSize;
        int_T          maxRows    = rtliGetLogMaxRows(li);
        int_T          decimation = rtliGetLogDecimation(li);
        int_T          logFormat  = rtliGetLogFormat(li);
        boolean_T      logTime    = (logFormat==2) ? 1 : 0;

        /* reset error status */
        *errStatus = NULL;

        if ((logInfo=calloc(1,sizeof(LogInfo))) == NULL) {
            char progmemBuffMsg[strlen_P(rtMemAllocError)];
            *errStatus = (char*)memcpy_P((void *)progmemBuffMsg, (void *)rtMemAllocError, strlen_P(rtMemAllocError));
            goto ERROR_EXIT;
        }
        rtliSetLogInfo(li, (void*)logInfo);

        /* time */
        varName = rtliGetLogT(li);
        if (varName[0] != '\0') {
            int_T dims = 1;
            logInfo->t = rt_CreateLogVarWithConvert(li, startTime, finalTime,
                    stepSize, errStatus,
                    varName,SS_DOUBLE,
                    NULL,
                    0,0,0,1,1,
                    &dims, NO_LOGVALDIMS, NULL, NULL,
                    maxRows,decimation,
                    sampleTime,1);
            if (logInfo->t == NULL) goto ERROR_EXIT;
        }

        /* states */
        if ( rtliGetLogX(li)[0] != '\0' ||  rtliGetLogXFinal(li)[0] != '\0' ) {
            const RTWLogSignalInfo  *xInfo = rtliGetLogXSignalInfo(li);

            if (logFormat == 0) {                                /* Matrix Format */
                int            numCols;
                int            nDims;
                const int      *dims;
                BuiltInDTypeId dataType;
                int            isComplex;
                int_T          sIdx;

                const RTWLogDataTypeConvert *pDTConvInfo;

                numCols = xInfo[0].numCols ? xInfo[0].numCols[0] : 0;
                for (sIdx = 1; sIdx < xInfo[0].numSignals; sIdx++) {
                    numCols += xInfo[0].numCols[sIdx];
                }
                /* If we have only one "matrix" state, we can log as a matrix */
                if (xInfo[0].numSignals == 1) {
                    nDims     = xInfo[0].numDims ? xInfo[0].numDims[0] : 1;
                    dims      = xInfo[0].dims;
                } else {
                    nDims     = 1;
                    dims      = &numCols;
                }
                dataType  = xInfo[0].dataTypes ? xInfo[0].dataTypes[0] : 0;
                isComplex = xInfo[0].complexSignals ? xInfo[0].complexSignals[0] : 0;

                pDTConvInfo = xInfo[0].dataTypeConvert;

                if (rtliGetLogX(li)[0] != '\0') {
                    logInfo->x = rt_CreateLogVarWithConvert(li, startTime, finalTime,
                            stepSize, errStatus,
                            rtliGetLogX(li),dataType,
                            pDTConvInfo,
                            0,
                            isComplex,0,numCols,nDims,dims,
                            NO_LOGVALDIMS, NULL, NULL,
                            maxRows,decimation,sampleTime,1);
                    if (logInfo->x == NULL)  goto ERROR_EXIT;
                }
                if (rtliGetLogXFinal(li)[0] != '\0') {
                    logInfo->xFinal = rt_CreateLogVarWithConvert(li, startTime, finalTime,
                            stepSize, errStatus,
                            rtliGetLogXFinal(li),dataType,
                            pDTConvInfo,
                            0,isComplex,0,numCols,nDims,
                            dims, NO_LOGVALDIMS, NULL,
                            NULL, 1,decimation,
                            sampleTime,1);
                    if (logInfo->xFinal == NULL)  goto ERROR_EXIT;
                }
            } else {                                          /* Structure Format */
                if (rtliGetLogX(li)[0] != '\0') {
                    logInfo->x = local_CreateStructLogVar(li, startTime, finalTime,
                            stepSize, errStatus,
                            rtliGetLogX(li), logTime,
                            maxRows, decimation,
                            sampleTime, xInfo, NULL);
                    if (logInfo->x == NULL) goto ERROR_EXIT;
                }
                if (rtliGetLogXFinal(li)[0] != '\0') {
                    logInfo->xFinal = local_CreateStructLogVar(li, startTime, finalTime,
                            stepSize, errStatus,
                            rtliGetLogXFinal(li),
                            logTime,1,decimation,
                            sampleTime,xInfo,NULL);
                    if (logInfo->xFinal == NULL) goto ERROR_EXIT;
                }
            }
        }

        /* outputs */
        *errStatus = rt_StartDataLoggingForOutput(li,startTime,finalTime,
                stepSize,errStatus);
        if (*errStatus != NULL)  goto ERROR_EXIT;

        return(NULL); /* NORMAL_EXIT */

        ERROR_EXIT:
                    //(void)fprintf(stderr, "*** Errors occurred when starting data logging.\n");
                    if (*errStatus == NULL) {
                        char progmemBuffMsg[strlen_P(rtMemAllocError)];
                        *errStatus = (char*)memcpy_P((void *)progmemBuffMsg, (void *)rtMemAllocError, strlen_P(rtMemAllocError));
                    }
                    if (logInfo) {
                        rt_DestroyLogVar(logInfo->logVarsList);
                        logInfo->logVarsList = NULL;
                        rt_DestroyStructLogVar(logInfo->structLogVarsList);
                        logInfo->structLogVarsList = NULL;
                    }
                    return(*errStatus);

    } /* end rt_StartDataLoggingWithStartTime */


#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_StartDataLogging ===============================================
     * Abstract:
     */
    const char_T *rt_StartDataLogging(RTWLogInfo   *li,
            const real_T finalTime,
            const real_T stepSize,
            const char_T **errStatus)
    {
        return rt_StartDataLoggingWithStartTime(li,
                0.0,
                finalTime,
                stepSize,
                errStatus);
    }


#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_UpdateLogVar ===================================================
     * Abstract:
     *  Called to log data for a log variable.
     */
    void rt_UpdateLogVar(LogVar *var, const void *data, boolean_T isVarDims)
    {
        if(bStopLogging ==0)
        {
            size_t        elSize    = var->data.elSize;
            const  char_T *cData    = data;
            const  int_T  frameData = var->data.frameData;
            const  int_T  frameSize = frameData ? (var->data.frameSize) : 1;
            const  int_T  logWidth  = var->data.nCols;
            BuiltInDTypeId dTypeID  = var->data.dTypeID;

            size_t offset        = 0;
            char_T *currRealRow  = NULL;
            char_T *currImagRow  = NULL;
            int_T  pointSize     = (int_T)((var->data.complex) ? rt_GetSizeofComplexType(dTypeID) : elSize);

            int    i, j, k;

            /* The following variables will be used for
             * logging variable-size signals */
            const  int_T  nDims = var->data.nDims;
            const  int_T  *dims = var->data.dims;
            const  void   * const *currDimsPtr = NULL;
            const  int_T  *currDimsSizePtr = NULL;

            /* The following variables will be used for
             * logging "valueDimensions" field */
            size_t offset_valDims   = 0;
            char_T *currValDimsRow  = NULL;
            size_t elSize_valDims   = sizeof(real_T);
            real_T currentSigDims   = 0;
            int_T  nRows_valDims    = 0;
            int_T  logWidth_valDims = 0;


            for (i = 0; i < frameSize; i++) {
                if (++var->numHits % var->decimation) continue;
                var->numHits = 0;

                if ((var->rowIdx == ((var->data.nRows)/2)) || (var->rowIdx == var->data.nRows)) {
                    bDataReady = 1;
                }

                if(bDataReady ==1) /* switching to the other half of the ping-pong */
                {
                    if (var->wrapped == -2)
                    {
                        var->rowIdx = 0; /* Needs to switch to ping, this var didn't see bDataReady */
                        var->wrapped = var->rowIdx; //Store the row Index before wrapped
                    }
                    else if ((!((var->wrapped == -1) || (var->wrapped == (var->data.nRows + 1)) ))&& ((var->wrapped < (var->data.nRows)/2) && (var->rowIdx < (var->data.nRows)/2))) /* wrapped and rowIdx are in the ping */
                    {
                        var->rowIdx = ((var->data.nRows)/2); /* switching to pong */
                    }
                    else if ((!((var->wrapped == -1) || (var->wrapped == (var->data.nRows + 1)) )) && ((var->wrapped >= (var->data.nRows)/2) && (var->rowIdx >= (var->data.nRows)/2))) /* wrapped and rowIdx are in the pong */
                    {
                        var->rowIdx = 0; /* switching to ping */
                    }
                    else if (((var->wrapped < (var->data.nRows)/2)  || (var->wrapped == (var->data.nRows + 1))) && (var->rowIdx == var->data.nRows)) /* check if the rowIdx is not crossing boundaries */
                    {
                        var->rowIdx = (var->data.nRows)/2;
                    }
                    else if (((var->wrapped >= (var->data.nRows)/2) || (var->wrapped == -1)) && (var->rowIdx == var->data.nRows/2)) /*check if the rowIdx is not crossing boundaries */
                    {
                        var->rowIdx = 0;
                    }
                }
                else if (var->wrapped == -2)
                {
                    var->rowIdx = 0; /* Needs to switch to ping, this var didn't see bDataReady */
                    var->wrapped = var->rowIdx; //Store the row Index before wrapped
                }
                else if ((var->wrapped < (var->data.nRows)/2) && (var->rowIdx >= (var->data.nRows)/2)) /* wrapped is in the ping, rowIdx in the pong */
                {
                    var->rowIdx = 0; /* Needs to switch to ping, this var didn't see bDataReady */
                    var->wrapped = var->rowIdx; //Store the row Index before wrapped
                }
                else if ((var->wrapped >= (var->data.nRows)/2) && (var->rowIdx < (var->data.nRows)/2)) /* wrapped is in the pong, rowIdx in the ping */
                {
                    var->rowIdx = ((var->data.nRows)/2); /* Needs to switch to pong, this var didn't see bDataReady */
                    var->wrapped = var->rowIdx; //Store the row Index before wrapped
                }
                else
                {
                    var->wrapped = var->rowIdx; //Store the row Index before wrapped
                }

                if(isVarDims){
                    currDimsPtr = (const void * const *) var->valDims->currSigDims;
                    currDimsSizePtr = (const int_T*) var->valDims->currSigDimsSize;
                    logWidth_valDims = frameData ? 1 : var->valDims->nCols;
                    nRows_valDims = var->valDims->nRows;

                    var->strides[0] = 1;
                    var->currStrides[0] = 1;

                    for (k = 1; k < nDims; k++){
                        int32_T currDimsVal=0;
                        switch (currDimsSizePtr[k-1]) {
                            case 1:
                                currDimsVal = (**(((const uint8_T * const *) currDimsPtr)+(k-1)));
                                break;
                            case 2:
                                currDimsVal = (**(((const uint16_T * const *) currDimsPtr)+(k-1)));
                                break;
                            case 4:
                                currDimsVal = (**(((const uint32_T * const *) currDimsPtr)+(k-1)));
                                break;
                        }
                        var->strides[k] = var->strides[k-1] * dims[k-1];
                        var->currStrides[k] = var->currStrides[k-1] * currDimsVal;
                    }
                }

                offset       = (size_t)(elSize * var->rowIdx * logWidth);
                currRealRow  = ((char_T*) (var->data.re)) + offset;
                currImagRow  = (var->data.complex) ?
                    ((char_T*) (var->data.im)) + offset :  NULL;

                    /* update logging data */
                    for (j = 0; j < logWidth; j++) {

                        boolean_T inRange = true;
                        int idx = j;

                        /* Check whether the currently logged value is in range or not.
                         * For fixed-size signal logging, always inRange = true; idx = j;
                         * For variable-size signal logging, use strides, coordinates
                         * and current strides to decide whether the currently logged
                         * data is in range or not and its location in the logging
                         * matrix.
                         */
                        if(isVarDims){
                            int rem = j;
                            idx = 0;
                            for(k = nDims-1; k>=0; k--){
                                int32_T currDimsVal=0;
                                switch (currDimsSizePtr[k]) {
                                    case 1:
                                        currDimsVal = (**(((const uint8_T * const *) currDimsPtr)+k));
                                        break;
                                    case 2:
                                        currDimsVal = (**(((const uint16_T * const *) currDimsPtr)+k));
                                        break;
                                    case 4:
                                        currDimsVal = (**(((const uint32_T * const *) currDimsPtr)+k));
                                        break;
                                }
                                var->coords[k] = rem / var->strides[k];
                                if( var->coords[k] >= currDimsVal ){
                                    inRange = false;
                                    break;
                                }
                                rem = rem - var->coords[k] * var->strides[k];
                            }
                            if(inRange){
                                idx = var->coords[0];
                                for (k = 1; k < nDims; k++){
                                    idx += var->coords[k] * var->currStrides[k];
                                }
                            }
                        }

                        if (!var->data.dataTypeConvertInfo.conversionNeeded) {
                            /* NO  conversion needed
                             */
                            if (inRange) {
                                /* If in range, fill in data */
                                const char *cDataPoint = cData + (i+frameSize*idx) * pointSize;

                                (void) memcpy(currRealRow, cDataPoint, elSize);
                                currRealRow += elSize;
                                if (var->data.complex) {
                                    (void) memcpy(currImagRow, cDataPoint + pointSize/2, elSize);
                                    currImagRow += elSize;
                                }
                            } else {
                                /* If out of range, fill in NaN or 0:
                                 * 1) For bool, int32, uint32, int16, uint16, etc,
                                 * memset to zeros;
                                 * 2) For fixed-point data type, NaN conversion is not
                                 * allowed, memset to zeros.
                                 */
                                if (dTypeID == SS_DOUBLE) {
                                    (void) memcpy(currRealRow, &rtNaN, elSize);
                                } else if (dTypeID == SS_SINGLE){
                                    (void) memcpy(currRealRow, &rtNaNF, elSize);
                                } else {
                                    (void) memset(currRealRow, 0, elSize);
                                }

                                currRealRow += elSize;
                                if (var->data.complex) {
                                    /* For imaginary part, fill in 0 */
                                    (void) memset(currImagRow, 0, elSize);
                                    currImagRow += elSize;
                                }
                            }
                        }
                        else
                        {
                            /* YES conversion needed
                             */
                            DTypeId dataTypeIdOriginal =
                                    var->data.dataTypeConvertInfo.dataTypeIdOriginal;
                            int_T DpSize = (int_T)((var->data.complex) ?
                                rt_GetSizeofComplexType(dataTypeIdOriginal) :
                                rt_GetSizeofDataType(dataTypeIdOriginal));

                                DTypeId dataTypeIdLoggingTo =
                                        var->data.dataTypeConvertInfo.dataTypeIdLoggingTo;

                                int bitsPerChunk = var->data.dataTypeConvertInfo.bitsPerChunk;
                                int numOfChunk =  var->data.dataTypeConvertInfo.numOfChunk;
                                unsigned int isSigned = var->data.dataTypeConvertInfo.isSigned;

                                double fracSlope = var->data.dataTypeConvertInfo.fracSlope;
                                int    fixedExp  = var->data.dataTypeConvertInfo.fixedExp;
                                double bias      = var->data.dataTypeConvertInfo.bias;

                                double curRealValue = -0.12345678987654;
                                double curImagValue = -0.12345678987654;

                                int_T adjIndexIfComplex = (var->data.complex) ? 2 : 1;

                                if(inRange){
                                    if(numOfChunk > 1)
                                    {
                                        /* For multiword */
                                        const char *pInData = (const char *)(cData);
                                        int dtSize = bitsPerChunk*numOfChunk/8;
                                        pInData += ((i+frameSize*idx) * adjIndexIfComplex) * dtSize;

                                        curRealValue = rt_GetDblValueFromOverSizedData(pInData, bitsPerChunk, numOfChunk,
                                                isSigned, fracSlope, fixedExp, bias);
                                        if (var->data.complex) {
                                            curImagValue = rt_GetDblValueFromOverSizedData((pInData+dtSize), bitsPerChunk, numOfChunk,
                                                    isSigned, fracSlope, fixedExp, bias);
                                        }
                                    }
                                    else
                                    {
                                        /* if in range, fill in data that is converted first */
                                        switch ( dataTypeIdOriginal )
                                        {
                                            case SS_DOUBLE:
                                            {
                                                const real_T *pInData = (const real_T *)(cData + (i+frameSize*idx)* DpSize);

                                                curRealValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                if (var->data.complex) {
                                                    pInData = (const real_T *)(cData + (i+frameSize*idx)* DpSize + DpSize/2);
                                                    curImagValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                }
                                            }
                                            break;
                                            case SS_SINGLE:
                                            {
                                                const real32_T *pInData = (const real32_T *)(cData + (i+frameSize*idx)* DpSize);

                                                curRealValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                if (var->data.complex) {
                                                    pInData = (const real32_T *)(cData + (i+frameSize*idx)* DpSize + DpSize/2);
                                                    curImagValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                }
                                            }
                                            break;
                                            case SS_INT8:
                                            {
                                                const int8_T *pInData = (const int8_T *)(cData + (i+frameSize*idx)* DpSize);

                                                curRealValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                if (var->data.complex) {
                                                    pInData = (const int8_T *)(cData + (i+frameSize*idx)* DpSize + DpSize/2);
                                                    curImagValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                }
                                            }
                                            break;
                                            case SS_UINT8:
                                            {
                                                const uint8_T *pInData = (const uint8_T *)(cData + (i+frameSize*idx)* DpSize);

                                                curRealValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                if (var->data.complex) {
                                                    pInData = (const uint8_T *)(cData + (i+frameSize*idx)* DpSize + DpSize/2);
                                                    curImagValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                }
                                            }
                                            break;
                                            case SS_INT16:
                                            {
                                                const int16_T *pInData = (const int16_T *)(cData + (i+frameSize*idx)* DpSize);

                                                curRealValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                if (var->data.complex) {
                                                    pInData = (const int16_T *)(cData + (i+frameSize*idx)* DpSize + DpSize/2);
                                                    curImagValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                }
                                            }
                                            break;
                                            case SS_UINT16:
                                            {
                                                const uint16_T *pInData = (const uint16_T *)(cData + (i+frameSize*idx)* DpSize);

                                                curRealValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                if (var->data.complex) {
                                                    pInData = (const uint16_T *)(cData + (i+frameSize*idx)* DpSize + DpSize/2);
                                                    curImagValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                }
                                            }
                                            break;
                                            case SS_INT32:
                                            {
                                                const int32_T *pInData = (const int32_T *)(cData + (i+frameSize*idx)* DpSize);

                                                curRealValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                if (var->data.complex) {
                                                    pInData = (const int32_T *)(cData + (i+frameSize*idx)* DpSize + DpSize/2);
                                                    curImagValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                }
                                            }
                                            break;
                                            case SS_UINT32:
                                            {
                                                const uint32_T *pInData = (const uint32_T *)(cData + (i+frameSize*idx)* DpSize);

                                                curRealValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                if (var->data.complex) {
                                                    pInData = (const uint32_T *)(cData + (i+frameSize*idx)* DpSize + DpSize/2);
                                                    curImagValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                }
                                            }
                                            break;
                                            case SS_BOOLEAN:
                                            {
                                                const boolean_T *pInData = ((const boolean_T *)(cData));

                                                pInData += (i+frameSize*idx) * adjIndexIfComplex;

                                                curRealValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                if (var->data.complex) {
                                                    curImagValue = ldexp( fracSlope * (double)(*pInData), fixedExp ) + bias;
                                                }
                                            }
                                            break;
                                            default:
                                            {
                                                /* For biglong */
                                                const char *pInData = (const char *)(cData);
                                                int dtSize = bitsPerChunk*numOfChunk/8;
                                                pInData += ((i+frameSize*idx) * adjIndexIfComplex) * dtSize;

                                                curRealValue = rt_GetDblValueFromOverSizedData(pInData, bitsPerChunk, numOfChunk,
                                                        isSigned, fracSlope, fixedExp, bias);
                                                if (var->data.complex) {
                                                    curImagValue = rt_GetDblValueFromOverSizedData((pInData+dtSize), bitsPerChunk, numOfChunk,
                                                            isSigned, fracSlope, fixedExp, bias);
                                                }
                                            }
                                            break;
                                        } /* -- end of switch -- */
                                    }
                                } else {
                                    /* if out of range, just fill NaN or 0 */
                                    if(dTypeID == SS_DOUBLE || dTypeID == SS_SINGLE){
                                        /* vijay 4/11/2013: DO NOT CALL ldexp() with NaN below as it causes
                                         * lcc-win64 to generate inf instead of NaN as output.
                                         * Just use rtNaN directly */
                                        curRealValue = rtNaN;
                                    }
                                    else{
                                        curRealValue = ldexp( 0, fixedExp ) + bias;
                                    }
                                    if (var->data.complex) {
                                        /* fill 0 in imaginary part*/
                                        curImagValue = ldexp( 0, fixedExp ) + bias;
                                    }
                                }

                                switch ( dataTypeIdLoggingTo )
                                {
                                    case SS_DOUBLE:
                                    {
                                        *((real_T *)currRealRow) = (real_T)curRealValue;

                                        if (var->data.complex) {

                                            *((real_T *)currImagRow) = (real_T)curImagValue;
                                        }
                                    }
                                    break;
                                    case SS_SINGLE:
                                    {
                                        *((real32_T *)currRealRow) = (real32_T)curRealValue;

                                        if (var->data.complex) {

                                            *((real32_T *)currImagRow) = (real32_T)curImagValue;
                                        }
                                    }
                                    break;
                                    case SS_INT8:
                                    {
                                        *((int8_T *)currRealRow) = (int8_T)curRealValue;

                                        if (var->data.complex) {

                                            *((int8_T *)currImagRow) = (int8_T)curImagValue;
                                        }
                                    }
                                    break;
                                    case SS_UINT8:
                                    {
                                        *((uint8_T *)currRealRow) = (uint8_T)curRealValue;

                                        if (var->data.complex) {

                                            *((uint8_T *)currImagRow) = (uint8_T)curImagValue;
                                        }
                                    }
                                    break;
                                    case SS_INT16:
                                    {
                                        *((int16_T *)currRealRow) = (int16_T)curRealValue;

                                        if (var->data.complex) {

                                            *((int16_T *)currImagRow) = (int16_T)curImagValue;
                                        }
                                    }
                                    break;
                                    case SS_UINT16:
                                    {
                                        *((uint16_T *)currRealRow) = (uint16_T)curRealValue;

                                        if (var->data.complex) {

                                            *((uint16_T *)currImagRow) = (uint16_T)curImagValue;
                                        }
                                    }
                                    break;
                                    case SS_INT32:
                                    {
                                        *((int32_T *)currRealRow) = (int32_T)curRealValue;

                                        if (var->data.complex) {

                                            *((int32_T *)currImagRow) = (int32_T)curImagValue;
                                        }
                                    }
                                    break;
                                    case SS_UINT32:
                                    {
                                        *((uint32_T *)currRealRow) = (uint32_T)curRealValue;

                                        if (var->data.complex) {

                                            *((uint32_T *)currImagRow) = (uint32_T)curImagValue;
                                        }
                                    }
                                    break;
                                    case SS_BOOLEAN:
                                    {
                                        *((boolean_T *)currRealRow) = (boolean_T)(curRealValue != 0.0);

                                        if (var->data.complex) {

                                            *((boolean_T *)currImagRow) = (boolean_T)(curImagValue != 0.0);
                                        }
                                    }
                                    break;
                                } /* -- end of switch -- */

                                currRealRow += elSize;
                                if (var->data.complex) {
                                    currImagRow += elSize;
                                }
                        }
                    }

                    if(isVarDims){ /* update "valueDimensions" field */
                        for(j = 0; j < logWidth_valDims; j ++){
                            int32_T currDimsVal=0;
                            switch (currDimsSizePtr[j]) {
                                case 1:
                                    currDimsVal = (**(((const uint8_T * const *) currDimsPtr)+j));
                                    break;
                                case 2:
                                    currDimsVal = (**(((const uint16_T * const *) currDimsPtr)+j));
                                    break;
                                case 4:
                                    currDimsVal = (**(((const uint32_T * const *) currDimsPtr)+j));
                                    break;
                            }
                            offset_valDims  = (size_t)(elSize_valDims *( var->rowIdx + nRows_valDims * j));
                            currValDimsRow  = ((char_T*) (var->valDims->dimsData)) + offset_valDims;

                            /* convert int_T to real_T */
                            currentSigDims = (real_T) currDimsVal;
                            (void) memcpy(currValDimsRow, &currentSigDims, elSize_valDims);
                            currValDimsRow += elSize_valDims;
                        }
                    }

                    ++var->rowIdx;
            }
        }
        return;

    } /* end rt_UpdateLogVar */


#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_UpdateStructLogVar =============================================
     * Abstract:
     *      Called to log data for a structure log variable.
     */
    void rt_UpdateStructLogVar(StructLogVar *var, const real_T *t, const void *data)
    {
        if(bStopLogging ==0)
        {
            LogVar       *values = var->signals.values;
            const char_T *signal = data;
            boolean_T *isVarDims = var->signals.isVarDims;
            int i = 0;

            /* time */
            if (var->logTime) {
                rt_UpdateLogVar(var->time, t, false);
            }

            /* signals */
            while (values) {
                size_t elSz = values->data.elSize;

                rt_UpdateLogVar(values, signal, isVarDims[i]);

                if (values->data.complex) elSz *= 2;
                signal += elSz * values->data.nCols;

                values = values->next;
                i++;
            }
        }
    } /* end rt_UpdateStructLogVar */


#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_UpdateTXYLogVars ===============================================
     * Abstract:
     *  Update the xFinal,T,X,Y variables that are being logged.
     */
    const char_T *rt_UpdateTXYLogVars(RTWLogInfo *li, time_T *tPtr)
    {
        if(bStopLogging==0)
        {
            return rt_UpdateTXXFYLogVars(li, tPtr, true);
        }
        else
            return(NULL);
    }

    /* Function: rt_UpdateTXXFYLogVars =============================================
     * Abstract:
     *  Update xFinal and/or the T,X,Y variables that are being logged
     */
    const char_T *rt_UpdateTXXFYLogVars(RTWLogInfo *li, time_T *tPtr, boolean_T updateTXY)
    {
        LogInfo *logInfo     = rtliGetLogInfo(li);
        int_T   matrixFormat = (rtliGetLogFormat(li) == 0);

        /* time */
        if (logInfo->t != NULL && updateTXY) {
            rt_UpdateLogVar(logInfo->t, tPtr, false);
        }

        if (matrixFormat) {                                      /* MATRIX_FORMAT */
            /* states */
            if (logInfo->x != NULL || logInfo->xFinal != NULL) {
                const RTWLogSignalInfo *xInfo      = rtliGetLogXSignalInfo(li);
                LogSignalPtrsType      segAddr     = rtliGetLogXSignalPtrs(li);
                const int_T            *segLengths = xInfo->numCols;
                int_T                  nSegments   = xInfo->numSignals;
                if (logInfo->x != NULL && updateTXY) {
                    rt_UpdateLogVarWithDiscontiguousData(logInfo->x, segAddr,
                            segLengths, nSegments);
                }
                if (logInfo->xFinal != NULL) {
                    rt_UpdateLogVarWithDiscontiguousData(logInfo->xFinal, segAddr,
                            segLengths, nSegments);
                }
            }
            /* outputs */
            if (logInfo->y != NULL && updateTXY) {
                LogVar **var = (LogVar**) (logInfo->y);
                int_T  ny    = logInfo->ny;
                int_T  i;
                int    yIdx;
                LogSignalPtrsType data = rtliGetLogYSignalPtrs(li);

                for (i = 0, yIdx = 0; i < ny; i++) {
                    if (data[i] != NULL) {
                        /*
                         * When outputs are logged in Matrix format,
                         * no variable-size signal logging is allowed.
                         */
                        rt_UpdateLogVar(var[yIdx++], data[i], false);
                    }
                }
            }
        } else {                                              /* STRUCTURE_FORMAT */
            /* states */
            if (logInfo->x != NULL && updateTXY) {
                int_T             i;
                StructLogVar      *var = logInfo->x;
                LogVar            *val = var->signals.values;
                int_T             nsig = var->signals.numSignals;
                LogSignalPtrsType data = rtliGetLogXSignalPtrs(li);

                /* time */
                if (var->logTime) {
                    rt_UpdateLogVar(var->time, tPtr, false);
                }

                /* signals */
                for (i = 0; i < nsig; i++) {
                    rt_UpdateLogVar(val, data[i], false);
                    val = val->next;
                }
            }

            /* outputs */
            if (logInfo->y != NULL && updateTXY) {
                int_T             ny      = logInfo->ny;
                LogSignalPtrsType data    = rtliGetLogYSignalPtrs(li);
                StructLogVar      **var   = (StructLogVar**) (logInfo->y);

                if (ny == 1) {
                    int_T  i;
                    int_T  dataIdx;
                    LogVar *val = var[0]->signals.values;
                    int_T  nsig = var[0]->signals.numSignals;
                    boolean_T   *isVarDims = var[0]->signals.isVarDims;

                    /* time */
                    if (var[0]->logTime) {
                        rt_UpdateLogVar(var[0]->time, tPtr, false);
                    }

                    /* signals */
                    for (i = 0, dataIdx = 0; i < nsig; i++) {
                        while (data[dataIdx] == NULL) {
                            ++dataIdx;
                        }
                        rt_UpdateLogVar(val,  data[dataIdx], isVarDims[i]);
                        val = val->next;
                        ++dataIdx;
                    }
                } else {
                    int_T  i;
                    int_T  dataIdx;

                    for (i = 0, dataIdx = 0; i < ny && var[i] != NULL; i++) {
                        LogVar *val = var[i]->signals.values;
                        boolean_T   *isVarDims = var[i]->signals.isVarDims;

                        /* time */
                        if (var[i]->logTime) {
                            rt_UpdateLogVar(var[i]->time, tPtr, false);
                        }

                        /* signals */
                        while (data[dataIdx] == NULL) {
                            ++dataIdx;
                        }
                        rt_UpdateLogVar(val,  data[dataIdx], isVarDims[0]);
                        val = val->next;
                        ++dataIdx;
                    }
                }
            }
            /* final state */
            if (logInfo->xFinal != NULL) {
                StructLogVar *xf  = logInfo->xFinal;
                LogVar       *val = xf->signals.values;
                int_T        nsig = xf->signals.numSignals;
                int_T        i;

                /* time */
                if (xf->logTime) {
                    rt_UpdateLogVar(xf->time, tPtr, false);
                }

                /* signals */
                for (i = 0; i < nsig; i++) {
                    LogSignalPtrsType data = rtliGetLogXSignalPtrs(li);

                    rt_UpdateLogVar(val, data[i], false);
                    val = val->next;
                }
            }
        }
        return(NULL);
    } /* end rt_UpdateTXXFYLogVars */


#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_StopDataLoggingImpl =======================================
     * Abstract:
     *  Write logged data to model.mat and free memory.
     */
    void rt_StopDataLoggingImpl(const char_T *file, RTWLogInfo *li, boolean_T isRaccel)
    {
        FILE          *fptr;
        LogInfo       *logInfo     = (LogInfo*) rtliGetLogInfo(li);
        LogVar        *var         = logInfo->logVarsList;
        StructLogVar  *svar        = logInfo->structLogVarsList;
        /* At this time, verbose is only needed if running rapid accelerator
         * simulations. */
        int           verbose      = isRaccel ? 0: 1;
        //static void *origLocation=NULL;
        boolean_T     emptyFile    = 1; /* assume */
        boolean_T     errFlag      = 0;
        const char_T  *msg;

        /*******************************
         * Create MAT file with header *
         *******************************/
        if ((fptr=MW_fopen(file,"w+b")) == NULL) {
            bStopLogging = 1;
            goto EXIT_POINT;
        }
        checkFP = fptr;
        if (rt_WriteMat5FileHeader(fptr)) {
            bStopLogging = 1;
            goto EXIT_POINT;
        }

        /**************************************************
         * First log all the variables in the LogVar list *
         **************************************************/
        while (var != NULL) {
            if ( (msg = rt_FixupLogVar(var,verbose)) != NULL ) {
                //(void)fprintf(stderr,"*** Error writing %s due to: %s\n",file,msg);
                errFlag = 1;
                bStopLogging = 1;
                break;
            }
            if (var->nDataPoints > 0 || isRaccel) {
                MatItem item;
                MatrixData  tempData;

                item.type   = matMATRIX;
                item.nbytes = 0; /* not yet known */

                dataSizeToWrite = var->nDataPoints;
                wrappedSize = var->wrapped;
                /*if(var->wrapped > var->data.nRows/2)
                 * {
                 * origLocation = var->data.re;
                 * var->data.re = ((char_T *)var->data.re + ((var->data.elSize * var->data.nRows * var->data.nCols) /2));
                 * }
                 * else if(origLocation != NULL)
                 * {
                 * var->data.re = origLocation;
                 * }*/
                (void)memcpy(tempData.name, var->data.name, mxMAXNAM);
                tempData.nRows = dataSizeToWrite; //values->valDims->nRows;
                tempData.nCols = var->data.nCols;
                tempData.nDims = var->data.nDims;
                tempData.dims =  var->data.dims;
                //tempData._dims = var->data._dims;
                tempData._dims[0] = var->data._dims[0];
                tempData._dims[1] = var->data._dims[1];
                if(wrappedSize < var->data.nRows/2)
                {
                    tempData.re = var->data.re;
                }
                else
                {
                    tempData.re = var->data.re + ((var->data.elSize * (var->data.nRows/2) * var->data.nCols) );
                }
                //item.nbytes = (int32_T)(dataSizeToWrite * var->nCols * elSize);
                tempData.im = var->data.im;
                tempData.dTypeID = var->data.dTypeID;
                tempData.elSize =  var->data.elSize;
                tempData.mxID = var->data.mxID;
                tempData.logical = var->data.logical;
                tempData.complex = var->data.complex;
                tempData.frameData = var->data.frameData;
                tempData.frameSize = var->data.frameSize;

                item.data = &tempData; /*var->valDims;*/
                //item.data   = &(var->data);

                if (rt_WriteItemToMatFile(fptr, &item, MATRIX_ITEM)) {
                   /* (void)fprintf(stderr,"*** Error writing log variable %s to "
                            "file %s",var->data.name, file);*/
                    errFlag = 1;
                    bStopLogging = 1;
                    break;
                }
                emptyFile = 0;
                //var->wrapped = (var->wrapped >= (var->data.nRows/2))? 0 : (var->data.nRows/2);
                if((var->wrapped == -1) || (var->wrapped == -2) ||(var->wrapped ==  (var->data.nRows+1)))
                {
                    var->wrapped = -2;
                }
                else
                {
                    var->wrapped = (var->wrapped >= (var->data.nRows/2))? -1 : (var->data.nRows+1);
                }
            }
            else{
                var->wrapped = -2;
            }
            var = var->next;
        }

        /*******************************************************
         * Next log all the variables in the StructLogVar list *
         *******************************************************/
        while (svar != NULL) {
            MatItem item;

            if (svar->logTime) {
                var = svar->time;
                if ( (msg = rt_FixupLogVar(var,verbose)) != NULL ) {
                    /*(void)fprintf(stderr, "*** Error writing %s due to: %s\n",
                            file, msg);*/
                    errFlag = 1;
                    bStopLogging = 1;
                    break;
                }
                //var->wrapped = (var->wrapped >= (var->data.nRows/2))? 0 : (var->data.nRows/2);
                if((var->wrapped == -1) || (var->wrapped == -2) || (var->wrapped ==  (var->data.nRows+1)))
                {
                    var->wrapped = -2;
                }
                else
                {
                    var->wrapped = (var->wrapped >= (var->data.nRows/2))? -1 : (var->data.nRows+1);
                }
            }

            var = svar->signals.values;
            while (var) {
                if ( (msg = rt_FixupLogVar(var,verbose)) != NULL ) {
                    /*(void)fprintf(stderr, "*** Error writing %s due to: %s\n",
                            file, msg);*/
                    errFlag = 1;
                    bStopLogging = 1;
                    break;
                }

                dataSizeToWrite = var->nDataPoints;
                wrappedSize = var->wrapped;
                //var->wrapped = (var->wrapped >= (var->data.nRows/2))? 0 : (var->data.nRows/2);
                if((var->wrapped == -1) || (var->wrapped == -2) ||(var->wrapped ==  (var->data.nRows+1)))
                {
                    var->wrapped = -2;
                }
                else
                {
                    var->wrapped = (var->wrapped >= (var->data.nRows/2))? -1 : (var->data.nRows+1);
                }
                var = var->next;
            }

            item.type   = matMATRIX;
            item.nbytes = 0; /* not yet known */

            item.data   = svar;

            if (rt_WriteItemToMatFile(fptr, &item, STRUCT_LOG_VAR_ITEM)) {
                /*(void)fprintf(stderr,"*** Error writing structure log variable "
                        "%s to file %s",svar->name, file);*/
                errFlag = 1;
                bStopLogging = 1;
                break;
            }
            emptyFile = 0;

            svar = svar->next;
        }

        /******************
         * Close the file *
         ******************/
        (void)MW_fclose(fptr);
        if (emptyFile || errFlag) {
            (void)MW_fremove(file);
        } else {
            if( verbose ) {
                //(void)printf("** created %s **\n\n", file);
            }
        }

        EXIT_POINT:

                        /****************
                         * free logInfo *
                         ****************/
                        if(bStopLogging ==1)
                        {
                            rt_DestroyLogVar(logInfo->logVarsList);
                            logInfo->logVarsList = NULL;
                            rt_DestroyStructLogVar(logInfo->structLogVarsList);
                            logInfo->structLogVarsList = NULL;
                            FREE(logInfo);
                            rtliSetLogInfo(li,NULL);
                        }
    } /* end rt_StopDataLoggingImpl */


#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif


    /* Function: rt_StopDataLogging ================================================
     * Abstract:
     *  Write logged data to model.mat and free memory.
     */
    void rt_StopDataLogging(const char_T *file, RTWLogInfo *li)
    {
        int8_T newfilename[256],numstr_file[256],numstr_run[256],extn[10]=".mat";
        uint32_T len=0;
        static uint32_T file_index=1;
        static uint32_T run_index=1;
        static uint8_T firstTime =1;
        static int8_T filename[256];


        FILE *result;

        if((stopRequested == 1) && (bStopLogging ==0))
        {
            bDataReady =1;
        }

        if(bDataReady ==1)
        {
            if (firstTime)
            {
                /* Remove the .mat from filename */
                len = strlen(file)-4;
                /* memset the filename holders to 0 */
                memset(newfilename,0,256);
                memset(filename,0,256);
                /* Prepend pathName */
                memcpy(filename,file,len);
                /* make a copy of the filename */
                memcpy(newfilename,filename,strlen((const char *)filename));
                sprintf((char *)numstr_file,"%d",file_index);
                sprintf((char *)numstr_run,"_%d_",run_index);
                strcat((char *)newfilename,(char *)numstr_run);
                strcat((char *)newfilename,(char *)numstr_file);
                strcat((char *)newfilename,(char *)extn);

                while((result=MW_fopen((const char *)newfilename,"r")))
                {
                    MW_fclose(result);
                    run_index++;
                    memset(newfilename,0,strlen((const char *)newfilename));
                    memcpy(newfilename,file,len);
                    sprintf((char *)numstr_file,"%d",file_index);
                    sprintf((char *)numstr_run,"_%d_",run_index);
                    strcat((char *)newfilename,(char *)numstr_run);
                    strcat((char *)newfilename,(char *)numstr_file);
                    strcat((char *)newfilename,(char *)extn);
                }
                firstTime = 0;
            }
            else
            {
                file_index++;
                memset(newfilename,0,256);
                memcpy(newfilename,filename,strlen((const char *)filename));
                sprintf((char *)numstr_file,"%d",file_index);
                sprintf((char *)numstr_run,"_%d_",run_index);
                strcat((char *)newfilename,(char *)numstr_run);
                strcat((char *)newfilename,(char *)numstr_file);
                strcat((char *)newfilename,(char *)extn);
            }
            rt_StopDataLoggingImpl((char *)newfilename,li,false);
            bDataReady =0;
            if((stopRequested == 1) && (bStopLogging ==0))
        {
            bStopLogging = 1;
        }
        }
    } /* end rt_StopDataLogging */


#ifdef __cplusplus
}
#endif

#else /*!defined(MAT_FILE) || (defined(MAT_FILE) && MAT_FILE == 1)*/

#define rt_StartDataLogging(li, finalTime, stepSize, errStatus) NULL /* do nothing */
#define rt_UpdateTXYLogVars(li, tPtr) NULL /* do nothing */
#define rt_StopDataLogging(file, li); /* do nothing */

#endif /*!defined(MAT_FILE) || (defined(MAT_FILE) && MAT_FILE == 1)*/



/* [eof] rt_logging.c */

/* LocalWords:  Tfinal MAXNAM nonfinite DType PWS RSim Fixup logvar DDEFAULT th
 * LocalWords:  curr Realloc realloc inp biglong vijay ldexp TXY eof XFinal th
 * LocalWords:  TXXFY NULL
 */
