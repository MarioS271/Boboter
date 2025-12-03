// LINE_FOLLOW_TASK_H
// Boboter
// (C) MarioS271, ThomS263 2025

#pragma once

#include "system_context.h"
#include "flex_struct.h"
#include "delay.h"
#include "logger.h"
#include "motors.h"
#include "linefollower.h"

namespace LINEFOLLOW_TASK {
    constexpr const char* TAG = "task:LINEFOLLOWER";

    constexpr uint16_t BASE_SPEED = MAX_MOTOR_SPEED / 4;
    constexpr int16_t Kp = MAX_MOTOR_SPEED / 10;
    constexpr bool LOGGING_ENABLE = false;
}

void lineFollowTask(void* params) {
    using namespace LINEFOLLOW_TASK;
    
    SystemContext* ctx = static_cast<SystemContext*>(params);
    FlexStruct &ioShieldFlex = ctx->ioShieldFlex;
    Linefollower &linefollower = ctx->linefollower;
    Motor &motorL = ctx->motorL;
    Motor &motorR = ctx->motorR;

    motorL.setDirection(M_FORWARD);
    motorR.setDirection(M_FORWARD);

    while (true) {
        if (!(ioShieldFlex.get<bool>("linefollower_enabled"))) {
            motorL.stop(false);
            motorR.stop(false);
            delay(50);
            continue;
        }

        lf_result_t left = linefollower.get(LF_LEFT);
        lf_result_t right = linefollower.get(LF_RIGHT);
        
        int16_t error = 0;
        if (left == LF_BLACK && right == LF_WHITE) {
            error = -1; 
            if (LOGGING_ENABLE) LOGI(TAG, "Line left → Corrigiere nach links");
        }
        else if (left == LF_WHITE && right == LF_BLACK) {
            error = 1;  
            if (LOGGING_ENABLE) LOGI(TAG, "Line right → Corrigiere nach rechts");
        }
        else if (left == LF_BLACK && right == LF_BLACK) {
            error = 0;  
            if (LOGGING_ENABLE) LOGI(TAG, "Line centered → Moving forward");
        }
        else if (left == LF_WHITE && right == LF_WHITE) {
            if (LOGGING_ENABLE) LOGW(TAG, "Line lost → Rotating left to find line");

            // Rotate left until the line is found
            motorL.setSpeed(BASE_SPEED / 2);       // Left motor slower
            motorR.setSpeed(BASE_SPEED);           // Right motor faster
            motorL.setDirection(M_FORWARD);        // Left motor backward
            motorR.setDirection(M_BACKWARD);       // Right motor forward

            delay(50);  // Small delay to allow rotation
            continue;   // Recheck sensors
        }
        
        // Normal line following logic
        int16_t correction = error * Kp;

        int32_t tempLeftSpeed = BASE_SPEED + correction;
        int32_t tempRightSpeed = BASE_SPEED - correction;

        tempLeftSpeed = std::max((int32_t) 0, std::min((int32_t) MAX_MOTOR_SPEED, tempLeftSpeed));
        tempRightSpeed = std::max((int32_t) 0, std::min((int32_t) MAX_MOTOR_SPEED, tempRightSpeed));
    
        if (LOGGING_ENABLE) LOGI(TAG, "Error: %d, Correction: %d, Speed L:%u R:%u", error, correction, (uint16_t)tempLeftSpeed, (uint16_t)tempRightSpeed);
        
        motorL.setDirection(M_FORWARD);
        motorR.setDirection(M_FORWARD);
        motorL.setSpeed((uint16_t) tempLeftSpeed);
        motorR.setSpeed((uint16_t) tempRightSpeed);

        delay(40);
    }
}
