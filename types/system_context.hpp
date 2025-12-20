/**
 * @file system_context.hpp
 * @authors MarioS271
*/

#pragma once

#include "types/flex.hpp"

namespace Boboter::Libs {
    class Battery::Battery;
    class NeoLeds::NeoLeds;
    class OtherLeds;
    
    class Motor;
    class Encoder;
    class Bumper;
    
    class Ultrasonic;
    class Gyro;
    class Magnetometer;
    
    class Display;
    class Buzzer;
    class Button;
}

namespace Boboter::Types {
    struct system_context {
        Boboter::Libs::Battery::Battery& battery;
        Boboter::Libs::RgbLeds::RgbLeds& rgb_leds;
        Boboter::Libs::OtherLeds::OtherLeds& other_leds;

        Boboter::Libs::Motor& motorL;
        Boboter::Libs::Motor& motorR;
        Boboter::Libs::Encoder& encoderL;
        Boboter::Libs::Encoder& encoderR;
        Boboter::Libs::Bumper& bumperL;
        Boboter::Libs::Bumper& bumperR;

        Boboter::Libs::Ultrasonic& ultrasonic;
        Boboter::Libs::Gyro& gyro;
        Boboter::Libs::Magnetometer& magnetometer;

        Boboter::Libs::Display& display;
        Boboter::Libs::Buzzer& buzzer;
        Boboter::Libs::Button& button1;
        Boboter::Libs::Button& button2;

        Boboter::Types::flex ledsFlex;
        Boboter::Types::flex ioShieldFlex;
        Boboter::Types::flex webuiFlex;
    };
}