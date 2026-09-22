# Settings validation

A successful setter returns `true`. An invalid setting returns `false`
and keeps the previous values. A successful change lives in RAM; it is not
yet saved across restarts.

## Numeric settings

- `Schedule::setTiming(hour, minute, duration)` validates the whole timing
  change before storing anything. Hours are 0–23, minutes 0–59, and duration
  1–1440 minutes. Ending at midnight is allowed; crossing midnight is not.
- The separate start-time and duration setters also check the resulting
  time window. A newly constructed schedule has duration zero (not configured).
- Days are 0–6 (Sunday–Saturday). Invalid day setters return false.
- `AutoControl::set_Thresholds(lower, upper)` requires finite numbers and
  lower < upper. Rejected edits preserve both thresholds and the previous
  output decision.
- Automatic thresholds use the same units as their readings. This generic
  class supports temperatures, percentages, and raw ADC values, so it does
  not impose a universal 0–100 range. Device-specific limits must be chosen
  when those readings and settings are connected.

## Text from a future web request

C++ numeric arguments are already converted by the time a numeric setter
receives them. A bool parameter, for example, cannot tell whether its caller
originally passed 2 or true. Do not pass raw text to bool setters and do not
use a conversion that silently turns invalid text into zero.

Use the controller's `FromText` methods for form values:

```cpp
bool accepted = irrigation.setScheduleFromText("9", "30", "10");
bool rejected = irrigation.setScheduleFromText("abc", "30", "10"); // false
```

- Whole numbers accept digits and an optional initial + or -.
- Decimal numbers also allow one decimal point.
- Empty strings, spaces, trailing letters/units, overflow, NaN, infinity,
  hexadecimal and exponent notation are rejected.
- Integer fields reject fractional input rather than truncating it.
- Boolean text accepts exactly true, false, 1 or 0.
- All fields in one call are parsed before any settings are changed.

The numeric rules belong in Schedule and AutoControl. InputValidation only
checks text conversion; Controller passes the parsed settings to their owners.
The existing /api/state handler already rejects unknown device/mode pairs.

These methods prepare validation for web requests. No settings endpoint or
automatic browser saving has been added in this change. The update methods
remain unfinished and commented out.

## Host regression tests

With a C++11 compiler available, run from the repository root:

```sh
g++ -std=c++11 -Wall -Wextra -Werror -Itests/stubs -I. tests/validation_test.cpp Schedule.cpp Auto_Control.cpp Controller.cpp InputValidation.cpp -o validation_test
./validation_test
```

On Windows, use validation_test.exe as the output filename and run it.
The tests use a small Arduino stand-in and a controlled clock. They check
parsing, boundary values, rejected updates preserving old settings, and
the existing schedule/automatic decision behavior. They do not replace an
ESP32 build or hardware test.
