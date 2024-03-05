#pragma once

#include <Arduino.h>

namespace otters {

class timer {
public:
    explicit inline timer()
        : _start_time_ms(millis()) { }

    inline auto
    reset() -> void {
        _start_time_ms = millis();
    }

    [[nodiscard]] inline auto
    ellapsed_time_ms() const -> unsigned long long int {
        return millis() - _start_time_ms;
    }

private:
    unsigned long long int _start_time_ms;
};

} // namespace otters
