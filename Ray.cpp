#include "Ray.h"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
:
origin(origin),
direction(direction) {
    // nothing to do for now
}

// --------------------------------------------------------------------------

Ray::~Ray() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

const glm::vec3& Ray::get_origin() const {
    return origin;
}

// --------------------------------------------------------------------------

const glm::vec3& Ray::get_direction() const {
    return direction;
}

// --------------------------------------------------------------------------

glm::vec3 Ray::at(float t) const {
    return origin + t * direction;
}
