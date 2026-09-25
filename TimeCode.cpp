// Author: Teague Wright
// CS-222 HW2: TimeCode
// Help: Claude (Anthropic's AI assistant) reviewed my code and explained C++ concepts.

#include <iostream>  // cout for debugging output
#include <string>    // string and to_string() for ToString() and exception messages
#include <stdexcept> // invalid_argument, thrown for negative or invalid values
// NOTE: <cmath> is intentionally not included; the assignment forbids the math library

#include "TimeCode.h"

using namespace std;

// Roll-over is free here: ComponentsToSeconds turns 0:61:0 into 3660 seconds,
// which GetComponents later splits back out as 1:1:0.
TimeCode::TimeCode(unsigned int hr, unsigned int min, long long unsigned int sec) { t = ComponentsToSeconds(hr, min, sec);  }
// t is the only data, so copying it copies the whole object. Reading tc.t is allowed
// because private means private to the class, not to the individual object.
TimeCode::TimeCode(const TimeCode& tc) { t = tc.t; }

// Setters change one part only: split t apart, swap one piece, rebuild.
// Hours have no upper limit and unsigned rules out negatives, so SetHours rejects nothing.
void TimeCode::SetHours(unsigned int hours) {
    unsigned int h;
	unsigned int m;
	unsigned int s;

	GetComponents(h, m, s);

    h = hours;

    t = ComponentsToSeconds(h, m, s);
}
// Setters throw rather than roll over: SetMinutes(80) is almost certainly a mistake,
// and silently changing the hours would surprise the caller.
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

void TimeCode::reset() { t = 0; }

// Getters reuse GetComponents so the conversion math lives in exactly one place.
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

// Three results, and a function can only return one, so they are written back through
// references (&) into the caller's own variables.
void TimeCode::GetComponents(unsigned int& hr, unsigned int& min, unsigned int& sec) const {
    hr = t / 3600;
    min = (t % 3600) / 60;
    sec = ((t % 3600) % 60);
}

// Static: it converts plain numbers and needs no existing object. It accepts any values
// (e.g. 71 minutes), which is what makes the constructor's roll-over work.
long long unsigned int TimeCode::ComponentsToSeconds(unsigned int hr, unsigned int min, unsigned long long int sec) {
    long long unsigned int result = 0;
    result += (static_cast<long long unsigned int>(hr) * 3600) + (min * 60) + sec; // cast hr first so hr * 3600 can't overflow unsigned int
    return result;
}

// No zero-padding (e.g. "8:5:9"), matching the provided tests.
string TimeCode::ToString() const {
    string str = "";
    return str += to_string(GetHours()) + ":" + to_string(GetMinutes()) + ":" + to_string(GetSeconds());
}

// Building the result with (0, 0, seconds) lets the constructor handle all roll-over.
TimeCode TimeCode::operator+(const TimeCode& other) const {
    TimeCode tc = TimeCode(0, 0, t + other.t);
    return tc;
}
TimeCode TimeCode::operator-(const TimeCode& other) const {
    if (t < other.t) { throw invalid_argument("Subtraction would result in a negative TimeCode: " + ToString() + " - " + other.ToString()); } // t is unsigned, so this would wrap to a huge number instead of going negative
    TimeCode tc = TimeCode(0, 0, t - other.t);
    return tc;
}
// Scaling by a plain number, since TimeCode * TimeCode has no meaning. Negatives throw
// rather than taking an absolute value, because 1:0:0 * -2 = 2:0:0 would be silently wrong.
// Fractions of a second are dropped on the way back to whole seconds.
TimeCode TimeCode::operator*(double a) const {
    if (a < 0) { throw invalid_argument("Factor cannot be a negative number: " + to_string(a)); }
    TimeCode tc = TimeCode(0, 0, t * a);
    return tc;
}
TimeCode TimeCode::operator/(double a) const {
    if (a < 0) { throw invalid_argument("Divisor cannot be a negative number: " + to_string(a)); }
    if (a == 0) { throw invalid_argument("Divisor cannot be zero"); } // t / 0 is infinity, which can't be converted to a whole number of seconds
    TimeCode tc = TimeCode(0, 0, t / a);
    return tc;
}

// Comparing totals compares whole times, so 1:0:0 == 0:60:0 and 1:59:59 < 2:0:0.
bool TimeCode::operator == (const TimeCode& other) const {return t == other.t;}
bool TimeCode::operator != (const TimeCode& other) const {return t != other.t;}

bool TimeCode::operator < (const TimeCode& other) const {return t < other.t;}
bool TimeCode::operator <= (const TimeCode& other) const {return t <= other.t;}

bool TimeCode::operator > (const TimeCode& other) const {return t > other.t;}
bool TimeCode::operator >= (const TimeCode& other) const {return t >= other.t;}
