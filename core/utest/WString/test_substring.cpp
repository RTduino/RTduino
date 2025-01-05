/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::substring(unsigned int, unsigned int)", "[String-substring-01]")
{
  WHEN ("left higher than len")
  {
    String str("Hello");
    str.substring(7,9);
  }

  WHEN ("right higher than len")
  {
    String str1("Hello");
    String str2("ello");
    REQUIRE(str2 == str1.substring(1,9));
  }

  WHEN ("left higher than right")
  {
    String str1("Hello");
    String str2("ello");
    REQUIRE(str2 == str1.substring(9,1));
  }
}
