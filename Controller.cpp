#include "Controller.h"

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

// Remaining methods will be implemented one at a time.

// Manual request and final decision

void Controller::setManual(bool choice) {
    manual = choice;
}

bool Controller::getManual() const {
    return manual;
}

bool Controller::getShouldRun() const {
    return Should_Run;
}

// User settings
// Set the starting time and duration
bool Controller::setSchedule(const Controller& device, int hour, int minute, int durationMinutes) {
    schedule.setStartTime(hour, minute);
    schedule.setDurataionMinutes(durationMinutes);
    return device.setManual(false);
}

bool Controller::setScheduleDay(int day, bool enabled) {
    // TODO
}

bool Controller::setAutomaticThresholds(float lower, float upper) {
    // TODO
}

void Controller::setTurnOnBelow(bool enabled) {
    // TODO
}

// Read settings

const Schedule& Controller::getSchedule() const {
    // TODO
}

const AutoControl& Controller::getAutomatic() const {
    // TODO
}

// Update each kind of device

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
