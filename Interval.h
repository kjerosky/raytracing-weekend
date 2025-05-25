#ifndef INTERVAL_H
#define INTERVAL_H

#include "rt_weekend.h"

class Interval {

public:

    Interval();
    Interval(float min, float max);
    ~Interval();

    float size() const;
    bool contains(float value) const;
    bool surrounds(float value) const;
    float clamp(float value) const;

    float get_min() const;
    float get_max() const;

    static const Interval EMPTY;
    static const Interval UNIVERSE;

private:

    float min;
    float max;
};

#endif
