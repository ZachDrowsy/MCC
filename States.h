#pragma once

#include <Arduino.h>
#include "connection.h"  // Declares server and includes web_ui.h.
/* Function calls:
Update_Irrigation(); for attatching the actual state of Irrigation from user interaction
Update_Mist(); user interaction to change Mist State
Update_Light(); user interaction to change Light state
Update_Fan(); user interation to change fan state
Update_Heater(); User interaction to change heater state. 

*/

enum class Pump : uint8_t {
    ON,
    OFF
};

enum class Sol_State : uint8_t {
    OFF,
    MANUAL,
    SCHEDULE,
    AUTO
};

Sol_State Irrigation_State = Sol_State::OFF;
Sol_State Mist_State = Sol_State::OFF;

void Update_Irrigation() {
    server.on("/api/irrigation-off", HTTP_POST, []() {
        Irrigation_State = Sol_State::OFF;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/irrigation-manual", HTTP_POST, []() {
        Irrigation_State = Sol_State::MANUAL;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/irrigation-schedule", HTTP_POST, []() {
        Irrigation_State = Sol_State::SCHEDULE;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/irrigation-auto", HTTP_POST, []() {
        Irrigation_State = Sol_State::AUTO;
        server.send(200, "text/plain", "OK");
    });
}

void Update_Mist() {
    server.on("/api/mist-off", HTTP_POST, []() {
        Mist_State = Sol_State::OFF;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/mist-manual", HTTP_POST, []() {
        Mist_State = Sol_State::MANUAL;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/mist-schedule", HTTP_POST, []() {
        Mist_State = Sol_State::SCHEDULE;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/mist-auto", HTTP_POST, []() {
        Mist_State = Sol_State::AUTO;
        server.send(200, "text/plain", "OK");
    });
}

enum class Light : uint8_t {
    OFF,
    MANUAL,
    SCHEDULE
};

Light Light_State = Light::OFF;

void Update_Light() {
    server.on("/api/light-off", HTTP_POST, []() {
        Light_State = Light::OFF;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/light-manual", HTTP_POST, []() {
        Light_State = Light::MANUAL;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/light-schedule", HTTP_POST, []() {
        Light_State = Light::SCHEDULE;
        server.send(200, "text/plain", "OK");
    });
}

enum class Fan : uint8_t {
    OFF,
    AUTO,
    MANUAL
};

Fan Fan_State = Fan::OFF;

void Update_Fan() {
    server.on("/api/fan-off", HTTP_POST, []() {
        Fan_State = Fan::OFF;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/fan-auto", HTTP_POST, []() {
        Fan_State = Fan::AUTO;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/fan-manual", HTTP_POST, []() {
        Fan_State = Fan::MANUAL;
        server.send(200, "text/plain", "OK");
    });
}

enum class Heater : uint8_t {
    OFF,
    AUTO,
    MANUAL
};

Heater Heater_State = Heater::OFF;

void Update_Heater() {
    server.on("/api/heater-off", HTTP_POST, []() {
        Heater_State = Heater::OFF;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/heater-auto", HTTP_POST, []() {
        Heater_State = Heater::AUTO;
        server.send(200, "text/plain", "OK");
    });

    server.on("/api/heater-manual", HTTP_POST, []() {
        Heater_State = Heater::MANUAL;
        server.send(200, "text/plain", "OK");
    });
}
void Update_

