/**
 * @file robot.cpp
 *
 * @authors MarioS271
 * @copyright MIT License
 */

#include "include/robot.h"

#include "lib/logger/logger.h"

Robot::Robot() :
    gpio(GPIO::Controller::get_instance()),
    ledc(LEDC::Controller::get_instance()),
    adc(ADC::Controller::get_instance()),
    i2c(I2C::Controller::get_instance())
{
    LOGI("Constructor of Robot called");
}

Robot::~Robot() {
    LOGI("Destructor of Robot called");

    gpio.shutdown();
    ledc.shutdown();
    adc.shutdown();
    i2c.shutdown();
}