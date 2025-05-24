#include "HittableList.h"

HittableList::HittableList() {
    // do nothing for now
}

// --------------------------------------------------------------------------

HittableList::HittableList(std::shared_ptr<Hittable> object) {
    add(object);
}

// --------------------------------------------------------------------------

void HittableList::clear() {
    objects.clear();
}

// --------------------------------------------------------------------------

void HittableList::add(std::shared_ptr<Hittable> object) {
    objects.push_back(object);
}

// --------------------------------------------------------------------------

bool HittableList::hit(const Ray& r, Interval ray_t, HitRecord& hit_record) const {
    HitRecord temp_hit_record;
    bool did_hit_anything = false;
    float closest_t_so_far = ray_t.get_max();

    for (std::shared_ptr<Hittable> object : objects) {
        if (object->hit(r, Interval(ray_t.get_min(), closest_t_so_far), temp_hit_record)) {
            did_hit_anything = true;
            closest_t_so_far = temp_hit_record.t;
            hit_record = temp_hit_record;
        }
    }

    return did_hit_anything;
}
