/**
 * @file system_context.hpp
 * @authors MarioS271
*/

#pragma once

#include "types/flex.hpp"

namespace Boboter::Libs {
    class Battery::Battery;
    class RGB_Leds::RGB_Leds;
    class OtherLeds::OtherLeds;
    
    class PID_Controller::PID_Controller;

    class Motor::Motor;
    class Encoder::Encoder;
    class Bumper::Bumper;
    
    class Ultrasonic::Ultrasonic;
    class Gyro::Gyro;
    class Magnetometer::Magnetometer;

    class Colorsensor::Colorsensor;
    class Linefollower::Linefollower;
    
    class Display::Display;
    class Buzzer::Buzzer;
    class Button::Button;

    class Bluepad::Bluepad;
}

namespace Boboter::Types {
    struct SystemContext {
        Boboter::Libs::Battery::Battery& battery;
        Boboter::Libs::RGB_Leds::RGB_Leds& rgb_leds;
        Boboter::Libs::OtherLeds::OtherLeds& other_leds;

        Boboter::Libs::PID_Controller::PID_Controller& pid_controller;

        Boboter::Libs::Motor::Motor& motor_left;
        Boboter::Libs::Motor::Motor& motor_right;
        Boboter::Libs::Encoder::Encoder& encoder_left;
        Boboter::Libs::Encoder::Encoder& encoder_right;
        Boboter::Libs::Bumper::Bumper& bumper_left;
        Boboter::Libs::Bumper::Bumper& bumper_right;

        Boboter::Libs::Ultrasonic::Ultrasonic& ultrasonic;
        Boboter::Libs::Gyro::Gyro& gyro;
        Boboter::Libs::Magnetometer::Magnetometer& magnetometer;

        Boboter::Libs::Colorsensor::Colorsensor& colorsensor;
        Boboter::Libs::Linefollower::Linefollower& linefollower;

        Boboter::Libs::Display::Display& display;
        Boboter::Libs::Buzzer::Buzzer& buzzer;
        Boboter::Libs::Button::Button& button_primary;
        Boboter::Libs::Button::Button& button_secondary;

        Boboter::Libs::Bluepad::Bluepad& bluepad;

        Boboter::Types::flex settings_flex;
        Boboter::Types::flex drive_flex;
    };
}