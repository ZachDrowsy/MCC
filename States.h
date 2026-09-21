#pragma once

#include <Arduino.h>
#include "connection.h"

// The state names stay the same so the rest of the project can use them.
enum class SimpleSolenoidState : uint8_t {
    OFF,
    MANUAL,
    SCHEDULE,
    AUTO
};

enum class SimpleLightState : uint8_t {
    OFF,
    MANUAL,
    SCHEDULE
};

enum class SimpleDeviceState : uint8_t {
    OFF,
    AUTO,
    MANUAL
};

SimpleSolenoidState Irrigation_State = SimpleSolenoidState::OFF;
SimpleSolenoidState Mist_State = SimpleSolenoidState::OFF;
SimpleLightState Light_State = SimpleLightState::OFF;
SimpleDeviceState Fan_State = SimpleDeviceState::OFF;
SimpleDeviceState Heater_State = SimpleDeviceState::OFF;

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
                Irrigation_State = SimpleSolenoidState::OFF;
                stateChanged = true;
            }
            else if (mode == "manual") {
                Irrigation_State = SimpleSolenoidState::MANUAL;
                stateChanged = true;
            }
            else if (mode == "schedule") {
                Irrigation_State = SimpleSolenoidState::SCHEDULE;
                stateChanged = true;
            }
            else if (mode == "auto") {
                Irrigation_State = SimpleSolenoidState::AUTO;
                stateChanged = true;
            }
        }
        else if (system == "mist") {
            if (mode == "off") {
                Mist_State = SimpleSolenoidState::OFF;
                stateChanged = true;
            }
            else if (mode == "manual") {
                Mist_State = SimpleSolenoidState::MANUAL;
                stateChanged = true;
            }
            else if (mode == "schedule") {
                Mist_State = SimpleSolenoidState::SCHEDULE;
                stateChanged = true;
            }
            else if (mode == "auto") {
                Mist_State = SimpleSolenoidState::AUTO;
                stateChanged = true;
            }
        }
        else if (system == "light") {
            if (mode == "off") {
                Light_State = SimpleLightState::OFF;
                stateChanged = true;
            }
            else if (mode == "manual") {
                Light_State = SimpleLightState::MANUAL;
                stateChanged = true;
            }
            else if (mode == "schedule") {
                Light_State = SimpleLightState::SCHEDULE;
                stateChanged = true;
            }
        }
        else if (system == "fan") {
            if (mode == "off") {
                Fan_State = SimpleDeviceState::OFF;
                stateChanged = true;
            }
            else if (mode == "auto") {
                Fan_State = SimpleDeviceState::AUTO;
                stateChanged = true;
            }
            else if (mode == "manual") {
                Fan_State = SimpleDeviceState::MANUAL;
                stateChanged = true;
            }
        }
        else if (system == "heater") {
            if (mode == "off") {
                Heater_State = SimpleDeviceState::OFF;
                stateChanged = true;
            }
            else if (mode == "auto") {
                Heater_State = SimpleDeviceState::AUTO;
                stateChanged = true;
            }
            else if (mode == "manual") {
                Heater_State = SimpleDeviceState::MANUAL;
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
