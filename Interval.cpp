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

float Interval::size() const {
    return max - min;
}

// --------------------------------------------------------------------------

bool Interval::contains(float value) const {
    return min <= value && value <= max;
}

// --------------------------------------------------------------------------

bool Interval::surrounds(float value) const {
    return min < value && value < max;
}

// --------------------------------------------------------------------------

float Interval::clamp(float value) const {
    return glm::clamp(value, min, max);
}

// --------------------------------------------------------------------------

float Interval::get_min() const {
    return min;
}

// --------------------------------------------------------------------------

float Interval::get_max() const {
    return max;
}
