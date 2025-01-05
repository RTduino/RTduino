/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "../catch_compatible.h"

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::indexOf(char ch)", "[String-indexOf-01]")
{
  WHEN ("str is empty")
  {
    String str;
    REQUIRE(str.indexOf('a') == -1);
  }
  WHEN ("str does not contained searched element")
  {
    String str("Hello");
    REQUIRE(str.indexOf('a') == -1);
  }
  WHEN ("str does contain searched element")
  {
    String str("Hello");
    REQUIRE(str.indexOf('l') == 2);
  }
}

TEST_CASE ("Testing String::indexOf(char ch, unsigned int fromIndex)", "[String-indexOf-02]")
{
  WHEN ("str is empty")
  {
    String str;
    REQUIRE(str.indexOf('a', 5) == -1);
  }
  WHEN ("str does not contained searched element")
  {
    String str("Hallo");
    REQUIRE(str.indexOf('a', 3) == -1);
  }
  WHEN ("str does contain searched element")
  {
    String str("Hello");
    REQUIRE(str.indexOf('l', 3) == 3);
  }
}

TEST_CASE ("Testing String::indexOf(const String &)", "[String-indexOf-03]")
{
  String const search_str("Arduino");

  WHEN ("str is empty")
  {
    String str;
    REQUIRE(str.indexOf(search_str) == -1);
  }
  WHEN ("str does not contained searched element")
  {
    String str("Hallo");
    REQUIRE(str.indexOf(search_str) == -1);
  }
  WHEN ("str does contain searched element")
  {
    String str("Hello Arduino!");
    REQUIRE(str.indexOf(search_str) == 6);
  }
}

TEST_CASE ("Testing String::indexOf(const String &, unsigned int fromIndex)", "[String-indexOf-04]")
{
  String const search_str("Arduino");

  WHEN ("str is empty")
  {
    String str;
    REQUIRE(str.indexOf(search_str, 3) == -1);
  }
  WHEN ("str does not contained searched element")
  {
    String str("Hallo");
    REQUIRE(str.indexOf(search_str, 3) == -1);
  }
  WHEN ("str does contain searched element and fromIndex is < start of searched element")
  {
    String str("Hello Arduino!");
    REQUIRE(str.indexOf(search_str, 3) == 6);
  }
  WHEN ("str does contain searched element and fromIndex is > start of searched element")
  {
    String str("Hello Arduino!");
    REQUIRE(str.indexOf(search_str, 8) == -1);
  }
}
