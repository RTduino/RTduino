#include <Arduino.h>

void setup(void)
{
    SerialUSB.begin();
}

void loop(void)
{
    SerialUSB.println("Hello world!");
    delay(500);
}
