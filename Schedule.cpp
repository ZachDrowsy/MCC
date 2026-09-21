#include "Schedule.h"

Schedule::Schedule() {
    startHour = 0;
    startMinute = 0;
    durationMinutes = 0;

    for (int day = 0; day < 7; day++) {
        enabledDays[day] = false;
    }
}

void Schedule::setStartTime(int hour, int minute) {
    startHour = hour;
    startMinute = minute;
}

void Schedule::setDurationMinutes(int minutes) {
    durationMinutes = minutes;
}

void Schedule::setDayEnabled(int day, bool enabled) {
    if (day < 0 || day > 6) {
        return;
    }

    enabledDays[day] = enabled;
}

int Schedule::getStartHour() const {
    return startHour;
}

int Schedule::getStartMinute() const {
    return startMinute;
}

int Schedule::getDurationMinutes() const {
    return durationMinutes;
}

bool Schedule::isDayEnabled(int day) const {
    if (day < 0 || day > 6) {
        return false;
    }

    return enabledDays[day];
}

bool Schedule::isActive() const {
    struct tm currentTime;

    if (!getLocalTime(&currentTime)) {
        return false;
    }

    if (!isDayEnabled(currentTime.tm_wday)) {
        return false;
    }

    int currentMinutes =
        currentTime.tm_hour * 60 +
        currentTime.tm_min;

    int startMinutes =
        startHour * 60 +
        startMinute;

    int endMinutes =
        startMinutes + durationMinutes;

    return currentMinutes >= startMinutes &&
           currentMinutes < endMinutes;
}
