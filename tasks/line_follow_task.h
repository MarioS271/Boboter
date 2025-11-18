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

namespace LINEFOLLOW_TASK {
    constexpr const char* TAG = "task:LINEFOLLOWER";
}

void lineFollowTask(void* params) {
    using namespace LINEFOLLOW_TASK;
    
    SystemContext* ctx = static_cast<SystemContext*>(params);
    Linefollower &linefollower = ctx->linefollower;
    Motor &motorL = ctx->motorL;
    Motor &motorR = ctx->motorR;

    constexpr uint16_t BASE_SPEED = MAX_MOTOR_SPEED / 4;

    motorL.setDirection(M_FORWARD);
    motorR.setDirection(M_FORWARD);

    while (true) {
        lf_result_t left = linefollower.get(LF_LEFT);
        lf_result_t right = linefollower.get(LF_RIGHT);

        uint16_t leftSpeed = BASE_SPEED;
        uint16_t rightSpeed = BASE_SPEED;

        if (left == LF_BLACK && right == LF_WHITE) {
            LOGI(TAG, "Line left → turning left (L:%d R:%d)", leftSpeed, rightSpeed);
            leftSpeed = BASE_SPEED / 2;
            rightSpeed = BASE_SPEED;
        }
        else if (left == LF_WHITE && right == LF_BLACK) {
            LOGI(TAG, "Line right → turning right (L:%d R:%d)", leftSpeed, rightSpeed);
            leftSpeed = BASE_SPEED;
            rightSpeed = BASE_SPEED / 2;
        }
        else if (left == LF_BLACK && right == LF_BLACK) {
            LOGI(TAG, "Line centered → moving forward (L:%d R:%d)", leftSpeed, rightSpeed);
            leftSpeed = BASE_SPEED;
            rightSpeed = BASE_SPEED;
        }
        else if (left == LF_WHITE && right == LF_WHITE) {
            LOGW(TAG, "Line lost → stopping motors");
            motorL.stop();
            motorR.stop();
            delay(50);
            continue;
        }

        motorL.setSpeed(leftSpeed);
        motorR.setSpeed(rightSpeed);

        delay(20);
    }
}