#ifndef HITTABLE_H
#define HITTABLE_H

#include "rt_weekend.h"

struct HitRecord {
    glm::vec3 point;
    glm::vec3 normal;
    float t;
    bool is_front_face;

    void set_facing_and_normal(const Ray& r, const glm::vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: The parameter `outward_normal` is assumed to have unit length.

        is_front_face = glm::dot(r.get_direction(), outward_normal) < 0.0f;
        normal = is_front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {

public:

    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& hit_record) const = 0;
};

#endif
