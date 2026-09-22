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
/*
// Manual request and final decision

void Controller::setManual(bool on) {
    // TODO
}

bool Controller::getManual() const {
    // TODO
}

bool Controller::getShouldRun() const {
    // TODO
}

// User settings

bool Controller::setSchedule(int hour, int minute, int durationMinutes) {
    // TODO
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
