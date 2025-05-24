#ifndef INTERVAL_H
#define INTERVAL_H

#include "rt_weekend.h"

class Interval {

public:

    Interval();
    Interval(float min, float max);
    ~Interval();

    float size();
    bool contains(float value);
    bool surrounds(float value);

    float get_min();
    float get_max();

    static const Interval EMPTY;
    static const Interval UNIVERSE;

private:

    float min;
    float max;
};

#endif
