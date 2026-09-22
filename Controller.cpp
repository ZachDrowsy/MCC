#include "Controller.h"
#include "InputValidation.h"

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

// Check ALL incoming fields before changing any member.

bool Controller::setManualFromText(const String& text) {
    bool choice;
    if (!InputValidation::readBoolean(text, choice)) {
        return false;
    }
    return setManual(choice);
}

bool Controller::setScheduleFromText(const String& hour, const String& minute,
                                      const String& durationMinutes) {
    int newHour;
    int newMinute;
    int newDuration;
    if (!InputValidation::readInteger(hour, newHour) ||
        !InputValidation::readInteger(minute, newMinute) ||
        !InputValidation::readInteger(durationMinutes, newDuration)) {
        return false;
    }
    return setSchedule(newHour, newMinute, newDuration);
}

bool Controller::setScheduleDayFromText(const String& day, const String& enabled) {
    int newDay;
    bool newEnabled;
    if (!InputValidation::readInteger(day, newDay) ||
        !InputValidation::readBoolean(enabled, newEnabled)) {
        return false;
    }
    return setScheduleDay(newDay, newEnabled);
}

bool Controller::setAutomaticThresholdsFromText(const String& lower, const String& upper) {
    float newLower;
    float newUpper;
    if (!InputValidation::readDecimal(lower, newLower) ||
        !InputValidation::readDecimal(upper, newUpper)) {
        return false;
    }
    return setAutomaticThresholds(newLower, newUpper);
}

bool Controller::setTurnOnBelowFromText(const String& text) {
    bool newEnabled;
    if (!InputValidation::readBoolean(text, newEnabled)) {
        return false;
    }
    return setTurnOnBelow(newEnabled);
}

const Schedule& Controller::getSchedule() const {
    return schedule;
}

const AutoControl& Controller::getAutomatic() const {
    return automatic;
}

// Device behavior will be implemented separately.
/*
void Controller::update(SolenoidState mode, float reading) {
    // TODO
}

void Controller::update(LightState mode) {
    // TODO
}

void Controller::update(DeviceState mode, float reading) {
    // TODO
}
*/
