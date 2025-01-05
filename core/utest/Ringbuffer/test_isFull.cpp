/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("'isFull' should return false for empty ring buffer", "[Ringbuffer-isFull-01]")
{
  RingBufferN<2> ringbuffer;
  REQUIRE(ringbuffer.isFull() == false);
}

TEST_CASE ("'isFull' should return false for a partial full ring buffer", "[Ringbuffer-isFull-02]")
{
  RingBufferN<2> ringbuffer;
  ringbuffer.store_char('A');
  REQUIRE(ringbuffer.isFull() == false);
}

TEST_CASE ("'isFull' should return true for full ring buffer", "[Ringbuffer-isFull-03]")
{
  RingBufferN<2> ringbuffer;
  ringbuffer.store_char('A');
  ringbuffer.store_char('B');
  REQUIRE(ringbuffer.isFull() == true);
}
