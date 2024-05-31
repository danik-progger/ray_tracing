#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
 public:
  Sphere(const Point3& center, double radius)
      : center(center), radius(fmax(0, radius)) {}

  bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
    Vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0) return false;

    auto sqrtD = sqrt(discriminant);

    auto root = (h - sqrtD) / a;
    if (!ray_t.surrounds(root)) {
      root = (h + sqrtD) / a;
      if (!ray_t.surrounds(root)) return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    return true;
  }

 private:
  Point3 center;
  double radius;
};

#endif