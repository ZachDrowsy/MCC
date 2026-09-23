#include "../include/Controller.h"

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
        // The pump should turn on here.
        // digitalWrite(PUMP_PIN, HIGH);

        // The irrigation solenoid should open here.
        // digitalWrite(IRRIGATION_VALVE_PIN, HIGH);
    }
    else {
        // The pump should turn off here.
        // digitalWrite(PUMP_PIN, LOW);

        // The irrigation solenoid should close here.
        // digitalWrite(IRRIGATION_VALVE_PIN, LOW);
    }
}
void Controller::Run_Mist(Pump p_state) {
    if (p_state == Pump::ON) {
        // The pump should turn on here.
        // digitalWrite(PUMP_PIN, HIGH);

        // The irrigation solenoid should open here.
        // digitalWrite(MIST VALVE PIN, HIGH);
    }
    else {
        // The pump should turn off here.
        // digitalWrite(PUMP_PIN, LOW);

        // The irrigation solenoid should close here.
        // digitalWrite(MIST VALVE_PIN, LOW);
    }
}
void Controller::Run_Light() {
    if (Should_Run) {
        // Turn the light on here.
    }
    else {
        // Turn the light off here.
    }
}
void Controller::Run_Heater() {
    if (Should_Run) {
        // Turn the heater on here.
    }
    else {
        // Turn the heater off here.
    }
}

void Controller::Run_Fan() {
    if (Should_Run) {
        // Turn the fan on here.
    }
    else {
        // Turn the fan off here.
    }
}
