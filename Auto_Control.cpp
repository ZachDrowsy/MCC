#include "Auto_Control.h"
#include <cmath>

AutoControl::AutoControl() {
    lower_Threshold = 0;
    upper_Threshold = 0;
    turn_On_Below = true;
    output_On = false;
}

void AutoControl::set_Thresholds(float lower, float upper) {
    lower_Threshold = lower;
    upper_Threshold = upper;
    output_On = false;
}

void AutoControl::set_Turn_On_Below(bool enabled) {
    turn_On_Below = enabled;
    output_On = false;
}

float AutoControl::get_Lower_Threshold() const {
    return lower_Threshold;
}

float AutoControl::get_Upper_Threshold() const {
    return upper_Threshold;
}

bool AutoControl::get_Output() const {
    return output_On;
}

void AutoControl::reset() {
    output_On = false;
}

bool AutoControl::isActive(float reading) {
    // A missing or invalid sensor reading must leave the output off.
    if (!std::isfinite(reading)) {
        output_On = false;
        return false;
    }

    // Both thresholds must be valid numbers.
    if (!std::isfinite(lower_Threshold) || !std::isfinite(upper_Threshold)) {
        output_On = false;
        return false;
    }

    // The lower threshold must come before the upper threshold.
    if (lower_Threshold >= upper_Threshold) {
        output_On = false;
        return false;
    }

    // Used for moisture, humidity, or heating: turn on when the reading is low.
    if (turn_On_Below) {
        if (reading < lower_Threshold) {
            output_On = true;
        }
        else if (reading >= upper_Threshold) {
            output_On = false;
        }
    }
    // Used for cooling: turn on when the reading is high.
    else {
        if (reading > upper_Threshold) {
            output_On = true;
        }
        else if (reading <= lower_Threshold) {
            output_On = false;
        }
    }

    // Between the thresholds, keep the previous on/off state.
    return output_On;
}
