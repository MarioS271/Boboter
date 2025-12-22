/**
 * @file system_context.hpp
 * @authors MarioS271
*/

#pragma once

#include "types/flex.hpp"

namespace Boboter::Libs {
    namespace Battery { class Battery; }
    namespace RGB_Leds { class RGB_Leds; }
    namespace OtherLeds { class OtherLeds; }

    namespace PID_Controller { class PID_Controller; }

    namespace Motor { class Motor; }
    namespace Encoder { class Encoder; }
    namespace Bumper { class Bumper; }

    namespace Ultrasonic { class Ultrasonic; }
    namespace Gyro { class Gyro; }
    namespace Magnetometer { class Magnetometer; }

    namespace Colorsensor { class Colorsensor; }
    namespace Linefollower { class Linefollower; }

    namespace Display { class Display; }
    namespace Buzzer { class Buzzer; }
    namespace Button { class Button; }
}

namespace Boboter::Types {
    struct SystemContext {
        Boboter::Libs::Battery::Battery& battery;
        Boboter::Libs::RGB_Leds::RGB_Leds& rgb_leds;
        Boboter::Libs::OtherLeds::OtherLeds& other_leds;

        // Boboter::Libs::PID_Controller::PID_Controller& pid_controller;

        Boboter::Libs::Motor::Motor& motor_left;
        Boboter::Libs::Motor::Motor& motor_right;
        Boboter::Libs::Encoder::Encoder& encoder_left;
        Boboter::Libs::Encoder::Encoder& encoder_right;
        Boboter::Libs::Bumper::Bumper& bumper_left;
        Boboter::Libs::Bumper::Bumper& bumper_right;

        Boboter::Libs::Ultrasonic::Ultrasonic& ultrasonic;
        Boboter::Libs::Gyro::Gyro& gyro;
        Boboter::Libs::Magnetometer::Magnetometer& magnetometer;

        // Boboter::Libs::Colorsensor::Colorsensor& colorsensor;
        Boboter::Libs::Linefollower::Linefollower& linefollower;

        Boboter::Libs::Display::Display& display;
        Boboter::Libs::Buzzer::Buzzer& buzzer;
        Boboter::Libs::Button::Button& button_primary;
        Boboter::Libs::Button::Button& button_secondary;

        Boboter::Types::flex system_flex;
        Boboter::Types::flex buzzer_flex;
    };
}