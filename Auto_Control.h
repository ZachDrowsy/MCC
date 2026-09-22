#pragma once

// Decides whether an output should be on. Relay control is handled separately.
class AutoControl {
private:
    float lower_Threshold;
    float upper_Threshold;
    bool turn_On_Below;
    bool output_On;

public:
    AutoControl();

    // Reject non-finite or unordered thresholds without changing any state.
    bool set_Thresholds(float lower, float upper);
    // true: turn on below the lower threshold. false: turn on above the upper.
    bool set_Turn_On_Below(bool enabled);

    float get_Lower_Threshold() const;
    float get_Upper_Threshold() const;
    bool get_Output() const;

    void reset();
    bool isActive(float reading);
};
