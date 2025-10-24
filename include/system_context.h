// SYSTEM_CONTEXT_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "leds.h"
#include "motors.h"
#include "encoder.h"
#include "bumper.h"
#include "ultrasonic.h"
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

    Move &move;

    IOShield &ioShield;
} SystemContext;