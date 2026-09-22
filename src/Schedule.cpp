#include "../include/Schedule.h"

Schedule::Schedule() {
    startHour = 0;
    startMinute = 0;
    durationMinutes = 0;

    for (int day = 0; day < 7; day++) {
        enabledDays[day] = false;
    }
}

bool Schedule::isValidTiming(int hour, int minute, int minutes) const {
    if (hour < 0 || hour > 23) {
        return false;
    }
    if (minute < 0 || minute > 59) {
        return false;
    }
    // Zero is only used by a new schedule that has no duration yet.
    if (minutes < 0 || minutes > 1440) {
        return false;
    }

    int endMinutes = hour * 60 + minute + minutes;
    return endMinutes <= 1440;
}

bool Schedule::setTiming(int hour, int minute, int minutes) {
    if (minutes < 1 || !isValidTiming(hour, minute, minutes)) {
        return false;
    }

    // Change all three together, only after all checks pass.
    startHour = hour;
    startMinute = minute;
    durationMinutes = minutes;
    return true;
}

bool Schedule::setStartTime(int hour, int minute) {
    if (!isValidTiming(hour, minute, durationMinutes)) {
        return false;
    }

    startHour = hour;
    startMinute = minute;
    return true;
}

bool Schedule::setDurationMinutes(int minutes) {
    return setTiming(startHour, startMinute, minutes);
}

bool Schedule::setDayEnabled(int day, bool enabled) {
    if (day < 0 || day > 6) {
        return false;
    }

    enabledDays[day] = enabled;
    return true;
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
