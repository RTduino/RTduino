/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("No write error has been set", "[Print-getWriteError-01]")
{
  PrintMock mock;
  REQUIRE(mock.getWriteError() == 0);
}

TEST_CASE ("A write error has been set", "[Print-getWriteError-02]")
{
  PrintMock mock;
  mock.mock_setWriteError(5);
  REQUIRE(mock.getWriteError() == 5);
}
