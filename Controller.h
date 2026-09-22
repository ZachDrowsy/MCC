#pragma once

#include <Arduino.h>
#include "Auto_Control.h"
#include "Schedule.h"

// These enum types are defined in States.h.
// Their names and sizes are enough for the update declarations below.
enum class SolenoidState : uint8_t;
enum class LightState : uint8_t;
enum class DeviceState : uint8_t;

class Controller{
private:
    bool Should_Run = false;
    bool manual = false;
    Schedule schedule;
    AutoControl automatic;

public:
    // Constructor and method declarations only.
    // Their implementations will go in Controller.cpp.

    Controller();

    // Irrigation and mist: automatic control and scheduling.
    Controller(const AutoControl& A, const Schedule& S);

    // Light: scheduling.
    Controller(const Schedule& S);

    // Heater and fan: automatic control.
    Controller(const AutoControl& A);

    // Manual ON/OFF request. Selecting MANUAL alone does not request ON.
    bool setManual(bool on);
    bool getManual() const;

    // Ask for this device's final decision, not its physical relay status.
    bool getShouldRun() const;

    // Settings entered by the user.
    // Return true only when accepted; false leaves the old settings unchanged.
    // Hours: 0-23, minutes: 0-59, duration: 1-1440 minutes.
    // The current Schedule class does not support runs crossing midnight.
    bool setSchedule(int hour, int minute, int durationMinutes);

    // Sunday = 0 through Saturday = 6.
    bool setScheduleDay(int day, bool enabled);

    // Both values must be finite, and lower must be less than upper.
    // The sensor reading and thresholds must use the same units.
    bool setAutomaticThresholds(float lower, float upper);

    // true: turn on below the lower threshold (for example, heating).
    // false: turn on above the upper threshold (for example, cooling).
    bool setTurnOnBelow(bool enabled);

    // Incoming webpage text: reject empty, malformed, or overflowing numbers.
    // Use these BEFORE any toInt(), toFloat(), or bool conversion.
    // Booleans accept only "true", "false", "1", or "0".
    bool setManualFromText(const String& text);
    bool setScheduleFromText(const String& hour, const String& minute,
                             const String& durationMinutes);
    bool setScheduleDayFromText(const String& day, const String& enabled);
    bool setAutomaticThresholdsFromText(const String& lower, const String& upper);
    bool setTurnOnBelowFromText(const String& text);

    // Read settings back without allowing outside code to change the members.
    const Schedule& getSchedule() const;
    const AutoControl& getAutomatic() const;

    // Use the selected mode already stored in States.h.
    // These three versions match the modes supported by each device.
    void update(SolenoidState mode, float reading); // Irrigation or mist.
    void update(LightState mode);                 // Light.
    void update(DeviceState mode, float reading); // Heater or fan.

    // The shared pump will use the valve controllers' getShouldRun() results.
    // Network requests, typing delays, and receipt indicators belong outside
    // this class. Accepting a setting does not save it across restarts.
};
