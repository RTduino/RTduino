/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::toUpperCase", "[String-toUpperCase-01]")
{
  String str("hello arduino");
  str.toUpperCase();
  REQUIRE(str == "HELLO ARDUINO");
}
