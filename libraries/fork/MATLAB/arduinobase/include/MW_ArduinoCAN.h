#ifndef _MW_ARDUINOCAN_H_
#define _MW_ARDUINOCAN_H_
#include <inttypes.h>
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER))
 /* This will be run in Rapid Accelerator Mode */
 #define MW_GetCANBaud(a,b,c)                       (0)
 #define MW_GetCANFilters(a,b,c,d,e,f,g,h,i,j,k)    (0)
 #define MW_GetCANMillis()                          (0)
 #define MW_GetMCPInitStatus()                      (0)
 #define MW_SetMCPInitStatus()                      (0)
 #define MW_CANInitializeInterrupt()                (0)
 #define MW_GetCANMessageWithID(a,b,c,d,e,f,g)      (0)
 #define MW_GetCANMessageNew(a,b,c,d,e,f,g)         (0)
 #define MW_CANAssignIdAndLength(a,b,c)             (0)
 #define MW_CANGetChipSelect()                      (0)
#else
    void MW_GetCANBaud(uint8_T*,uint8_T*,uint8_T*);
    void MW_GetCANFilters(uint8_T* allowAll,uint8_T* buffer0Extended,uint32_T* mask0,uint32_T* filter0,uint32_T* filter1,uint8_T* buffer1Extended,uint32_T* mask1,uint32_T* filter2,uint32_T* filter3,uint32_T* filter4,uint32_T* filter5);
    uint32_T MW_GetCANMillis(void);
    uint8_T MW_GetMCPInitStatus(void);
    void MW_SetMCPInitStatus(void);
    void MW_CANInitializeInterrupt();
    void MW_GetCANMessageWithID(uint32_T id,uint8_T* data1, uint8_T dataLength, uint8_T* status, uint8_T extended, uint8_T* remote, uint8_T* error);
    void MW_GetCANMessageNew(uint32_T* id,uint8_T* data1, uint8_T* dataLength, uint8_T* status, uint8_T* extended, uint8_T* remote, uint8_T* error);
    void MW_CANAssignIdAndLength(uint32_T id,uint8_T extended,uint8_T msgLength);
    uint8_T MW_CANGetChipSelect(void);
#endif

#ifdef __cplusplus
}
#endif
#endif //_MW_SERIALREAD_H_
