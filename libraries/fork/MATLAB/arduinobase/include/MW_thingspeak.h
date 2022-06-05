/*
 * ThingSpeak APIs
 *
 * Copyright 2015 The MathWorks, Inc. */

#if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
/* This will be run in Rapid Accelerator Mode */

    #define MW_initThingSpeak(  ) (0)
    #define MW_getCurrentTimeInMillis() (0)
    #define MW_addField(a, b) (0)
    #define MW_addLocation(a) (0)
    #define MW_addStatus(a) (0)
    #define MW_postThingSpeak(a, b, c) (0)
    #define MW_updatelastConnected() (0)
    #define MW_readFloatField(a, b,c,d,e) (0)
    #define MW_lastreadStatus() (0)
#else
        #ifndef _MW_THINGSPEAK_H_
        #define _MW_THINGSPEAK_H_
        #include <inttypes.h>
        #include "rtwtypes.h"

        #ifdef __cplusplus
        extern "C" {
        #endif

        void MW_initThingSpeak( );
        uint32_T MW_getCurrentTimeInMillis(void);
        void MW_addField(const real_T data, const uint8_T fieldNo);
        void MW_addLocation(const real_T *location);
        void MW_addStatus(const char *statusMessage);
        void MW_postThingSpeak(unsigned long ChannelNumber,
                const char *writeAPIKey,
                const boolean_T printDiagnosticMessages);
        void MW_updatelastConnected();
        void MW_readFloatField(unsigned long channelNumber, unsigned int fieldNumber, const char * readAPIKey, float *data, int16_t *status);
        int16_t MW_lastreadStatus(void);

        #ifdef __cplusplus
        }
        #endif
        #endif //_MW_THINGSPEAK_H_

#endif
