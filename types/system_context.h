// SYSTEM_CONTEXT_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "flex_struct.h"
#include "leds.h"
#include "motors.h"
#include "encoder.h"
#include "bumper.h"
#include "ultrasonic.h"
#include "gyro.h"
#include "move.h"
#include "io_shield.h"

typedef struct {
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
    
    FlexStruct ioShieldFlex;
    FlexStruct webuiFlex;
} SystemContext;