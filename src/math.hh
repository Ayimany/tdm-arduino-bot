#pragma once

#ifdef PLATFORMIO
#    include <Arduino.h>
#endif

namespace otters {

constexpr uint8_t PWM_MAX = 255;

inline constexpr auto
ftpwm(double value) -> uint8_t {
    return lround(value * PWM_MAX);
}

struct vector2 { };

} // namespace otters
