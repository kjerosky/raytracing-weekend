#include "Interval.h"

const Interval Interval::EMPTY = Interval(+INF, -INF);
const Interval Interval::UNIVERSE = Interval(-INF, +INF);

Interval::Interval()
:
min(+INF),
max(-INF) {
    // do nothing for now
}

// --------------------------------------------------------------------------

Interval::Interval(float min, float max)
:
min(min),
max(max) {
    // do nothing for now
}

// --------------------------------------------------------------------------

Interval::~Interval() {
    // do nothing for now
}

// --------------------------------------------------------------------------

float Interval::size() {
    return max - min;
}

// --------------------------------------------------------------------------

bool Interval::contains(float value) {
    return min <= value && value <= max;
}

// --------------------------------------------------------------------------

bool Interval::surrounds(float value) {
    return min < value && value < max;
}

// --------------------------------------------------------------------------

float Interval::get_min() {
    return min;
}

// --------------------------------------------------------------------------

float Interval::get_max() {
    return max;
}
