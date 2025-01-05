/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Clear write error should set the error code back to 0", "[Print-clearWriteError-01]")
{
  PrintMock mock;
  mock.mock_setWriteError(5);
  mock.clearWriteError();
  REQUIRE(mock.getWriteError() == 0);
}
