#pragma once

#include <Arduino.h>
#include <time.h>

class Schedule {
private:
    int startHour;
    int startMinute;
    int durationMinutes;
    bool enabledDays[7];

    bool isValidTiming(int hour, int minute, int minutes) const;

public:
    Schedule();

    // Invalid settings return false and leave the old settings unchanged.
    // Hours: 0-23, minutes: 0-59, duration: 1-1440.
    // A run may end at midnight, but cannot cross into the following day.
    bool setTiming(int hour, int minute, int minutes);
    bool setStartTime(int hour, int minute);
    bool setDurationMinutes(int minutes);
    bool setDayEnabled(int day, bool enabled); // Sunday = 0, Saturday = 6.

    int getStartHour() const;
    int getStartMinute() const;
    int getDurationMinutes() const;
    bool isDayEnabled(int day) const;

    bool isActive() const;
};
