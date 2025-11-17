// SYSTEM_CONTEXT_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "flex_struct.h"


class BatteryManager;

class Leds;
class Motor;
class Encoder;
class Bumper;
class Ultrasonic;
class Gyro;
class Move;
class IOShield;


typedef struct {
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
    Move &move;
    IOShield &ioShield;
    
    FlexStruct ledsFlex;
    FlexStruct ioShieldFlex;
    FlexStruct webuiFlex;
} SystemContext;