#include "rtwtypes.h"

#define ANALOG_MODULES_MAX 70  /* For Mega and Due. Other boards have less number of Modules */
#define PWM_MODULES_MAX 70  /* For Mega and Due. Other boards have less number of Modules */

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_T PeripheralHandleMapType;
PeripheralHandleMapType analogPinHandleMap[(ANALOG_MODULES_MAX/8)+1] = {0};
PeripheralHandleMapType PWMPinHandleMap[(PWM_MODULES_MAX/8)+1] = {0};

#ifdef __cplusplus
}
#endif
