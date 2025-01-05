/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Print::availableForWrite() should return 0 if not overwritten by derived class", "[Print-availableForWrite-01]")
{
  PrintMock mock;
  REQUIRE(mock.availableForWrite() == 0);
}
