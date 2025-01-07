/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Verifying if calling 'setTimeout' is indeed modifying the timeout", "[Stream-setTimeout-01]")
{
  StreamMock mock;

  mock.setTimeout(100);

  REQUIRE(mock.getTimeout() == 100);
}
