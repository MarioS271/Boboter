/**
 * @file system_context.hpp
 * @authors MarioS271
 */

#pragma once

#include "flex_struct.hpp"


class BatteryManager;

class Leds;
class Motor;
class Encoder;
class Bumper;
class Ultrasonic;
class Gyro;
class Colorsensor;
class Linefollower;
class Move;
class IOShield;


typedef struct
{
    BatteryManager &batteryManager;

    Leds &leds;
    Motor &motorL;
    Motor &motorR;
    Encoder &encoderL;
    Encoder &encoderR;
    Bumper &bumperL;
    Bumper &bumperR;
    Ultrasonic &usonic;
    Gyro &gyro;
    Colorsensor &colorsensor;
    Linefollower &linefollower;
    Move &move;
    IOShield &ioShield;
    
    FlexStruct ledsFlex;
    FlexStruct ioShieldFlex;
    FlexStruct webuiFlex;
} SystemContext;