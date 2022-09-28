#ifndef _MW_SERIALREAD_H_
#define _MW_SERIALREAD_H_
#include <inttypes.h>
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER))
 /* This will be run in Rapid Accelerator Mode */
 #define MW_Serial_read(a, b, c, d)  (0)
#else
    void MW_Serial_read(const uint8_T port,const uint16_t DataSizeInBytes, char *data, uint8_T *status);
#endif

#ifdef __cplusplus
}
#endif
#endif //_MW_SERIALREAD_H_
