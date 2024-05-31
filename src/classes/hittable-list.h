#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"

class HittableList : public Hittable {
 public:
  HittableList() {}
  HittableList(std::shared_ptr<Hittable> object) { push(object); }

  void push(std::shared_ptr<Hittable> object) { objects.push_back(object); }
  void clear() { objects.clear(); }

  bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
    HitRecord temp_rec;
    bool hitAnything = false;
    auto closest = ray_t.max;

    for (const auto& object : objects) {
      if (object->hit(r, Interval(ray_t.min, closest), temp_rec)) {
        hitAnything = true;
        closest = temp_rec.t;
        rec = temp_rec;
      }
    }

    return hitAnything;
  }

 private:
  std::vector<std::shared_ptr<Hittable>> objects;
};

#endif  // HITTABLE_LIST_H