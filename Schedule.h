#pragma once

#include <Arduino.h>
#include <time.h>

class Schedule {
private:
    int startHour;
    int startMinute;
    int durationMinutes;
    bool enabledDays[7];

public:
    Schedule();

    void setStartTime(int hour, int minute);
    void setDurationMinutes(int minutes);
    void setDayEnabled(int day, bool enabled);

    int getStartHour() const;
    int getStartMinute() const;
    int getDurationMinutes() const;
    bool isDayEnabled(int day) const;

    bool isActive() const;
};
