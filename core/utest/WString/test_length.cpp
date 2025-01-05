/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::length when string is empty", "[String-length-01]")
{
  String str;
  REQUIRE(str.length() == 0);
}

TEST_CASE ("Testing String::length when string contains characters", "[String-length-02]")
{
  String str("Testing String::length");
  REQUIRE(str.length() == strlen("Testing String::length"));
}
