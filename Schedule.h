#pragma once
#include <time.h>



class Schedule
{
private:
    int start_Hr;
    int start_Min;
    int duration_Min;
    bool days[7];

public:
    Schedule() {
        start_Hr = 0;
        start_Min = 0;
        duration_Min = 0;

        for (int i = 0; i < 7; i++){
            days[i] = false;
        }
    }

    void setStart_Time(int hour, int minute){
        start_Hr = hour;
        start_Min = minute;
    }

    void set_Duration(int minutes){
        duration_Min = minutes;
    }

    void set_Day(int day, bool enabled){
        days[day] = enabled;
    }

    bool get_Day(int day) const{
        return days[day];
    }

    int get_Start_Hour() const{
        return start_Hr;
    }

    int get_Start_Minute() const{
        return start_Min;
    }

    int get_Duration() const{
        return duration_Min;
    }

    bool isActive() const{
        struct tm currentTime;

        if (!getLocalTime(&currentTime)){
            return false;
        }

        if (!days[currentTime.tm_wday]){
            return false;
        }

        int current_Minutes =
            currentTime.tm_hour * 60 +
            currentTime.tm_min;

        int start_Minutes =
            start_Hr * 60 +
            start_Min;

        int end_Minutes =
            start_Minutes + duration_Min;

        return current_Minutes >= start_Minutes &&
               current_Minutes < end_Minutes;
    }
};