/*
 * Copyright (c) 2021-2025, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-01-04     Meco Man     first version
 */

#include <RTduino.h>
#include <WCharacter.h>
#include <utest.h>

/* Test case for isAlphaNumeric */
static void TC_isAlphaNumeric(void)
{
    for (int i = 0; i <= 127; i++) {
        if (isalnum(i)) {
            uassert_true(isAlphaNumeric(i));  /* Alphanumeric characters should return true */
        } else {
            uassert_false(isAlphaNumeric(i)); /* Non-alphanumeric characters should return false */
        }
    }
    /* Specific tests */
    uassert_true(isAlphaNumeric('A'));
    uassert_true(isAlphaNumeric('z'));
    uassert_true(isAlphaNumeric('0'));
    uassert_true(isAlphaNumeric('9'));
    uassert_false(isAlphaNumeric(' '));
    uassert_false(isAlphaNumeric('@'));
}

/* Test case for isAlpha */
static void TC_isAlpha(void)
{
    for (int i = 0; i <= 127; i++) {
        if (isalpha(i)) {
            uassert_true(isAlpha(i));  /* Alphabetic characters should return true */
        } else {
            uassert_false(isAlpha(i)); /* Non-alphabetic characters should return false */
        }
    }
    /* Specific tests */
    uassert_true(isAlpha('A'));
    uassert_true(isAlpha('z'));
    uassert_false(isAlpha('0'));
    uassert_false(isAlpha(' '));
}

static void TC_isAscii(void)
{
    /* Test all possible integer values from -255 to 255, focusing on ASCII boundaries */
    for (int i = -255; i <= 255; i++) {
        if (i >= 0 && i <= 127) {
            uassert_true(isAscii(i));  /* Values from 0 to 127 should return true */
        } else {
            uassert_false(isAscii(i)); /* Other values should return false */
        }
    }

    /* List some specific values for clarity and readability */
    uassert_true(isAscii(0));         /* ASCII value 0 should return true */
    uassert_true(isAscii(127));       /* ASCII value 127 should return true */
    uassert_false(isAscii(-1));       /* Value -1 is not an ASCII character */
    uassert_false(isAscii(128));      /* Value 128 is not an ASCII character */
    uassert_false(isAscii(255));      /* Value 255 is not an ASCII character */
}

/* Test case for isWhitespace */
static void TC_isWhitespace(void)
{
    for (int i = 0; i <= 127; i++) {
        if (isblank(i)) {
            uassert_true(isWhitespace(i));  /* Blank characters (space, tab) should return true */
        } else {
            uassert_false(isWhitespace(i)); /* Non-blank characters should return false */
        }
    }
    /* Specific tests */
    uassert_true(isWhitespace(' '));
    uassert_true(isWhitespace('\t'));
    uassert_false(isWhitespace('A'));
    uassert_false(isWhitespace('0'));
}

/* Test case for isControl */
static void TC_isControl(void)
{
    for (int i = 0; i <= 127; i++) {
        if (iscntrl(i)) {
            uassert_true(isControl(i));  /* Control characters should return true */
        } else {
            uassert_false(isControl(i)); /* Non-control characters should return false */
        }
    }
    /* Specific tests */
    uassert_true(isControl(0));
    uassert_true(isControl(127));
    uassert_false(isControl('A'));
    uassert_false(isControl(' '));
}

/* Test case for isDigit */
static void TC_isDigit(void)
{
    for (int i = 0; i <= 127; i++) {
        if (isdigit(i)) {
            uassert_true(isDigit(i));  /* Digit characters (0-9) should return true */
        } else {
            uassert_false(isDigit(i)); /* Non-digit characters should return false */
        }
    }
    /* Specific tests */
    uassert_true(isDigit('0'));
    uassert_true(isDigit('9'));
    uassert_false(isDigit('A'));
    uassert_false(isDigit(' '));
}

/* Test case for isGraph */
static void TC_isGraph(void)
{
    for (int i = 0; i <= 127; i++) {
        if (isgraph(i)) {
            uassert_true(isGraph(i));  /* Printable non-space characters should return true */
        } else {
            uassert_false(isGraph(i)); /* Non-printable or space characters should return false */
        }
    }
    /* Specific tests */
    uassert_true(isGraph('A'));
    uassert_false(isGraph(' '));
}

/* Test case for isLowerCase */
static void TC_isLowerCase(void)
{
    for (int i = 0; i <= 127; i++) {
        if (islower(i)) {
            uassert_true(isLowerCase(i));  /* Lowercase letters should return true */
        } else {
            uassert_false(isLowerCase(i)); /* Non-lowercase letters should return false */
        }
    }
    /* Specific tests */
    uassert_true(isLowerCase('a'));
    uassert_true(isLowerCase('z'));
    uassert_false(isLowerCase('A'));
    uassert_false(isLowerCase('0'));
}

/* Test case for isPrintable */
static void TC_isPrintable(void)
{
    for (int i = 0; i <= 127; i++) {
        if (isprint(i)) {
            uassert_true(isPrintable(i));  /* Printable characters should return true */
        } else {
            uassert_false(isPrintable(i)); /* Non-printable characters should return false */
        }
    }
    /* Specific tests */
    uassert_true(isPrintable('A'));
    uassert_true(isPrintable(' '));
    uassert_false(isPrintable(127)); /* ASCII DEL is not printable */
}

/* Test case for isPunct */
static void TC_isPunct(void)
{
    for (int i = 0; i <= 127; i++) {
        if (ispunct(i)) {
            uassert_true(isPunct(i));  /* Punctuation characters should return true */
        } else {
            uassert_false(isPunct(i)); /* Non-punctuation characters should return false */
        }
    }
    /* Specific tests */
    uassert_true(isPunct('!'));
    uassert_true(isPunct('.'));
    uassert_false(isPunct('A'));
    uassert_false(isPunct(' '));
}

