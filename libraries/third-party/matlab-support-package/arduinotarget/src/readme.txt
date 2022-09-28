# Copyright 2016-2017 The MathWorks, Inc.
This file has two sections.

Section 1 : supported targets for the daemon external mode
Section 2 : rtiostream_interface_arduino.c and rtiostream_serial_daemon.cpp



Section 1
==================================================================================================================================
External mode with daemon supports only non-Leonardo based Arduino. For clarity all supported boards are listed below
	1. Uno
	2. Mega ADK
	3. Mega 2560
	4. Due
	6. Nano30



Section 2
===================================================================================================================================
The new external mode now uses the modified versions of the following files for the supported targets listed above
	1. rtiostream_interface.c	->	rtiostream_interface_arduino.c
	2. rtiostream_serial.c		->	rtiostream_serial_daemon.cpp