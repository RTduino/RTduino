/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "StreamMock.h"
#include <utest.h>

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void StreamMock::resize(size_t new_capacity)
{
    if (new_capacity == 0) {
        return;
    }

    if (new_capacity == _capacity) {
        return;
    }

    char* new_buffer = new char[new_capacity];
    if (!_buffer) { // If buffer is null, nothing to copy
        // Directly set the new buffer and return
        _buffer = new_buffer;
        _capacity = new_capacity;
        _head = 0;
        _tail = 0;
        return;
    }

    size_t count = _size;
    size_t index = 0;

    // Copy data from old buffer to new buffer
    for (size_t i = 0; i < count; ++i) {
        new_buffer[index++] = _buffer[(_head + i) % _capacity];
    }
    delete[] _buffer; // Now safe to delete old buffer

    _buffer = new_buffer;
    _capacity = new_capacity;
    _head = 0;
    _tail = count;
}

void StreamMock::operator<<(char const* str)
{
    size_t len = rt_strlen(str);

    size_t new_size = _size + len;
    if (new_size > _capacity) {
        resize(new_size * 2); // Grow buffer
    }

    for (size_t i = 0; i < len; ++i) {
        _buffer[(_tail + i) % _capacity] = str[i];
    }
    _tail = (_tail + len) % _capacity;
    _size += len;
}

size_t StreamMock::write(uint8_t ch)
{
    if (_size == _capacity) {
        resize(_capacity == 0 ? 1 : _capacity * 2);
    }

    _buffer[_tail] = static_cast<char>(ch);
    _tail = (_tail + 1) % _capacity;
    ++_size;

    return 1;
}

int StreamMock::available()
{
    return static_cast<int>(_size);
}

int StreamMock::read()
{
    if (_size == 0) {
        return -1;
    }

    char c = _buffer[_head];
    _head = (_head + 1) % _capacity;
    --_size;

    return static_cast<int>(c);
}

int StreamMock::peek()
{
    if (_size == 0) {
        return -1;
    }

    return static_cast<int>(_buffer[_head]);
}
