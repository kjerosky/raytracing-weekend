#ifndef UTIL_H
#define UTIL_H

#include <limits>
#include <cstdlib>

const float PI = 3.1415926535897932385f;
const float INF= std::numeric_limits<float>::infinity();

#include <iostream>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include "Ray.h"
#include "Interval.h"

inline float random_float() {
    // Returns a random real in [0, 1).
    return std::rand() / (RAND_MAX + 1.0f);
}

inline float random_float(float min, float max) {
    // Returns a random real in [min, max).
    return min + (max - min) * random_float();
}

#endif
