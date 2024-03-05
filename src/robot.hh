#pragma once

#include "HardwareSerial.h"
#include "drivetrain.hh"
#include "motorcontrol.hh"
#include "srlmod.hh"
#include "timer.hh"
#include <Servo.h>

uint8_t constexpr SERVO1_PIN         = 9;
uint8_t constexpr SERVO2_PIN         = 10;
double constexpr DEFAULT_MOTOR_SPEED = 0.5;
uint8_t constexpr FULL_TURN          = 180;

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
        servo1.attach(SERVO1_PIN);
        servo2.attach(SERVO2_PIN);
        _comms_module.init();
    }

    inline auto
    periodic() -> void {
        serial_read_status bluetooth_rec = _comms_module.read();

        if (mode_is_auto) {
            if (_timer.ellapsed_time_ms() > 0
                && _timer.ellapsed_time_ms() < 1500) {
                _drivetrain.differential_drive(0.16, 0);
            }

            if (_timer.ellapsed_time_ms() > 1500
                && _timer.ellapsed_time_ms() < 3000) {
                _drivetrain.differential_drive(0.75, 0);
            }

            if (_timer.ellapsed_time_ms() > 3000
                && _timer.ellapsed_time_ms() < 5000) {
                _drivetrain.differential_drive(0.25, 0);
            }

            if (_timer.ellapsed_time_ms() > 5000
                && _timer.ellapsed_time_ms() < 7500) {
                _drivetrain.differential_drive(0, 0.25);
            }

            if (_timer.ellapsed_time_ms() > 7500
                && _timer.ellapsed_time_ms() < 9500) {
                _drivetrain.differential_drive(0.75, 0.75);
            }

            if (_timer.ellapsed_time_ms() > 9500
                && _timer.ellapsed_time_ms() < 10000) {
                _drivetrain.differential_drive(0.1, 0.25);
            }

            if (_timer.ellapsed_time_ms() > 10000
                && _timer.ellapsed_time_ms() < 12000) {
                _drivetrain.differential_drive(0.5, 0.0);
            }

            if (_timer.ellapsed_time_ms() > 12000
                && _timer.ellapsed_time_ms() < 13500) {
                _drivetrain.differential_drive(0.25, 0.25);
            }

            if (_timer.ellapsed_time_ms() > 13500
                && _timer.ellapsed_time_ms() < 14500) {
                _drivetrain.differential_drive(0.75, 0);
            }

            if (_timer.ellapsed_time_ms() > 14500
                && _timer.ellapsed_time_ms() < 15000) {
                _drivetrain.differential_drive(0.75, 1.0);
            }

            if (_timer.ellapsed_time_ms() > 15000) {
                mode_is_auto = false;
                _drivetrain.stop();
                return;
            }
        }

        if (bluetooth_rec.status == serial_read_status::PRESENT) {
            int32_t received = bluetooth_rec.value;

            switch (received) {
            case 'F' :
                _drivetrain.differential_drive(1.0, 0.0);
                break;
            case 'B' :
                _drivetrain.differential_drive(-1.0, 0.0);
                break;
            case 'L' :
                _drivetrain.differential_drive(0.0, -1.0);
                break;
            case 'R' :
                _drivetrain.differential_drive(0.0, 1.0);
                break;
            case '0' :
                if (! mode_is_auto) {
                    _drivetrain.stop();
                }

                break;
            case 'S' :
                servo1.write(FULL_TURN);
                break;
            case 'T' :
                servo1.write(0);
                break;
            case 'C' :
                break;
            case 'X' :
                mode_is_auto = true;
                _timer.reset();
                break;
            default :
                break;
            }
        }
    }

private:
    bluetooth_module &_comms_module; // NOLINT: INTENTIONAL
    drivetrain        _drivetrain { 0.175, 2, 3 };
    Servo             servo1;
    Servo             servo2;
    bool              mode_is_auto = false;
    timer             _timer;
};

} // namespace otters
