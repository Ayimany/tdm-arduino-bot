#pragma once

#include "math.hh"
#if ! defined(__AVR__) && defined(PLATFORMIO)
#    define __AVR__
#endif

#include "AFMotor.h"

namespace otters {

class dc_motor {
public:
    inline explicit dc_motor(uint8_t port)
        : motor(port) { }

    inline auto
    set(double value) -> void {
        motor.run(value < 0 ? BACKWARD : FORWARD);
        motor.setSpeed(ftpwm(abs(value)));
    }

    inline auto
    stop() -> void {
        motor.run(RELEASE);
        motor.setSpeed(0);
    }

private:
    AF_DCMotor motor;
};

} // namespace otters
