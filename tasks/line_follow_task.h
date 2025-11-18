// LINE_FOLLOW_TASK_H
// Boboter
// (C) MarioS271 2025

#pragma once

#include "system_context.h"
#include "flex_struct.h"

#include "delay.h"
#include "logger.h"

#include "linefollower.h"
#include "motors.h"

#define TAG "task:LINEFOLLOWER"

void lineFollowTask(void* params) {
    SystemContext* ctx = static_cast<SystemContext*>(params);
    Linefollower &linefollower = ctx->linefollower;
    Motor &motorL = ctx->motorL;
    Motor &motorR = ctx->motorR;

    constexpr uint16_t BASE_SPEED = MAX_MOTOR_SPEED / 4; // Base speed
    constexpr uint16_t TURN_DELTA = MAX_MOTOR_SPEED / 5; // Speed difference for turning

    while (true) {
        lf_result_t left = linefollower.get(LF_LEFT);
        lf_result_t right = linefollower.get(LF_RIGHT);

        uint16_t leftSpeed = BASE_SPEED;
        uint16_t rightSpeed = BASE_SPEED;

        if (left == LF_BLACK && right == LF_WHITE) {
            leftSpeed = BASE_SPEED / 2; // slow left motor
            rightSpeed = BASE_SPEED;
            LOGI(TAG, "Line left → turning left (L:%d R:%d)", leftSpeed, rightSpeed);
        } else if (left == LF_WHITE && right == LF_BLACK) {
            leftSpeed = BASE_SPEED;
            rightSpeed = BASE_SPEED / 2; // slow right motor
            LOGI(TAG, "Line right → turning right (L:%d R:%d)", leftSpeed, rightSpeed);
        } else if (left == LF_BLACK && right == LF_BLACK) {
            leftSpeed = BASE_SPEED;
            rightSpeed = BASE_SPEED;
            LOGI(TAG, "Line centered → moving forward (L:%d R:%d)", leftSpeed, rightSpeed);
        } else if (left == LF_WHITE && right == LF_WHITE) {
            // Lost line → stop
            LOGW(TAG, "Line lost → stopping motors");
            motorL.stop();
            motorR.stop();
            vTaskDelay(pdMS_TO_TICKS(50));
            continue;
        }

        // Both motors forward
        motorL.setDirection(M_FORWARD);
        motorR.setDirection(M_FORWARD);

        motorL.setSpeed(leftSpeed);
        motorR.setSpeed(rightSpeed);

        delay(20);
    }
}