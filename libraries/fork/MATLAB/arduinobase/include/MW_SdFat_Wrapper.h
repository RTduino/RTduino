#ifndef _MWSDFATWRAPPER_H_
#define _MWSDFATWRAPPER_H_

#include <inttypes.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER))
#define MW_SD_fopen(a,b,c,d) (0)
#define MW_SD_fwrite(a,b,c,d,e) (0)
#define MW_SD_fread(a,b,c,d,e) (0)
#define MW_SD_fread_delim(a,b,c,d) (0)
#define MW_SD_frewind(a,b) (0)
#define MW_SD_fclose(a,b) (0)
#define MW_SD_fremove(a) (0)
#define MW_SD_fseekset(a, b) (0)
#define MW_SD_fseekcur(a, b) (0)
#define MW_SD_fcurpos(a) (0)
#define MW_SD_favailable(a) (0)
#define MW_SD_audio_favailable(a,b) (0)
#define MW_SD_strtol(a) (0)
#define MW_SD_strtoul(a) (0)
#define MW_SD_strtod(a) (0)
#else
void* MW_SD_fopen(const char *filename, const char *mode, uint8_t *FileID, uint8_t *alreadyOpen);
size_t MW_SD_fwrite(const void *ptr, size_t size, size_t nmemb, void *stream, uint8_t FileID);
int MW_SD_fread(void *ptr, size_t size, size_t nmemb, void *stream, uint8_t FileID);
int MW_SD_fread_delim(char *ptr, char delim, uint8_t FileID, uint8_t floatStatus);
void MW_SD_frewind(void *stream,uint8_t FileID);
int MW_SD_fclose(void *stream, uint8_t FileID);
int MW_SD_fremove(const char *filename);
int MW_SD_fseekset(uint8_t FileID, uint32_t pos);
int MW_SD_fseekcur(uint8_t FileID, int pos);
uint32_t MW_SD_fcurpos(uint8_t FileID);
uint32_t MW_SD_favailable(uint8_t FileID);
uint32_t MW_SD_audio_favailable(uint8_t FileID, uint8_t *headerEnd);
long MW_SD_strtol(char *ptr);
unsigned long MW_SD_strtoul(char *ptr);
double MW_SD_strtod(char *ptr);
#endif /*Rapid Accel*/

#ifdef __cplusplus
}
#endif

#endif //_MWSDFATWRAPPER_H_