/* Test case for isSpace */
static void TC_isSpace(void)
{
    for (int i = 0; i <= 127; i++) {
        if (isspace(i)) {
            uassert_true(isSpace(i));  /* White-space characters should return true */
        } else {
            uassert_false(isSpace(i)); /* Non-white-space characters should return false */
        }
    }
    /* Specific tests */
    uassert_true(isSpace(' '));
    uassert_true(isSpace('\n'));
    uassert_false(isSpace('A'));
    uassert_false(isSpace('0'));
}

/* Test unit for isUpperCase */
static void TC_isUpperCase(void)
{
    for (int i = 0; i < 128; i++) {
        char c = (char)i;
        if (c >= 'A' && c <= 'Z') {
            uassert_true(isUpperCase(c));
        } else {
            uassert_false(isUpperCase(c));
        }
    }

    uassert_false(isUpperCase('\n'));
    uassert_false(isUpperCase('\t'));
    uassert_false(isUpperCase('\r'));
    uassert_false(isUpperCase('\0'));
}

/* Test unit for isHexadecimalDigit */
static void TC_isHexadecimalDigit(void)
{
    /* Hexadecimal digits are 0-9, A-F, and a-f */
    for (int i = 0; i <= 127; i++) {
        boolean expected = (i >= '0' && i <= '9') ||
                           (i >= 'A' && i <= 'F') ||
                           (i >= 'a' && i <= 'f');
        if (expected) {
            uassert_true(isHexadecimalDigit(i));
        } else {
            uassert_false(isHexadecimalDigit(i));
        }
    }

    /* Test some specific non-ASCII values */
    uassert_false(isHexadecimalDigit(128));
    uassert_false(isHexadecimalDigit(-1));
    uassert_false(isHexadecimalDigit(255));
}

/* Test unit for toAscii */
static void TC_toAscii(void)
{
    /* For ASCII values, toAscii should return the value itself */
    for (int i = 0; i <= 127; i++) {
        uassert_int_equal(toAscii(i), i);
    }

    uassert_int_equal(toAscii(0xf7), 0x77);
    uassert_int_equal(toAscii((unsigned char)0xf7), 0x77);
    uassert_int_equal(toAscii(0x3030), 0x30);
    uassert_int_equal(toAscii(' '), ' ');
    uassert_int_equal(toAscii('\n'), '\n');
    uassert_int_equal(toAscii('\r'), '\r');
    uassert_int_equal(toAscii('q'), 'q');
}

/* Test unit for toLowerCase */
static void TC_toLowerCase(void)
{
    /* Test all uppercase letters */
    for (int i = 'A'; i <= 'Z'; i++) {
        uassert_int_equal(toLowerCase(i), i + ('a' - 'A')); /* A-Z should convert to a-z */
    }

    /* Test lowercase letters and non-alphabetic characters */
    for (int i = 0; i <= 127; i++) {
        if (i >= 'a' && i <= 'z') {
            uassert_int_equal(toLowerCase(i), i); /* a-z should remain unchanged */
        }
        if (i < 'A' || (i > 'Z' && i < 'a') || i > 'z') {
            uassert_int_equal(toLowerCase(i), i); /* Non-alphabetic should remain unchanged */
        }
    }

    /* Test non-ASCII values */
    uassert_int_equal(toLowerCase(128), 128); /* Non-ASCII values should remain unchanged */
}

/* Test unit for toUpperCase */
static void TC_toUpperCase(void)
{
    /* Test all lowercase letters */
    for (int i = 'a'; i <= 'z'; i++) {
        uassert_int_equal(toUpperCase(i), i - ('a' - 'A')); /* a-z should convert to A-Z */
    }

    /* Test uppercase letters and non-alphabetic characters */
    for (int i = 0; i <= 127; i++) {
        if (i >= 'A' && i <= 'Z') {
            uassert_int_equal(toUpperCase(i), i); /* A-Z should remain unchanged */
        }
        if (i < 'A' || (i > 'Z' && i < 'a') || i > 'z') {
            uassert_int_equal(toUpperCase(i), i); /* Non-alphabetic should remain unchanged */
        }
    }

    /* Test non-ASCII values */
    uassert_int_equal(toUpperCase(128), 128); /* Non-ASCII values should remain unchanged */
}

/* Utest function to run all test cases */
static void utest_do_tc(void)
{
    UTEST_UNIT_RUN(TC_isAlphaNumeric);
    UTEST_UNIT_RUN(TC_isAlpha);
    UTEST_UNIT_RUN(TC_isAscii);
    UTEST_UNIT_RUN(TC_isWhitespace);
    UTEST_UNIT_RUN(TC_isControl);
    UTEST_UNIT_RUN(TC_isDigit);
    UTEST_UNIT_RUN(TC_isGraph);
    UTEST_UNIT_RUN(TC_isLowerCase);
    UTEST_UNIT_RUN(TC_isPrintable);
    UTEST_UNIT_RUN(TC_isPunct);
    UTEST_UNIT_RUN(TC_isSpace);
    UTEST_UNIT_RUN(TC_isUpperCase);
    UTEST_UNIT_RUN(TC_isHexadecimalDigit);
    UTEST_UNIT_RUN(TC_toAscii);
    UTEST_UNIT_RUN(TC_toLowerCase);
    UTEST_UNIT_RUN(TC_toUpperCase);
}

UTEST_TC_EXPORT(utest_do_tc, "RTduino.core.WCharacter", RT_NULL, RT_NULL, 1000);
