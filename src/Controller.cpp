#include "../include/Controller.h"

const uint8_t IRRIGATION_VALVE_PIN = 33;
const uint8_t MIST_VALVE_PIN = 25;
const uint8_t FAN_PIN = 26;
const uint8_t LIGHT_PIN = 27; // The heat lamp uses the Light controller.

void Setup_Relays() {
    // Set the OFF level before enabling each output.
    digitalWrite(IRRIGATION_VALVE_PIN, RELAY_OFF);
    pinMode(IRRIGATION_VALVE_PIN, OUTPUT);

    digitalWrite(MIST_VALVE_PIN, RELAY_OFF);
    pinMode(MIST_VALVE_PIN, OUTPUT);

    digitalWrite(FAN_PIN, RELAY_OFF);
    pinMode(FAN_PIN, OUTPUT);

    digitalWrite(LIGHT_PIN, RELAY_OFF);
    pinMode(LIGHT_PIN, OUTPUT);
}

// The booleans start false as declared in Controller.h.
// Members not listed below use their own default constructors.

Controller::Controller() {
}

// Irrigation and mist: copy both sets of settings into this controller.
Controller::Controller(const AutoControl& A, const Schedule& S)
    : schedule(S), automatic(A) {
}

// Light: copy the schedule settings.
Controller::Controller(const Schedule& S)
    : schedule(S) {
}

// Heater and fan: copy the automatic settings.
Controller::Controller(const AutoControl& A)
    : automatic(A) {
}

// Manual request and final decision

bool Controller::setManual(bool choice) {
    manual = choice;
    return true;
}

bool Controller::getManual() const {
    return manual;
}

bool Controller::getShouldRun() const {
    return Should_Run;
}

// The classes that own these settings perform their range checks.

bool Controller::setSchedule(int hour, int minute, int durationMinutes) {
    return schedule.setTiming(hour, minute, durationMinutes);
}

bool Controller::setScheduleDay(int day, bool enabled) {
    return schedule.setDayEnabled(day, enabled);
}

bool Controller::setAutomaticThresholds(float lower, float upper) {
    return automatic.set_Thresholds(lower, upper);
}

bool Controller::setTurnOnBelow(bool enabled) {
    return automatic.set_Turn_On_Below(enabled);
}

const Schedule& Controller::getSchedule() const {
    return schedule;
}

const AutoControl& Controller::getAutomatic() const {
    return automatic;
}

// Device behavior
// in main we will check if object.getShouldRun is true that way it doesnt run if we get false = false;
void Controller::update(SolenoidState mode, float reading) {
    if (mode == SolenoidState::OFF) {
        Should_Run = false;
    }
    else if (mode == SolenoidState::MANUAL) {
        Should_Run = true;
    }
    else if (mode == SolenoidState::SCHEDULE) {
        Should_Run = schedule.isActive();
    }
    else if (mode == SolenoidState::AUTO) {
        Should_Run = automatic.isActive(reading);
    }
}


void Controller::update(LightState mode) {
    if (mode == LightState::OFF) {
        Should_Run = false;
    }
    else if (mode == LightState::MANUAL) {
        Should_Run = true; 
    }
    else if (mode == LightState::SCHEDULE) {
        Should_Run = schedule.isActive(); // schedule.is active
    }
}

void Controller::update(DeviceState mode, float reading) {
    if (mode == DeviceState::OFF) {
        Should_Run = false;
    }
    else if (mode == DeviceState::MANUAL) {
        
        Should_Run = true;
    }
    else if (mode == DeviceState::AUTO) {
        Should_Run = automatic.isActive(reading);
    }
}

void Controller::Run_Irrigation(Pump p_state) {
    if (p_state == Pump::ON) {
        // Open the irrigation valve.
        digitalWrite(IRRIGATION_VALVE_PIN, RELAY_ON);
    }
    else {
        // Close the irrigation valve.
        digitalWrite(IRRIGATION_VALVE_PIN, RELAY_OFF);
    }
}

void Controller::Run_Mist(Pump p_state) {
    if (p_state == Pump::ON) {
        // Open the mist valve.
        digitalWrite(MIST_VALVE_PIN, RELAY_ON);
    }
    else {
        // Close the mist valve.
        digitalWrite(MIST_VALVE_PIN, RELAY_OFF);
    }
}

void Controller::Run_Light() {
    if (Should_Run) {
        digitalWrite(LIGHT_PIN, RELAY_ON);
    }
    else {
        digitalWrite(LIGHT_PIN, RELAY_OFF);
    }
}
void Controller::Run_Heater() {
    // No separate heater is connected. The heat lamp uses Run_Light().
}

void Controller::Run_Fan() {
    if (Should_Run) {
        digitalWrite(FAN_PIN, RELAY_ON);
    }
    else {
        digitalWrite(FAN_PIN, RELAY_OFF);
    }
}
