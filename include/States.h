#pragma once

#include <Arduino.h>

// The state names stay the same so the rest of the project can use them.
enum class Pump : uint8_t {
    ON,
    OFF
};

enum class SolenoidState : uint8_t {
    OFF,
    MANUAL,
    SCHEDULE,
    AUTO
};

enum class LightState : uint8_t {
    OFF,
    MANUAL,
    SCHEDULE
};

enum class DeviceState : uint8_t {
    OFF,
    AUTO,
    MANUAL
};

// These variables are defined once in States.cpp.
extern Pump Pump_State;

extern SolenoidState Irrigation_State;
extern SolenoidState Mist_State;
extern LightState Light_State;
extern DeviceState Fan_State;
extern DeviceState Heater_State;

class WebServer;

void Register_State_Controls(WebServer& server);
