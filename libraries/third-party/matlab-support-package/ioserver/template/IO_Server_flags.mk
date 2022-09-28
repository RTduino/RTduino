# Copyright 2017-2018 The MathWorks, Inc.

SUPPORT_PACKAGE_DIR = 
PERIPHERALS_DIR = 
IO_SERVER_INFRASTRUCTURE_DIR = 

IO_SERVER_INCLUDEPATHS = -I$(IO_SERVER_INFRASTRUCTURE_DIR)/ioserver/inc -I$(PERIPHERALS_DIR)/inc -I$(SUPPORT_PACKAGE_DIR)\include -I$(SUPPORT_PACKAGE_DIR)\..\..\shared\svd\include
IO_SERVER_SOURCEFILES = $(IO_SERVER_INFRASTRUCTURE_DIR)/ioserver/src/IO_utilities.c $(IO_SERVER_INFRASTRUCTURE_DIR)/ioserver/src/IO_packet.c $(IO_SERVER_INFRASTRUCTURE_DIR)/ioserver/src/IO_server.c $(IO_SERVER_INFRASTRUCTURE_DIR)/ioserver/src/IO_standardperipherals.c $(IO_SERVER_INFRASTRUCTURE_DIR)/ioserver/src/IO_wrapperDigitalIO.c $(IO_SERVER_INFRASTRUCTURE_DIR)/ioserver/src/IO_wrapperI2C.c $(IO_SERVER_INFRASTRUCTURE_DIR)/ioserver/src/IO_wrapperPWM.c $(IO_SERVER_INFRASTRUCTURE_DIR)/ioserver/src/IO_wrapperSPI.c $(IO_SERVER_INFRASTRUCTURE_DIR)/ioserver/src/PeripheralToHandle.c
IO_SERVER_OBJECTS = IO_utilities.o IO_packet.o IO_server.o IO_wrapperDigitalIO.o IO_standardperipherals.o IO_wrapperI2C.o IO_wrapperPWM.o IO_wrapperSPI.o PeripheralToHandle.o
