#pragma once

#include "HardwareSerial.h"
#include "motorcontrol.hh"
#include "drivetrain.hh"
#include "srlmod.hh"
#include <Servo.h>

uint8_t constexpr SERVO1_PIN = 9;
uint8_t constexpr SERVO2_PIN = 10;
double constexpr DEFAULT_MOTOR_SPEED = 0.5;
uint8_t constexpr FULL_TURN = 180;


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
                if (!mode_is_auto) {
                    _drivetrain.stop();
                }

                break;
            case 'S':
                servo1.write(FULL_TURN);
                break;
            case 'T':
                servo1.write(0);
                break;
            case 'C':
                break;
            case 'X':
                mode_is_auto = true;
                // TODO: Actually integrate sensors
                // Later tho

                _drivetrain.differential_drive(0.75, 0.0);
                delay(1500);
                _drivetrain.differential_drive(0.0, 0.25);
                delay(1500);
                _drivetrain.differential_drive(0.55, 0.0);
                delay(1500);
                _drivetrain.differential_drive(0.75, 0.25);
                delay(1500);
                _drivetrain.differential_drive(0.0, 0.0);
                delay(500);
                _drivetrain.differential_drive(-0.75, 0.0);
                delay(1500);
                _drivetrain.differential_drive(0.0, 0.0);
                delay(500);
                _drivetrain.differential_drive(0.15, 0.0);
                delay(1500);
                _drivetrain.differential_drive(0.5, 0.5);
                delay(1500);
                _drivetrain.differential_drive(0.2, -0.25);
                delay(1500);
                _drivetrain.differential_drive(0.0, 0.5);
                delay(1500);
                _drivetrain.differential_drive(0.0, 0.0);
                delay(500);
                _drivetrain.differential_drive(0.0, 0.25);
                delay(1500);
                _drivetrain.differential_drive(0.0, -0.25);
                delay(1500);
                _drivetrain.differential_drive(0.5, 0.0);
                delay(1500);
                _drivetrain.differential_drive(0.0, 0.0);
                delay(500);
                _drivetrain.differential_drive(-0.5, 0.0);
                delay(1500);
                _drivetrain.differential_drive(0.0, 0.0);

                break;
            default :
                break;
            }
        }
    }

private:
    bluetooth_module &_comms_module; // NOLINT: INTENTIONAL
    drivetrain _drivetrain { 0.175, 2, 3 };
    Servo servo1;
    Servo servo2;
    bool mode_is_auto = false;
};

} // namespace otters

