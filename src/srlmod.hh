#pragma once

#include "HardwareSerial.h"
#ifdef PLATFORMIO
#    include <Arduino.h>
#endif

#include <SoftwareSerial.h>

namespace otters {

struct serial_read_status {
    static constexpr uint8_t EMPTY   = 0;
    static constexpr uint8_t PRESENT = 1;

    uint8_t status;
    int32_t value;
};

struct hardware_serial_config {
    uint16_t baud_rate;
};

struct serial_config {
    uint8_t  receiver;
    uint8_t  transmitter;
    uint16_t baud_rate;
};

class serial_wrapper {
public:
    static auto
    read() -> serial_read_status {
        if (! static_cast<bool>(Serial.available())) {
            return { serial_read_status::EMPTY, 0x0 };
        }

        return { serial_read_status::PRESENT, Serial.read() };
    }

    inline static HardwareSerial &internal = Serial; // NOLINT INTENTIONAL

private:
    hardware_serial_config _config;
};

class bluetooth_module {
public:
    explicit bluetooth_module(serial_config const &config)
        : _module(config.receiver, config.transmitter)
        , _config(config) { }

    auto
    init() -> void {
        _module.begin(_config.baud_rate);
    }

    auto
    read() -> serial_read_status {
        if (! static_cast<bool>(_module.available())) {
            return { serial_read_status::EMPTY, 0x0 };
        }

        return { serial_read_status::PRESENT, _module.read() };
    }

    auto
    write(int32_t value) -> void {
        _module.write(value);
    }

private:
    SoftwareSerial _module;
    serial_config  _config;
};

} // namespace otters
