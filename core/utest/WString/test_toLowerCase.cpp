/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::toLowerCase", "[String-toLowerCase-01]")
{
  String str("HELLO ARDUINO");
  str.toLowerCase();
  REQUIRE(str == "hello arduino");
}
