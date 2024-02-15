#pragma once

#include "HardwareSerial.h"
#include "math.hh"
#include "motorcontrol.hh"
#include "srlmod.hh"

namespace otters {

uint16_t constexpr HARDWARE_BAUD_RATE = 9600;

class robot {
public:
    inline explicit robot(bluetooth_module &module)
        : _comms_module(module) { }

    inline auto
    init() -> void {
        serial_wrapper::internal.begin(HARDWARE_BAUD_RATE);
        serial_wrapper::internal.println("[SERIAL::STATUS] Initialized");
        _comms_module.init();
    }

    inline auto
    periodic() -> void {
        serial_read_status bluetooth_rec = _comms_module.read();
        serial_read_status serial_rec    = serial_wrapper::read();

        if (bluetooth_rec.status == serial_read_status::PRESENT) {
            int32_t received = bluetooth_rec.value;

            switch (received) {
            case 'F' :
                front_right.set(0.5);
                front_left.set(0.5);
                back_left.set(0.5);
                back_right.set(0.5);
                break;
            case 'B' :
                front_right.set(-0.5);
                front_left.set(-0.5);
                back_left.set(-0.5);
                back_right.set(-0.5);
                break;
            case '0' :
                front_right.stop();
                front_left.stop();
                back_left.stop();
                back_right.stop();
                break;
            default :
                break;
            }
        }
    }

private:
    bluetooth_module &_comms_module; // NOLINT: INTENTIONAL
    dc_motor          front_right { 1 };
    dc_motor          front_left { 2 };
    dc_motor          back_left { 3 };
    dc_motor          back_right { 4 };
};

} // namespace otters
