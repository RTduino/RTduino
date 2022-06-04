#include <Arduino.h>

void setup(void)
{
}

void loop(void)
{
    int ch;
    ch = SerialUSB.read();
    if (ch > 0) {
        SerialUSB.print((char)ch);
    }
}