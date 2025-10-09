// MOVE_CPP
// Boboter
// (C) MarioS271 2025

#include "move.h"
#include <cmath>
#include "esp_log.h"
#include "delay.h"

static const char* TAG = "MOVE";

// Constructor
Move::Move()
: motorLeft(MOTOR_LEFT),
  motorRight(MOTOR_RIGHT),
  encoderLeft(ENCODER_LEFT),
  encoderRight(ENCODER_RIGHT),
  bumperLeft(BUMPER_LEFT),
  bumperRight(BUMPER_RIGHT),
  usonic() {};



// Helper Function: Stop all Motors
void stopMotors(Motor &left, Motor &right) {
    left.stop(false);
    right.stop();
}

// Move forward a certain distance in cm
void Move::forward(uint32_t distance, uint16_t speed) {
    ESP_LOGI(TAG, "Moving forward for %d cm with speed %d", static_cast<int>(distance), speed);

    encoderLeft.resetPulseCount();
    encoderRight.resetPulseCount();

    motorLeft.setDirection(FORWARD);
    motorRight.setDirection(FORWARD);

    motorLeft.setSpeed(speed);
    motorRight.setSpeed(speed);

    float wheelCircumference = M_PI * WHEEL_DIAMETER_CM;
    int32_t targetPulses = static_cast<int32_t>((distance / wheelCircumference) * PULSES_PER_REV);

    while (encoderLeft.getPulseCount() < targetPulses && encoderRight.getPulseCount() < targetPulses) {
        if (bumperLeft.isHit() || bumperRight.isHit()) {
            ESP_LOGW(TAG, "Bumper hit! Stopping.");
            break;
        }

        if (usonic.measureCm() < MIN_ALLOWED_OBSTACLE_DISTANCE_CM) {
            ESP_LOGW(TAG, "Obstacle detected! Stopping.");
            break;
        }

        delay(10);
    }

    stopMotors(motorLeft, motorRight);
}

// Move backward a certain distance in cm
void Move::backward(uint32_t distance, uint16_t speed) {
    ESP_LOGI(TAG, "Moving backward for %d cm with speed %d", static_cast<int>(distance), speed);

    encoderLeft.resetPulseCount();
    encoderRight.resetPulseCount();

    motorLeft.setDirection(BACKWARD);
    motorRight.setDirection(BACKWARD);

    motorLeft.setSpeed(speed);
    motorRight.setSpeed(speed);

    float wheelCircumference = M_PI * WHEEL_DIAMETER_CM;
    int32_t targetPulses = static_cast<int32_t>((distance / wheelCircumference) * PULSES_PER_REV);

    while (encoderLeft.getPulseCount() < targetPulses && encoderRight.getPulseCount() < targetPulses) {
        if (bumperLeft.isHit() || bumperRight.isHit()) {
            ESP_LOGW(TAG, "Bumper hit! Stopping.");
            break;
        }

        delay(10);
    }

    stopMotors(motorLeft, motorRight);
}

// Turn robot a certain number of degrees (positive = right, negative = left)
void Move::turn(int16_t angle, uint16_t speed) {
    ESP_LOGI(TAG, "Turning for %d° with speed %d", static_cast<int>(angle), speed);

    encoderLeft.resetPulseCount();
    encoderRight.resetPulseCount();

    if (angle > 0) {  // Right turn
        motorLeft.setDirection(FORWARD);
        motorRight.setDirection(BACKWARD);
    }
    else {  // Left turn
        motorLeft.setDirection(BACKWARD);
        motorRight.setDirection(FORWARD);
        angle = -angle;
    }

    motorLeft.setSpeed(speed);
    motorRight.setSpeed(speed);

    // Calculate distance each wheel needs to travel
    float turnCircumference = M_PI * WHEEL_BASE_CM;
    float distancePerWheel = (turnCircumference * angle) / 360.0;
    int32_t targetPulses = static_cast<int32_t>((distancePerWheel / (M_PI * WHEEL_DIAMETER_CM)) * PULSES_PER_REV);

    while (encoderLeft.getPulseCount() < targetPulses && encoderRight.getPulseCount() < targetPulses) {
        if (bumperLeft.isHit() || bumperRight.isHit()) {
            ESP_LOGW(TAG, "Bumper hit during turn! Stopping.");
            break;
        }

        delay(10);
    }

    stopMotors(motorLeft, motorRight);
}