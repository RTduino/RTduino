/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::toInt when string is empty", "[String-toInt-01]")
{
  String str;
  int const val = str.toInt();
  REQUIRE(val == 0);
}

TEST_CASE ("Testing String::toInt when string contains no number", "[String-toInt-02]")
{
  String str("abc");
  int const val = str.toInt();
  REQUIRE(val == 0);
}

TEST_CASE ("Testing String::toInt when string contains a number", "[String-toInt-03]")
{
  {
    String str("-1");
    int const val = str.toInt();
    REQUIRE(val == -1);
  }
  {
    String str("1");
    int const val = str.toInt();
    REQUIRE(val == 1);
  }
}

TEST_CASE ("Testing String::toInt when string does not have a buffer", "[String-toInt-04]")
{
  char *buffer = NULL;
  String str(buffer);
  REQUIRE(str.toInt() == 0);
}
