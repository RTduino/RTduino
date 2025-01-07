/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Verifying if default timeout is returned correctly", "[Stream-getTimeout-01]")
{
  StreamMock mock;
  REQUIRE(mock.getTimeout() == 1000);
}
