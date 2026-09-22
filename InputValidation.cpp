#include "InputValidation.h"
#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdlib>

bool InputValidation::readInteger(const String& text, int& result) {
    if (text.length() == 0 || text.length() > 32) {
        return false;
    }

    unsigned int position = 0;
    if (text[0] == '-' || text[0] == '+') {
        position = 1;
    }
    if (position == text.length()) {
        return false;
    }

    // Do not accept partial numbers like "12abc" or decimals like "9.5".
    for (; position < text.length(); position++) {
        if (text[position] < '0' || text[position] > '9') {
            return false;
        }
    }

    errno = 0;
    char* end;
    long value = std::strtol(text.c_str(), &end, 10);
    if (errno == ERANGE || *end != '\0' || value < INT_MIN || value > INT_MAX) {
        return false;
    }

    result = static_cast<int>(value);
    return true;
}

bool InputValidation::readDecimal(const String& text, float& result) {
    if (text.length() == 0 || text.length() > 32) {
        return false;
    }

    unsigned int position = 0;
    bool hasDigit = false;
    bool hasPoint = false;
    if (text[0] == '-' || text[0] == '+') {
        position = 1;
    }

    // Accept ordinary decimals, including negatives. No units or exponent text.
    for (; position < text.length(); position++) {
        char letter = text[position];
        if (letter >= '0' && letter <= '9') {
            hasDigit = true;
        }
        else if (letter == '.' && !hasPoint) {
            hasPoint = true;
        }
        else {
            return false;
        }
    }
    if (!hasDigit) {
        return false;
    }

    errno = 0;
    char* end;
    float value = std::strtof(text.c_str(), &end);
    if (errno == ERANGE || *end != '\0' || !std::isfinite(value)) {
        return false;
    }

    result = value;
    return true;
}

bool InputValidation::readBoolean(const String& text, bool& result) {
    if (text == "true" || text == "1") {
        result = true;
        return true;
    }
    if (text == "false" || text == "0") {
        result = false;
        return true;
    }
    return false;
}
