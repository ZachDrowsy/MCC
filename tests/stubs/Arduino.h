#pragma once

// Minimal stand-in for the Arduino features used by these settings classes.
// Only used by the host tests through their include path.
#include <cstdint>
#include <ctime>
#include <string>

using String = std::string;
bool getLocalTime(tm* result, uint32_t timeout = 5000);
