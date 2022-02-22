#ifndef Adafruit_I2CDevice_h
#define Adafruit_I2CDevice_h

#include <Arduino.h>
#include <rtdevice.h>

///< The class which defines how we will talk to this device over I2C
class Adafruit_I2CDevice
{
    private:
        uint8_t _addr;
        bool _begun;
        struct rt_i2c_bus_device *_i2c_bus_dev;

    public:
        Adafruit_I2CDevice::Adafruit_I2CDevice(uint8_t addr, TwoWire *theWire);
        uint8_t address(void);
        bool begin(bool addr_detect = true);
        void end(void);
        bool detected(void);
        bool read(uint8_t *buffer, size_t len, bool stop = true);
        bool write(const uint8_t *buffer, size_t len, bool stop = true,
                  const uint8_t *prefix_buffer = NULL, size_t prefix_len = 0);
        bool write_then_read(const uint8_t *write_buffer, size_t write_len,
                             uint8_t *read_buffer, size_t read_len,
                             bool stop = false);
        bool setSpeed(uint32_t desiredclk);
};

#endif // Adafruit_I2CDevice_h
