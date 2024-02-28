#pragma once

#include "motorcontrol.hh"

namespace otters {

class drivetrain {
    public:
        explicit inline drivetrain(double wheel_distance, uint8_t left_motor_id, uint8_t right_motor_id):
    _wheel_distance(wheel_distance), _left_motor(left_motor_id), _right_motor(right_motor_id){
        _right_motor.set_inverted(true);
        }

        inline auto differential_drive(double velocity, double angular_velocity) -> void {
            _left_motor.set(velocity - angular_velocity);
            _right_motor.set(velocity + angular_velocity);
        }

        inline auto stop() -> void {
            _left_motor.stop();
            _right_motor.stop();
        }

    private:
        double _wheel_distance;
        dc_motor _left_motor;
        dc_motor _right_motor;
};

}

