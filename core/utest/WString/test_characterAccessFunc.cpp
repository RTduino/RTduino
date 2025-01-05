/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::charAt(unsigned int)", "[String-charAt-01]")
{
  String str1("Hello");
  REQUIRE(str1.charAt(2) == 'l');
}

TEST_CASE ("Testing String::setCharAt(unsigned int, char )", "[String-setCharAt-02]")
{
  String str1("Hello");
  str1.setCharAt(1, 'a');
  REQUIRE(str1 == "Hallo");
}

TEST_CASE ("Testing String::getBytes(unsigned char, unsigned int, unsigned int)", "[String-getBytes-02]")
{
  WHEN("No bufsize") {
    String str("Hello");
    unsigned char buf[2];
    str.getBytes(buf, 0, 0);
  }

  WHEN("Index >= len") {
    String str("Hello");
    unsigned char buf[2];
    str.getBytes(buf, 5, 6);
  }

  WHEN("Valid operation") {
    String str("Hello");
    unsigned char buf[3];
    str.getBytes(buf, 5, 3);
    REQUIRE(buf[0] == 'l');
    REQUIRE(buf[1] == 'o');
    REQUIRE(buf[2] == '\0');
  }
}

TEST_CASE ("Testing & String::operator[]", "[String-&operator[]-03]")
{
  String str("Hello");
  str[0] = 'M';
  REQUIRE(str == "Mello");
}

TEST_CASE ("Testing & String::operator[] with invalid buffer", "[String-&operator[]-04]")
{
  char *buffer = NULL;
  String str(buffer);
  str[0] = 'M';
  REQUIRE(str[0] == 0);
}
