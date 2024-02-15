#ifdef PLATFORMIO
#    include <Arduino.h>
#endif

#include "robot.hh"

otters::serial_config const g_btconfig { .receiver    = 2,
                                         .transmitter = 13,
                                         .baud_rate   = 38400 };

otters::bluetooth_module g_module { g_btconfig }; // NOLINT: INTENTIONAL
otters::robot            g_robot { g_module };    // NOLINT: INTENTIONAL

auto

setup() -> void {
    g_robot.init();
}

auto
loop() -> void {
    g_robot.periodic();
}
