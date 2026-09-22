#pragma once

#include <Arduino.h>

// Text received from a webpage must be checked BEFORE numeric conversion.
// These functions return false without changing the result on failure.
// Use the Controller methods ending in FromText for incoming form values.
namespace InputValidation {
    bool readInteger(const String& text, int& result);
    bool readDecimal(const String& text, float& result);
    bool readBoolean(const String& text, bool& result);
}
