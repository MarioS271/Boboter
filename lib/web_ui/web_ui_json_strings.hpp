/**
 * @file web_ui_json_strings.hpp
 * @authors MarioS271
 */

#pragma once

namespace JSON_RESP_GET
{
    constexpr const char* SYSTEM_STATUS_RESPONSE = R"({
        "bat_percent":%d,
        "bat_voltage_mv":%d,
        "ip_address":"%s"
    })";

    constexpr const char* LED_RESPONSE = R"({
        "led":"%s",
        "r":%d,
        "g":%d,
        "b":%d
    })";
}