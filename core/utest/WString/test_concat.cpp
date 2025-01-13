/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::concat(const String &)", "[String-concat-01]")
{
  String str1("Hello "), str2("Arduino!");
  REQUIRE(str1.concat(str2) == 1);
  REQUIRE(str1 == "Hello Arduino!");
}

TEST_CASE ("Testing String::concat(const char *)", "[String-concat-02]")
{
  String str("Hello ");
  REQUIRE(str.concat("Arduino!") == 1);
  REQUIRE(str == "Hello Arduino!");
}

TEST_CASE ("Testing String::concat(char)", "[String-concat-03]")
{
  String str("Hello ");
  char const c = 'A';
  REQUIRE(str.concat(c) == 1);
  REQUIRE(str == "Hello A");
}

TEST_CASE ("Testing String::concat(unsigned char)", "[String-concat-04]")
{
  String str("Hello ");
  unsigned char const c = 'A';
  REQUIRE(str.concat(c) == 1);
  REQUIRE(str == "Hello 65"); /* ASCII['A'] = 65 */
}

TEST_CASE ("Testing String::concat(int)", "[String-concat-05]")
{
  String str("Hello ");
  int const num = -1;
  REQUIRE(str.concat(num) == 1);
  REQUIRE(str == "Hello -1");
}

TEST_CASE ("Testing String::concat(unsigned int)", "[String-concat-06]")
{
  String str("Hello ");
  unsigned int const num = 1;
  REQUIRE(str.concat(num) == 1);
  REQUIRE(str == "Hello 1");
}

TEST_CASE ("Testing String::concat(long)", "[String-concat-07]")
{
  String str("Hello ");
  long const num = -1;
  REQUIRE(str.concat(num) == 1);
  REQUIRE(str == "Hello -1");
}

TEST_CASE ("Testing String::concat(unsigned long)", "[String-concat-08]")
{
  String str("Hello ");
  unsigned long const num = 1;
  REQUIRE(str.concat(num) == 1);
  REQUIRE(str == "Hello 1");
}

TEST_CASE ("Testing String::concat(float)", "[String-concat-09]")
{
  String str("Hello ");
  float const num = 1.234f;
  REQUIRE(str.concat(num) == 1);
  REQUIRE(str == "Hello 1.23");
}

TEST_CASE ("Testing String::concat(double)", "[String-concat-10]")
{
  String str("Hello ");
  double const num = 5.678;
  REQUIRE(str.concat(num) == 1);
  REQUIRE(str == "Hello 5.68");
}

TEST_CASE ("Testing String::concat(const __FlashStringHelper *)", "[String-concat-11]")
{
#undef F
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))
  String str1("Hello");
  REQUIRE(str1.concat(F(" Arduino")) == 1);
  REQUIRE(str1 == "Hello Arduino");
}
