#include "Sphere.h"

Sphere::Sphere(const glm::vec3& center, float radius)
:
center(center),
radius(glm::max(0.0f, radius)) {
    // do nothing for now
}

// --------------------------------------------------------------------------

Sphere::~Sphere() {
    // do nothing for now
}

// --------------------------------------------------------------------------

bool Sphere::hit(const Ray& r, Interval ray_t, HitRecord& hit_record) const {
    glm::vec3 oc = center - r.get_origin();
    float a = glm::dot(r.get_direction(), r.get_direction());
    float h = glm::dot(r.get_direction(), oc);
    float c = glm::dot(oc, oc) - radius * radius;

    float discriminant = h * h - a * c;
    if (discriminant < 0.0f) {
        return false;
    }

    float sqrt_discriminant = glm::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (h - sqrt_discriminant) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrt_discriminant) / a;
        if (!ray_t.surrounds(root)) {
            return false;
        }
    }

    hit_record.t = root;
    hit_record.point = r.at(hit_record.t);
    glm::vec3 outward_normal = (hit_record.point - center) / radius;
    hit_record.set_facing_and_normal(r, outward_normal);

    return true;
}
