/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing 'readString' with data available within the stream", "[Stream-readString-01]")
{
  StreamMock mock;
  mock.setTimeout(10);
  mock << "This is test stream content";

  REQUIRE(mock.readString() == String("This is test stream content"));
}
