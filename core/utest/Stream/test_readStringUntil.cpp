/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing 'readStringUntil' with separator available within the stream", "[Stream-readStringUntil-01]")
{
  StreamMock mock;
  mock.setTimeout(10);
  mock << "This is test! lorem ipsum lalala";

  REQUIRE(mock.readStringUntil('!') == String("This is test"));
}

TEST_CASE ("Testing 'readStringUntil' with separator not available within the stream", "[Stream-readStringUntil-02]")
{
  StreamMock mock;
  mock.setTimeout(10);
  mock << "This is test ... lorem ipsum lalala";

  REQUIRE(mock.readStringUntil('!') == String("This is test ... lorem ipsum lalala"));
}
