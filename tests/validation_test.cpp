#include "Controller.h"
#include "InputValidation.h"
#include <cassert>
#include <climits>
#include <iostream>
#include <limits>

// Host-only test clock; no ESP32 or network is needed.
static bool clockReady = false;
static tm testTime = {};
bool getLocalTime(tm* result, uint32_t) {
    if (!clockReady) return false;
    *result = testTime;
    return true;
}

int main() {
    using namespace InputValidation;
    int integer = 17;
    const char* badIntegers[] = {"", "abc", "12abc", "1.5", "1e2", " ", " 9",
                                "9 ", "-", "+", "0x10", "--1",
                                "2147483648", "-2147483649",
                                "999999999999999999999999999999999999"};
    for (const char* text : badIntegers) {
        assert(!readInteger(text, integer));
        assert(integer == 17);
    }
    assert(readInteger("0", integer) && integer == 0);
    assert(readInteger("+23", integer) && integer == 23);
    assert(readInteger("-1", integer) && integer == -1);
    assert(readInteger("2147483647", integer) && integer == INT_MAX);
    assert(readInteger("-2147483648", integer) && integer == INT_MIN);

    float decimal = 17.0f;
    const char* badDecimals[] = {"", "abc", "12abc", "45%", "NaN", "inf", "-inf",
                                ".", "-", "1.2.3", "1e2", " 1", "1 ",
                                "999999999999999999999999999999999999"};
    for (const char* text : badDecimals) {
        assert(!readDecimal(text, decimal));
        assert(decimal == 17.0f);
    }
    assert(readDecimal("-12.5", decimal) && decimal == -12.5f);
    assert(readDecimal(".5", decimal) && decimal == 0.5f);
    assert(readDecimal("100", decimal) && decimal == 100.0f);

    bool enabled = true;
    const char* badBooleans[] = {"", "abc", "2", "-1", "truejunk", "FALSE", " true"};
    for (const char* text : badBooleans) {
        assert(!readBoolean(text, enabled));
        assert(enabled);
    }
    assert(readBoolean("false", enabled) && !enabled);
    assert(readBoolean("1", enabled) && enabled);
    assert(readBoolean("0", enabled) && !enabled);
    assert(readBoolean("true", enabled) && enabled);

    Schedule schedule;
    assert(schedule.setStartTime(9, 30)); // Allowed before setting a duration.
    assert(schedule.getDurationMinutes() == 0);
    assert(schedule.setDurationMinutes(10));
    assert(schedule.setDayEnabled(1, true));

    const int badTiming[][3] = {
        {-1, 30, 10}, {24, 30, 10}, {9, -1, 10}, {9, 60, 10},
        {9, 30, 0}, {9, 30, -1}, {0, 0, 1441}, {23, 59, 2},
        {INT_MAX, 0, 1}, {0, INT_MAX, 1}, {0, 0, INT_MAX}
    };
    for (const auto& timing : badTiming) {
        assert(!schedule.setTiming(timing[0], timing[1], timing[2]));
        assert(schedule.getStartHour() == 9);
        assert(schedule.getStartMinute() == 30);
        assert(schedule.getDurationMinutes() == 10);
        assert(schedule.isDayEnabled(1));
    }
    assert(!schedule.setStartTime(23, 59)); // Would cross midnight with 10 minutes.
    assert(!schedule.setDurationMinutes(1000));
    assert(!schedule.setDayEnabled(-1, true));
    assert(!schedule.setDayEnabled(7, true));
    assert(!schedule.isDayEnabled(-1));
    assert(!schedule.isDayEnabled(7));
    assert(schedule.setTiming(23, 59, 1)); // Ending at midnight is supported.
    assert(schedule.setTiming(0, 0, 1440));

    assert(!schedule.isActive()); // No clock.
    clockReady = true;
    testTime.tm_wday = 1;
    testTime.tm_hour = 23;
    testTime.tm_min = 59;
    assert(schedule.isActive());
    testTime.tm_wday = 2;
    assert(!schedule.isActive());
    assert(schedule.setTiming(9, 30, 10));
    testTime.tm_wday = 1;
    testTime.tm_hour = 9;
    testTime.tm_min = 30;
    assert(schedule.isActive());
    testTime.tm_min = 40;
    assert(!schedule.isActive());

    AutoControl automatic;
    assert(automatic.set_Thresholds(30, 50));
    assert(automatic.isActive(20)); // Already ON before an invalid edit.
    const float nan = std::numeric_limits<float>::quiet_NaN();
    const float infinity = std::numeric_limits<float>::infinity();
    const float badThresholds[][2] = {
        {50, 30}, {30, 30}, {nan, 50}, {30, nan},
        {infinity, 50}, {30, infinity}, {-infinity, 50}
    };
    for (const auto& thresholds : badThresholds) {
        assert(!automatic.set_Thresholds(thresholds[0], thresholds[1]));
        assert(automatic.get_Lower_Threshold() == 30);
        assert(automatic.get_Upper_Threshold() == 50);
        assert(automatic.get_Output()); // Invalid input must not reset the output.
    }
    assert(automatic.isActive(40)); // Hysteresis still works.
    assert(!automatic.isActive(50));
    assert(!automatic.isActive(nan));
    assert(automatic.set_Turn_On_Below(false));
    assert(automatic.isActive(60));
    assert(automatic.isActive(40));
    assert(!automatic.isActive(30));
    assert(automatic.set_Thresholds(-10, -5)); // Negative temperatures are valid.

    Controller device;
    assert(device.setManual(true));
    assert(!device.setManualFromText("garbage"));
    assert(device.getManual());
    assert(device.setManualFromText("false"));
    assert(!device.getManual());
    assert(device.setManualFromText("true"));

    assert(device.setScheduleFromText("9", "30", "10"));
    assert(!device.setScheduleFromText("10", "30", "abc"));
    assert(!device.setScheduleFromText("10.5", "30", "10"));
    assert(!device.setScheduleFromText("24", "0", "10"));
    assert(!device.setSchedule(23, 59, 10));
    assert(device.getSchedule().getStartHour() == 9);
    assert(device.getSchedule().getStartMinute() == 30);
    assert(device.getSchedule().getDurationMinutes() == 10);
    assert(device.getManual()); // Editing a schedule must not change modes/requests.

    assert(device.setScheduleDayFromText("1", "true"));
    assert(!device.setScheduleDayFromText("1", "off"));
    assert(!device.setScheduleDayFromText("7", "true"));
    assert(!device.setScheduleDayFromText("abc", "true"));
    assert(device.getSchedule().isDayEnabled(1));

    assert(device.setAutomaticThresholdsFromText("30.5", "50.5"));
    assert(!device.setAutomaticThresholdsFromText("31.5", "oops"));
    assert(!device.setAutomaticThresholdsFromText("60", "50"));
    assert(!device.setAutomaticThresholdsFromText("NaN", "50"));
    assert(!device.setAutomaticThresholds(nan, 50));
    assert(device.getAutomatic().get_Lower_Threshold() == 30.5f);
    assert(device.getAutomatic().get_Upper_Threshold() == 50.5f);
    assert(device.setTurnOnBelowFromText("false"));
    assert(!device.setTurnOnBelowFromText("2"));

    std::cout << "All validation tests passed.\n";
}
