/* Copyright 2015-2018 The MathWorks, Inc. */
#ifndef __MW_ETH_H__
#define __MW_ETH_H__
#include "MW_SVD.h"
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _MW_ETH_Status_Type {
    MW_ETH_SUCCESS           = 0x00,
    MW_ETH_BUSY              = 0x01,
    MW_ETH_BUS_ERROR         = 0x02,
    MW_ETH_BUS_NOT_IN_USE    = 0x04,
    MW_ETH_BUS_NOT_AVAILABLE = 0x08
} MW_ETH_Status_Type;



MW_Handle_Type MW_ETH_Open(uint8_T SendReceiveFlag,uint8_T isClient,uint32_T LocalIPPort,uint32_T ServerPort,char_T *RemoteIPAddress);

MW_ETH_Status_Type MW_ETH_Send(MW_Handle_Type ETHHandle,  void *data, uint32_T length,uint32_T BlockingMode,  double blockingTimeout);

MW_ETH_Status_Type MW_ETH_Receive(MW_Handle_Type ETHHandle,  void *data, uint32_T length,uint32_T BlockingMode,  double blockingTimeout);

void MW_ETH_Close(MW_Handle_Type ETHHandle);

MW_ETH_Status_Type MW_ETH_connect(MW_Handle_Type ETHHandle, uint32_T ServerPort);

void MW_ETH_accept(MW_Handle_Type ETHHandle);

MW_ETH_Status_Type MW_ETH_bind(MW_Handle_Type ETHHandle,uint32_T LocalIPPort);

/* udp */

MW_Handle_Type MW_UDP_Open(uint32_T LocalIPPort,char_T *RemoteIPAddress,uint32_T RemoteIPPort);

MW_ETH_Status_Type MW_UDP_Send(MW_Handle_Type ETHHandle,  void *data, uint32_T length,uint32_T BlockingMode, double blockingTimeout);

MW_ETH_Status_Type MW_UDP_Receive(MW_Handle_Type ETHHandle,  void *data, uint32_T length,uint32_T BlockingMode, double blockingTimeout);

void MW_UDP_Close(MW_Handle_Type ETHHandle);

MW_ETH_Status_Type MW_UDP_connect(MW_Handle_Type ETHHandle,uint32_T RemoteIPPort);

MW_ETH_Status_Type MW_UDP_bind(MW_Handle_Type ETHHandle,uint32_T LocalIPPort);


#ifdef __cplusplus
}

#endif

#endif /* __MW_ETH_H__ */



