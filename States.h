#pragma once

#include <Arduino.h>
#include "connection.h"

// The state names stay the same so the rest of the project can use them.
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

SolenoidState Irrigation_State = SolenoidState::OFF;
SolenoidState Mist_State = SolenoidState::OFF;
LightState Light_State = LightState::OFF;
DeviceState Fan_State = DeviceState::OFF;
DeviceState Heater_State = DeviceState::OFF;

// This is the one web handler for changing a device mode.
//
// The browser sends:
// /api/state?system=mist&mode=auto
//
// The handler changes the matching Arduino state and reports whether
// the requested system and mode were understood.
void Register_State_Controls() {
    server.on("/api/state", HTTP_POST, []() {
        String system = server.arg("system");
        String mode = server.arg("mode");
        bool stateChanged = false;

        if (system == "irrigation") {
            if (mode == "off") {
                Irrigation_State = SolenoidState::OFF;
                stateChanged = true;
            }
            else if (mode == "manual") {
                Irrigation_State = SolenoidState::MANUAL;
                stateChanged = true;
            }
            else if (mode == "schedule") {
                Irrigation_State = SolenoidState::SCHEDULE;
                stateChanged = true;
            }
            else if (mode == "auto") {
                Irrigation_State = SolenoidState::AUTO;
                stateChanged = true;
            }
        }
        else if (system == "mist") {
            if (mode == "off") {
                Mist_State = SolenoidState::OFF;
                stateChanged = true;
            }
            else if (mode == "manual") {
                Mist_State = SolenoidState::MANUAL;
                stateChanged = true;
            }
            else if (mode == "schedule") {
                Mist_State = SolenoidState::SCHEDULE;
                stateChanged = true;
            }
            else if (mode == "auto") {
                Mist_State = SolenoidState::AUTO;
                stateChanged = true;
            }
        }
        else if (system == "light") {
            if (mode == "off") {
                Light_State = LightState::OFF;
                stateChanged = true;
            }
            else if (mode == "manual") {
                Light_State = LightState::MANUAL;
                stateChanged = true;
            }
            else if (mode == "schedule") {
                Light_State = LightState::SCHEDULE;
                stateChanged = true;
            }
        }
        else if (system == "fan") {
            if (mode == "off") {
                Fan_State = DeviceState::OFF;
                stateChanged = true;
            }
            else if (mode == "auto") {
                Fan_State = DeviceState::AUTO;
                stateChanged = true;
            }
            else if (mode == "manual") {
                Fan_State = DeviceState::MANUAL;
                stateChanged = true;
            }
        }
        else if (system == "heater") {
            if (mode == "off") {
                Heater_State = DeviceState::OFF;
                stateChanged = true;
            }
            else if (mode == "auto") {
                Heater_State = DeviceState::AUTO;
                stateChanged = true;
            }
            else if (mode == "manual") {
                Heater_State = DeviceState::MANUAL;
                stateChanged = true;
            }
        }

        if (stateChanged) {
            server.send(200, "text/plain", "State updated");
        }
        else {
            server.send(400, "text/plain", "Unknown system or mode");
        }
    });
}
