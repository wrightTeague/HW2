# HW2: TimeCode

**Author:** Teague Wright
**Course:** CS-222

A C++ `TimeCode` class that represents an amount of time as hours, minutes, and seconds (e.g. `4:15:32`). It stores only one value, `t`, the total number of seconds (`4:15:32` → `t = 15332`).

## Build and run

```
make
./tct
```

`make` compiles the program into an executable named `tct`, which runs all the tests. `make clean` removes the executable.

## Files

| File | Purpose |
|---|---|
| `TimeCode.h` | Class declaration (retyped from the provided screenshot) |
| `TimeCode.cpp` | Implementation of every method in `TimeCode.h` |
| `TimeCodeTests.cpp` | Tests for every method, plus `main()` to run them |
| `Makefile` | Builds `tct` |

## Design

- **Only `t` is stored.** Hours, minutes, and seconds are always calculated from `t`, never stored.
- **Conversions happen in two methods only.** `ComponentsToSeconds()` (static) turns hr/min/sec into `t`, and `GetComponents()` turns `t` back into hr/min/sec. Every other method, including the constructor, getters, and setters, uses these two instead of repeating the math.
- **Rollover vs. exceptions:**
  - The constructor and arithmetic **roll over** out-of-range values (`TimeCode(0, 61, 0)` becomes `1:1:0`).
  - `SetMinutes()` and `SetSeconds()` **throw** `invalid_argument` for values over 59, and leave the time unchanged.
  - Anything that would make a TimeCode negative **throws**: subtracting a larger time, multiplying or dividing by a negative number, and dividing by zero.
- Hours have no upper limit. `ToString()` uses `h:m:s` with no zero-padding (e.g. `8:5:9`).
- `<cmath>` is not used.

## Tests

`TimeCodeTests.cpp` has a test function for every method. They cover:

- normal values
- boundaries (0, 59, 60)
- rollover
- very large hours and seconds, including the spec's `TimeCode(2, 71, 234719572143)`
- every case that should throw
- checks that operators don't change the original TimeCodes

## Citations

I used Claude (Anthropic's AI assistant) to review my code, explain C++ concepts (constructors, pass by reference, integer division), and help write and comment some of the tests.
