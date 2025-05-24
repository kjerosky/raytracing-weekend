#ifndef SPHERE_H
#define SPHERE_H

#include "rt_weekend.h"
#include "Hittable.h"

class Sphere : public Hittable {

public:

    Sphere(const glm::vec3& center, float radius);
    ~Sphere();

    bool hit(const Ray& r, Interval ray_t, HitRecord& hit_record) const override;

private:

    glm::vec3 center;
    float radius;
};

#endif
