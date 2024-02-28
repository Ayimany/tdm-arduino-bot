#pragma once

#if ! defined(__AVR__) && defined(PLATFORMIO)
#    define __AVR__
#endif

#include "math.hh"
#include "AFMotor.h"

namespace otters {

class dc_motor {
public:
    inline explicit dc_motor(uint8_t port)
        : motor(port) { }

    inline auto
    set(double value) -> void {
        value = max(-1.0, min(1.0, value));

        motor.run((value * inversion_factor) < 0 ? BACKWARD : FORWARD);
        motor.setSpeed(ftpwm(abs(value)));
    }

    inline auto set_inverted(bool inverted) -> void {
        this->inversion_factor = inverted ? -1 : 1;
    }

    inline auto
    stop() -> void {
        motor.run(RELEASE);
        motor.setSpeed(0);
    }

private:
    AF_DCMotor motor;
    int8_t inversion_factor = 1;
};

} // namespace otters

