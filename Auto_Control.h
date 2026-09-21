
#pragma once
#include <cmath>

class AutoControl {
private:
    float lower_Threshold;
    float upper_Threshold;
    bool turn_On_Below;
    bool output_On;

public:
    AutoControl() {
        lower_Threshold = 0;
        upper_Threshold = 0;
        turn_On_Below = true;
        output_On = false;
    }

    void set_Thresholds(float lower, float upper) {
        lower_Threshold = lower;
        upper_Threshold = upper;
        output_On = false;
    }

    void set_Turn_On_Below(bool enabled) {
        turn_On_Below = enabled;
        output_On = false;
    }

    float get_Lower_Threshold() const {
        return lower_Threshold;
    }

    float get_Upper_Threshold() const {
        return upper_Threshold;
    }

    bool get_Output() const {
        return output_On;
    }

    void reset() {
        output_On = false;
    }

    bool isActive(float reading) {
        if (!std::isfinite(reading) ||
            !std::isfinite(lower_Threshold) ||
            !std::isfinite(upper_Threshold) ||
            lower_Threshold >= upper_Threshold) {
            output_On = false;
            return false;
        }

        if (turn_On_Below) {
            if (reading < lower_Threshold) {
                output_On = true;
            }
            else if (reading >= upper_Threshold) {
                output_On = false;
            }
        }
        else {
            if (reading > upper_Threshold) {
                output_On = true;
            }
            else if (reading <= lower_Threshold) {
                output_On = false;
            }
        }

        return output_On;
    }
};