/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#ifndef PRINT_MOCK_H_
#define PRINT_MOCK_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <WString.h>

#include <Print.h>

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class PrintMock : public Print
{
public:
  String _str;
  void flush() { _str = String(); }
  size_t write(uint8_t b) {  _str += static_cast<char>(b); return 1; };
  void mock_setWriteError() { setWriteError(); }
  void mock_setWriteError(int err) { setWriteError(err); }
};

#endif /* PRINT_MOCK_H_ */
