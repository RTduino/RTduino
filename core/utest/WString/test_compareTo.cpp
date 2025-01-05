/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::compareTo(const String &)", "[String-compareTo-01]")
{
  WHEN ("Strings are equal")
  {
    String str1("Hello"), str2("Hello");
    REQUIRE(str1.compareTo(str2) == 0);
  }

  WHEN ("str2 is empty")
  {
    String str1("Hello"), str2;
    REQUIRE(str1.compareTo(str2) > 0);
  }

  WHEN ("str1 is empty")
  {
    String str1, str2("Hello");
    REQUIRE(str1.compareTo(str2) < 0);
  }
}

TEST_CASE ("Testing String::compareTo(const char *)", "[String-compareTo-02]")
{
  WHEN ("Strings are equal")
  {
    String str("Hello");
    REQUIRE(str.compareTo("Hello") == 0);
  }

  WHEN ("Passed string is empty")
  {
    String str("Hello");
    REQUIRE(str.compareTo("") > 0);
  }

  WHEN ("Passed string is compared with empty string")
  {
    String str;
    REQUIRE(str.compareTo("") == 0);
  }
}

TEST_CASE ("Testing String::compareTo(const char *) with empty buffer", "[String-compareTo-03]")
{
  WHEN ("First string has a valid buffer")
  {
    char *buffer = NULL;

    String str("Hello");
    REQUIRE(str.compareTo(buffer) != 0);
  }

  WHEN ("First string does NOT have a valid buffer")
  {
    char *buffer1 = NULL;
    char *buffer2 = NULL;

    String str(buffer1);
    REQUIRE(str.compareTo(buffer2) == 0);
  }
}


TEST_CASE ("Testing String::compareTo(const String &) with empty buffer", "[String-compareTo-04]")
{
  WHEN ("First string has a valid buffer")
  {
    char *buffer = NULL;

    String str1("Hello");
    String str2(buffer);
    REQUIRE(str1.compareTo(str2) != 0);
  }

  WHEN ("First string does NOT have a valid buffer")
  {
    char *buffer1 = NULL;
    char *buffer2 = NULL;

    String str1(buffer1);
    String str2(buffer2);
    REQUIRE(str1.compareTo(str2) == 0);
  }
}
