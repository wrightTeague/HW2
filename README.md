# HW2: TimeCode, NASA Launch Analysis, and Paint Dry Timer

**Author:** Teague Wright
**Course:** CS-222

Three C++ programs built around a `TimeCode` class that represents an amount of time as hours, minutes, and seconds (for example `4:15:32`). A TimeCode stores only one value, `t`, the total number of seconds, so `4:15:32` is stored as `t = 15332`.

## Build

```
make
```

This builds all three programs. `make clean` deletes them.

| Program | Command | What it does |
|---|---|---|
| `tct` | `./tct` | Runs the TimeCode test suite |
| `nasa` | `./nasa` | Averages the time of day of every NASA launch in the CSV |
| `pdt` | `./pdt` | Interactive timer for batches of drying spheres |

## tct — TimeCode tests

```
./tct
```

Runs 18 test functions covering every method of `TimeCode`, and prints `PASSED ALL TESTS!!!` if they all pass. A failed test stops the program and names the line that failed.

## nasa — NASA Launch Analysis

```
./nasa
```

Reads `Space_Corrected.csv`, a table of space launches going back to the beginning of NASA, and extracts the UTC time of day from each one. Launches recorded with only a date and no time are skipped. The times are stored as a `vector<TimeCode>` and averaged using TimeCode's own `+` and `/` operators.

Output:

```
4198 data points.
AVERAGE: 12:7:56
```

## pdt — Paint Dry Timer

```
./pdt
```

Tracks batches of spheres while their paint dries. A batch takes one second per square centimetre of a sphere's surface area, and the whole batch dries at once, so the number of spheres doesn't matter.

The program runs its tests first, then shows a menu:

- **(A)dd** — asks for a radius, then starts timing a new batch
- **(V)iew Current Items** — lists every batch with the time it has left, and how many are being tracked
- **(Q)uit** — exits

A batch that has finished shows `DONE!` once and is dropped from the list after that. Example:

```
Choose an option: (A)dd, (V)iew Current Items, (Q)uit: a
        radius: 2
        Batch-16807 (takes 0:0:50 to dry) time remaining: 0:0:50
Choose an option: (A)dd, (V)iew Current Items, (Q)uit: v
        Batch-16807 (takes 0:0:50 to dry) time remaining: 0:0:44
        1 batches being tracked.
```

Each batch's TimeCode is allocated on the heap and deleted when the batch finishes, or when you quit while batches are still drying, so the program does not leak memory. Checked with:

```
valgrind --leak-check=full ./pdt
```

## Files

| File | Purpose |
|---|---|
| `TimeCode.h` | Class declaration |
| `TimeCode.cpp` | Implementation of every method in `TimeCode.h` |
| `TimeCodeTests.cpp` | Tests for every method, plus `main()` to run them |
| `NasaLaunchAnalysis.cpp` | The `nasa` program |
| `PaintDryTimer.cpp` | The `pdt` program |
| `Space_Corrected.csv` | Launch data read by `nasa` |
| `Makefile` | Builds all three programs |

## How TimeCode works

- **Only `t` is stored.** Hours, minutes, and seconds are always calculated from it.
- **All conversion lives in two methods.** `ComponentsToSeconds()` turns hours, minutes, and seconds into a total, and `GetComponents()` turns a total back into the three parts. Every other method uses those two rather than repeating the math.
- **Out-of-range values either roll over or throw.** The constructor and the arithmetic operators roll over, so `TimeCode(0, 61, 0)` becomes `1:1:0`. The setters throw `invalid_argument` instead, because `SetMinutes(80)` is almost certainly a mistake.
- **Nothing may go negative.** Subtracting a larger time, multiplying or dividing by a negative number, and dividing by zero all throw `invalid_argument`.
- **Hours have no upper limit**, so `187:53:27` is valid. Minutes and seconds are always 0-59.

## Citations

I used Claude (Anthropic's AI assistant) to review my code, explain C++ concepts, and help write tests and comments. The `split()` helper in `NasaLaunchAnalysis.cpp` was provided by Prof. Novak in an in-class Markov chain activity, and `PaintDryTimer.cpp` began from the starter file provided with the assignment.
