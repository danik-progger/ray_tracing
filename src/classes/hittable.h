#ifndef HITTABLE_H
#define HITTABLE_H

#include "utils.h"

class Material;

class HitRecord {
 public:
  Point3 p;
  Vec3 normal;
  std::shared_ptr<Material> material;
  double t;
  bool front_face;

  void set_face_normal(const Ray& r, const Vec3& outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
 public:
  virtual ~Hittable() = default;

  virtual bool hit(const Ray& r, Interval Ray_t, HitRecord& rec) const = 0;
};

#endif