/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#ifndef STREAM_MOCK_H_
#define STREAM_MOCK_H_

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class StreamMock : public Stream
{
public:
    StreamMock() : _buffer(nullptr), _size(0), _capacity(0), _head(0), _tail(0) {}

    ~StreamMock() {
        delete[] _buffer;
    }

    void operator<<(char const* str);

    virtual size_t write(uint8_t ch) override;
    virtual int available() override;
    virtual int read() override;
    virtual int peek() override;

private:
    void resize(size_t new_capacity);

    char* _buffer;
    size_t _size;
    size_t _capacity;
    size_t _head;
    size_t _tail;
};

#endif /* STREAM_MOCK_H_ */
