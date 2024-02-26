#pragma once

#include "HardwareSerial.h"
#include "motorcontrol.hh"
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
                front_right.set(DEFAULT_MOTOR_SPEED);
                front_left.set(DEFAULT_MOTOR_SPEED);
                back_left.set(DEFAULT_MOTOR_SPEED);
                back_right.set(DEFAULT_MOTOR_SPEED);
                break;
            case 'B' :
                front_right.set(-DEFAULT_MOTOR_SPEED);
                front_left.set(-DEFAULT_MOTOR_SPEED);
                back_left.set(-DEFAULT_MOTOR_SPEED);
                back_right.set(-DEFAULT_MOTOR_SPEED);
                break;
            case '0' :
                front_right.stop();
                front_left.stop();
                back_left.stop();
                back_right.stop();
                break;
            case 'S':
                servo1.write(FULL_TURN);
                break;
            case 'T':
                servo1.write(0);
                break;
            case 'C':
                servo2.write(FULL_TURN);
                break;
            case 'X':
                servo2.write(0);
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
    Servo servo1;
    Servo servo2;
};

} // namespace otters
