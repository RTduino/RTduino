/*
 * Copyright (c) 2023 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::isEmpty when string is empty", "[String-isEmpty-01]")
{
  String str;
  REQUIRE(str.isEmpty());
}

TEST_CASE ("Testing String::isEmpty when string contains characters", "[String-isEmpty-02]")
{
  String str("Testing String::isEmpty");
  REQUIRE(!str.isEmpty());
}
