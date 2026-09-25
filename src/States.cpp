#include "../include/States.h"
#include <WebServer.h>

Pump Pump_State = Pump::OFF;

SolenoidState Irrigation_State = SolenoidState::MANUAL;
SolenoidState Mist_State = SolenoidState::MANUAL;
LightState Light_State = LightState::MANUAL;
DeviceState Fan_State = DeviceState::MANUAL;
DeviceState Heater_State = DeviceState::MANUAL;

Activate Irrigation_Activation = Activate::OFF;
Activate Mist_Activation = Activate::OFF;
Activate Light_Activation = Activate::OFF;
Activate Fan_Activation = Activate::OFF;
Activate Heater_Activation = Activate::OFF;

// Mode buttons send /api/state?system=mist&mode=auto.
// On/Off buttons send /api/activation?system=mist&activation=on.
// Changing a mode never changes activation.
void Register_State_Controls(WebServer& server) {
    server.on("/api/state", HTTP_POST, [&server]() {
        String system = server.arg("system");
        String mode = server.arg("mode");
        bool stateChanged = false;

        if (system == "irrigation") {
            if (mode == "manual") {
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
            if (mode == "manual") {
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
            if (mode == "manual") {
                Light_State = LightState::MANUAL;
                stateChanged = true;
            }
            else if (mode == "schedule") {
                Light_State = LightState::SCHEDULE;
                stateChanged = true;
            }
        }
        else if (system == "fan") {
            if (mode == "manual") {
                Fan_State = DeviceState::MANUAL;
                stateChanged = true;
            }
            else if (mode == "auto") {
                Fan_State = DeviceState::AUTO;
                stateChanged = true;
            }
        }
        else if (system == "heater") {
            if (mode == "manual") {
                Heater_State = DeviceState::MANUAL;
                stateChanged = true;
            }
            else if (mode == "auto") {
                Heater_State = DeviceState::AUTO;
                stateChanged = true;
            }
        }

        if (stateChanged) {
            server.send(200, "text/plain", "Mode updated");
        }
        else {
            server.send(400, "text/plain", "Unknown system or mode");
        }
    });

    server.on("/api/activation", HTTP_POST, [&server]() {
        String system = server.arg("system");
        String activation = server.arg("activation");

        if (activation != "on" && activation != "off") {
            server.send(400, "text/plain", "Activation must be on or off");
            return;
        }

        Activate requested = Activate::OFF;
        if (activation == "on") {
            requested = Activate::ON;
        }

        if (system == "irrigation") {
            Irrigation_Activation = requested;
        }
        else if (system == "mist") {
            Mist_Activation = requested;
        }
        else if (system == "light") {
            Light_Activation = requested;
        }
        else if (system == "fan") {
            Fan_Activation = requested;
        }
        else if (system == "heater") {
            Heater_Activation = requested;
        }
        else {
            server.send(400, "text/plain", "Unknown system");
            return;
        }

        server.send(200, "text/plain", "Activation updated");
    });

    // Let a newly opened webpage display the states already on the ESP32.
    server.on("/api/state", HTTP_GET, [&server]() {
        String system = server.arg("system");
        String mode;
        Activate activation = Activate::OFF;

        if (system == "irrigation") {
            activation = Irrigation_Activation;
            if (Irrigation_State == SolenoidState::MANUAL) {
                mode = "manual";
            }
            else if (Irrigation_State == SolenoidState::SCHEDULE) {
                mode = "schedule";
            }
            else if (Irrigation_State == SolenoidState::AUTO) {
                mode = "auto";
            }
        }
        else if (system == "mist") {
            activation = Mist_Activation;
            if (Mist_State == SolenoidState::MANUAL) {
                mode = "manual";
            }
            else if (Mist_State == SolenoidState::SCHEDULE) {
                mode = "schedule";
            }
            else if (Mist_State == SolenoidState::AUTO) {
                mode = "auto";
            }
        }
        else if (system == "light") {
            activation = Light_Activation;
            if (Light_State == LightState::MANUAL) {
                mode = "manual";
            }
            else if (Light_State == LightState::SCHEDULE) {
                mode = "schedule";
            }
        }
        else if (system == "fan") {
            activation = Fan_Activation;
            if (Fan_State == DeviceState::MANUAL) {
                mode = "manual";
            }
            else if (Fan_State == DeviceState::AUTO) {
                mode = "auto";
            }
        }
        else if (system == "heater") {
            activation = Heater_Activation;
            if (Heater_State == DeviceState::MANUAL) {
                mode = "manual";
            }
            else if (Heater_State == DeviceState::AUTO) {
                mode = "auto";
            }
        }
        else {
            server.send(400, "text/plain", "Unknown system");
            return;
        }

        String data = "{\"mode\":\"" + mode + "\",\"activation\":\"";
        if (activation == Activate::ON) {
            data += "on";
        }
        else {
            data += "off";
        }
        data += "\"}";
        server.sendHeader("Cache-Control", "no-store");
        server.send(200, "application/json", data);
    });
}
