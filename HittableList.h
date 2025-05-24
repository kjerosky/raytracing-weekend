#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "rt_weekend.h"

#include <memory>
#include <vector>

#include "Hittable.h"

class HittableList : public Hittable {

public:

    HittableList();
    HittableList(std::shared_ptr<Hittable> object);

    void clear();
    void add(std::shared_ptr<Hittable> object);
    bool hit(const Ray& r, Interval ray_t, HitRecord& hit_record) const override;

private:

    std::vector<std::shared_ptr<Hittable>> objects;
};

#endif
