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

void Controller::update(SolenoidState mode, float reading) {
    if (mode == SolenoidState::OFF) {
        Should_Run = false;
    }
    else if (mode == SolenoidState::MANUAL) {
        Should_Run = manual;
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
        Should_Run = manual;
    }
    else if (mode == LightState::SCHEDULE) {
        Should_Run = schedule.isActive();
    }
}

void Controller::update(DeviceState mode, float reading) {
    if (mode == DeviceState::OFF) {
        Should_Run = false;
    }
    else if (mode == DeviceState::MANUAL) {
        Should_Run = manual;
    }
    else if (mode == DeviceState::AUTO) {
        Should_Run = automatic.isActive(reading);
    }
}
