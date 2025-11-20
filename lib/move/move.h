// MOVE_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include <cstdint>
#include "motors.h"
#include "encoder.h"
#include "bumper.h"
#include "ultrasonic.h"

class Move {
private:
    static constexpr const char* TAG = "MOVE";

    static constexpr float WHEEL_DIAMETER_CM = 6.45f;                   // Durchmesser des Rades
    static constexpr float WHEEL_BASE_CM = 12.65f;                      // Distanz zwischen den Mittelpunkten beider Räder
    static constexpr float PULSES_PER_REV = 20.0f;                      // Encoder-Pulse pro Radrotation
    static constexpr float MIN_ALLOWED_OBSTACLE_DISTANCE_CM = 7.0f;     // Kleine zugelassene Distanz zu Ultraschallsensor

    Motor &motorLeft;
    Motor &motorRight;
    Encoder &encoderLeft;
    Encoder &encoderRight;
    Bumper &bumperLeft;
    Bumper &bumperRight;
    Ultrasonic &usonic;

    void drive(int32_t distance_cm, uint16_t baseSpeed, bool forward);

public:
    explicit Move(Motor& leftMotor,
                  Motor& rightMotor,
                  Encoder& leftEncoder,
                  Encoder& rightEncoder,
                  Bumper& leftBumper,
                  Bumper& rightBumper,
                  Ultrasonic& ultrasonic);
    ~Move() = default;

    void forward(uint32_t distance, uint16_t speed);
    void backward(uint32_t distance, uint16_t speed);
    void turn(int16_t degrees, uint16_t speed);
};