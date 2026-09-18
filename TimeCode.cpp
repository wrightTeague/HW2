// Author: Teague Wright
// CS-222 HW2: TimeCode
// Help: I used Claude (Anthropic's AI assistant) to review my code, explain C++ concepts
// (constructors, pass by reference, integer division), and help write/comment tests.

#include <iostream>  // cout for debugging output
#include <string>    // string and to_string() for ToString() and exception messages
#include <stdexcept> // invalid_argument, thrown for negative or invalid values
// NOTE: <cmath> is intentionally not included; the assignment forbids the math library

#include "TimeCode.h"

using namespace std;

// Only t is stored, so the constructor converts the components into total seconds.
// ComponentsToSeconds handles roll-over for free (e.g. 0:61:0 becomes 1:1:0 once split back out).
TimeCode::TimeCode(unsigned int hr, unsigned int min, long long unsigned int sec) { t = ComponentsToSeconds(hr, min, sec);  }
// t is the only data, so copying it makes a complete, independent copy.
// Accessing tc.t is allowed because private means private to the class, not the object.
TimeCode::TimeCode(const TimeCode& tc) { t = tc.t; }

// The setters change one part only (no roll-over): split t into parts, swap one, rebuild t.
// Hours have no upper limit, and unsigned rules out negatives, so SetHours has nothing to reject.
void TimeCode::SetHours(unsigned int hours) {
    unsigned int h;
	unsigned int m;
	unsigned int s;

	GetComponents(h, m, s);

    h = hours;

    t = ComponentsToSeconds(h, m, s);
}
// Unlike the constructor, the setters throw instead of rolling over: SetMinutes(80) is
// almost certainly a mistake, and silently changing the hours would surprise the caller.
void TimeCode::SetMinutes(unsigned int minutes) {
    if (minutes > 59) { throw invalid_argument("Minutes must be between 0 and 59: " + to_string(minutes)); } // throw before touching t so an invalid call leaves the time unchanged
    unsigned int h;
	unsigned int m;
	unsigned int s;

	GetComponents(h, m, s);

    m = minutes;
    t = ComponentsToSeconds(h, m, s);
}
void TimeCode::SetSeconds(unsigned int seconds) {
    if (seconds > 59) { throw invalid_argument("Seconds must be between 0 and 59: " + to_string(seconds)); } // throw before touching t so an invalid call leaves the time unchanged
    unsigned int h;
	unsigned int m;
	unsigned int s;

	GetComponents(h, m, s);

    s = seconds;
    t = ComponentsToSeconds(h, m, s);
}

// Everything is derived from t, so zeroing it resets hours, minutes and seconds at once.
void TimeCode::reset() { t = 0; }

// The getters reuse GetComponents instead of repeating the conversion math,
// so the math lives in one place (as the spec requires).
unsigned int TimeCode::GetHours() const {
    unsigned int h;
	unsigned int m;
	unsigned int s;

	GetComponents(h, m, s);
    return h;
}
unsigned int TimeCode::GetMinutes() const {
    unsigned int h;
	unsigned int m;
	unsigned int s;

	GetComponents(h, m, s);
    return m;
}
unsigned int TimeCode::GetSeconds() const {
    unsigned int h;
	unsigned int m;
	unsigned int s;

	GetComponents(h, m, s);
    return s;
}

// Returns three values, so it writes into the caller's variables through references (&)
// instead of returning. Integer division drops the remainder, and % keeps only the remainder.
void TimeCode::GetComponents(unsigned int& hr, unsigned int& min, unsigned int& sec) const {
    hr = t / 3600;            // whole hours (3600 seconds per hour)
    min = (t % 3600) / 60;    // seconds left over after the hours, as whole minutes
    sec = ((t % 3600) % 60);  // seconds left over after the minutes
}

// Static because it doesn't need an existing object: it just converts numbers to seconds.
// It accepts any values (e.g. 71 minutes), which is what makes the constructor's roll-over work.
long long unsigned int TimeCode::ComponentsToSeconds(unsigned int hr, unsigned int min, unsigned long long int sec) {
    long long unsigned int result = 0;
    result += (static_cast<long long unsigned int>(hr) * 3600) + (min * 60) + sec; // cast hr first so hr * 3600 can't overflow unsigned int
    return result;
}

// Format is h:m:s with no zero-padding (e.g. "8:5:9"), matching the provided tests.
string TimeCode::ToString() const {
    string str = "";
    return str += to_string(GetHours()) + ":" + to_string(GetMinutes()) + ":" + to_string(GetSeconds());
}

// Arithmetic returns a new TimeCode and leaves both operands unchanged (the methods are const).
// Building the result with (0, 0, seconds) lets the constructor handle all roll-over.
TimeCode TimeCode::operator+(const TimeCode& other) const {
    TimeCode tc = TimeCode(0, 0, t + other.t);
    return tc;
}
TimeCode TimeCode::operator-(const TimeCode& other) const {
    if (t < other.t) { throw invalid_argument("Subtraction would result in a negative TimeCode: " + ToString() + " - " + other.ToString()); } // TimeCodes can't be negative, and t is unsigned so t - other.t would wrap to a huge number
    TimeCode tc = TimeCode(0, 0, t - other.t);
    return tc;
}
// Multiply/divide by a plain number, since TimeCode * TimeCode has no meaning.
// The result is converted back to whole seconds, so any fraction of a second is dropped.
// Negative numbers throw instead of using an absolute value, because 1:0:0 * -2 = 2:0:0 would be wrong.
TimeCode TimeCode::operator*(double a) const {
    if (a < 0) { throw invalid_argument("Factor cannot be a negative number: " + to_string(a)); } // TimeCodes can't be negative
    TimeCode tc = TimeCode(0, 0, t * a);
    return tc;
}
TimeCode TimeCode::operator/(double a) const {
    if (a < 0) { throw invalid_argument("Divisor cannot be a negative number: " + to_string(a)); } // TimeCodes can't be negative
    if (a == 0) { throw invalid_argument("Divisor cannot be zero"); } // t / 0 is infinity, which can't be converted to a whole number of seconds
    TimeCode tc = TimeCode(0, 0, t / a);
    return tc;
}

// Comparing total seconds compares whole times, so 1:0:0 == 0:60:0 and 1:59:59 < 2:0:0.
bool TimeCode::operator == (const TimeCode& other) const {return t == other.t;}
bool TimeCode::operator != (const TimeCode& other) const {return t != other.t;}

bool TimeCode::operator < (const TimeCode& other) const {return t < other.t;}
bool TimeCode::operator <= (const TimeCode& other) const {return t <= other.t;}

bool TimeCode::operator > (const TimeCode& other) const {return t > other.t;}
bool TimeCode::operator >= (const TimeCode& other) const {return t >= other.t;}
