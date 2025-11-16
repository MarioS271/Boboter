// MOVE_CPP
// Boboter
// (C) MarioS271 2025

#include "move.h"
#include <cmath>
#include "logger.h"
#include "delay.h"

#define TAG "MOVE"

// Constructor
Move::Move(Motor& leftMotor,
           Motor& rightMotor,
           Encoder& leftEncoder,
           Encoder& rightEncoder,
           Bumper& leftBumper,
           Bumper& rightBumper,
           Ultrasonic& ultrasonic)
    : motorLeft(leftMotor),
      motorRight(rightMotor),
      encoderLeft(leftEncoder),
      encoderRight(rightEncoder),
      bumperLeft(leftBumper),
      bumperRight(rightBumper),
      usonic(ultrasonic)
{
    LOGI(TAG, "Move subsystem initialized using external references.");
}

// Helper Function: Stop all Motors
void stopMotors(Motor &left, Motor &right) {
    left.stop(false);
    right.stop();
}

// Drive forward or backward with automatic encoder-based correction
void Move::drive(int32_t distance_cm, uint16_t speed, bool forward) {
    encoderLeft.resetPulseCount();
    encoderRight.resetPulseCount();

    motorLeft.setDirection(forward ? FORWARD : BACKWARD);
    motorRight.setDirection(forward ? FORWARD : BACKWARD);

    float wheelCircumference = M_PI * WHEEL_DIAMETER_CM;
    int32_t targetPulses = static_cast<int32_t>((distance_cm / wheelCircumference) * PULSES_PER_REV);

    while (encoderLeft.getPulseCount() < targetPulses || encoderRight.getPulseCount() < targetPulses) {
        int32_t leftCount = encoderLeft.getPulseCount();
        int32_t rightCount = encoderRight.getPulseCount();

        // Scale the slower wheel to match the faster wheel
        int16_t leftSpeed, rightSpeed;

        if (leftCount == rightCount) {
            leftSpeed  = speed;
            rightSpeed = speed;
        } else if (leftCount < rightCount) {
            leftSpeed  = speed;
            rightSpeed = speed * leftCount / rightCount;
        } else {
            leftSpeed  = speed * rightCount / leftCount;
            rightSpeed = speed;
        }

        motorLeft.setSpeed(leftSpeed);
        motorRight.setSpeed(rightSpeed);

        // Safety checks
        if (bumperLeft.isHit() || bumperRight.isHit()) break;
        if (forward && usonic.measureCm() < MIN_ALLOWED_OBSTACLE_DISTANCE_CM) break;

        delay(5);
    }

    stopMotors(motorLeft, motorRight);
}

// Forward wrapper
void Move::forward(uint32_t distance, uint16_t speed) {
    drive(distance, speed, true);
}

// Backward wrapper
void Move::backward(uint32_t distance, uint16_t speed) {
    drive(distance, speed, false);
}

// In-place turn with automatic encoder synchronization
void Move::turn(int16_t angle, uint16_t speed) {
    encoderLeft.resetPulseCount();
    encoderRight.resetPulseCount();

    if (angle > 0) {
        motorLeft.setDirection(FORWARD);
        motorRight.setDirection(BACKWARD);
    } else {
        motorLeft.setDirection(BACKWARD);
        motorRight.setDirection(FORWARD);
        angle = -angle;
    }

    float distancePerWheel = (M_PI * WHEEL_BASE_CM * angle) / 720.0;
    int32_t targetPulses = static_cast<int32_t>((distancePerWheel / (M_PI * WHEEL_DIAMETER_CM)) * PULSES_PER_REV);

    while (abs(encoderLeft.getPulseCount()) < targetPulses || abs(encoderRight.getPulseCount()) < targetPulses) {
        int32_t leftCount  = abs(encoderLeft.getPulseCount());
        int32_t rightCount = abs(encoderRight.getPulseCount());

        int16_t leftSpeed, rightSpeed;

        if (leftCount == rightCount) {
            leftSpeed  = speed;
            rightSpeed = speed;
        } else if (leftCount < rightCount) {
            leftSpeed  = speed;
            rightSpeed = speed * leftCount / rightCount;
        } else {
            leftSpeed  = speed * rightCount / leftCount;
            rightSpeed = speed;
        }

        motorLeft.setSpeed(leftSpeed);
        motorRight.setSpeed(rightSpeed);

        if (bumperLeft.isHit() || bumperRight.isHit()) break;

        delay(5);
    }

    stopMotors(motorLeft, motorRight);
}
