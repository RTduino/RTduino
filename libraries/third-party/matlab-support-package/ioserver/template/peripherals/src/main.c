/* Copyright 2017 The MathWorks, Inc. */
#include "IO_include.h"
#include "IO_server.h"
#include "IO_packet.h"

static uint8_T PayloadBufferRxBackground[PAYLOAD_SIZE];
static uint8_T PayloadBufferTxBackground[PAYLOAD_SIZE];
// Hook for configuring the base rate at which the scheduler should be called and starts the scheduler
void configureScheduler(float SchedulerBaseRate)
{

}
//Hook to enable the scheduler interrupt
void enableSchedulerInterrupt(void)
{

}
//Hook to disable the scheduler interrupt
void disableSchedulerInterrupt(void)
{

}
//Hook to enable the global interrupt on the target
void enableGlobalInterrupt(void)
{

}
//Hook to disable the global interrupt on the target
void disableGlobalInterrupt(void)
{

}
//Hook to stop the scheduler
void stopScheduler(void)
{

}

void rt_OneStep(void)
{
    serverScheduler();
}

int main(void)
{

  ioServerInit();

  while (1) {
      server((uint8_T*)&PayloadBufferRxBackground,(uint8_T*)&PayloadBufferTxBackground,1);
  }
  return 0;
}
