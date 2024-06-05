#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include "utils.h"

class Camera {
 public:
  Camera(double aspectRatio, int32_t imageWidth, int32_t maxRays, double fov,
         Point3 lookFrom, Point3 lookAt, Vec3 vup, double defocusAngle,
         double focusDist)
      : aspectRatio(aspectRatio),
        imageWidth(imageWidth),
        maxRays(maxRays),
        fov(fov),
        lookFrom(lookFrom),
        lookAt(lookAt),
        vup(vup),
        defocusAngle(defocusAngle),
        focusDist(focusDist) {
    imageHeight = int32_t(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    // double focalLength = (lookFrom - lookAt).length();
    auto theta = deg_to_rad(fov);
    auto h = tan(theta / 2);
    auto viewportHeight = 2 * h * focusDist;
    double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
    cameraPosition = lookFrom;

    w = unit_vector(lookFrom - lookAt);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    Vec3 viewportU = viewportWidth * u;
    Vec3 viewportV = viewportHeight * -v;
    pixelDeltaU = viewportU / imageWidth;
    pixelDeltaV = viewportV / imageHeight;

    Vec3 viewportUpperLeft =
        cameraPosition - (focusDist * w) - viewportU / 2 - viewportV / 2;

    pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    pixelSamplesScale = 1.0 / samplesPerPixel;
    auto defocus_radius = focusDist * tan(deg_to_rad(defocusAngle / 2));
    defocusDiskU = u * defocus_radius;
    defocusDiskV = v * defocus_radius;
  }

  void render(const Hittable& world) {
    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++) {
      std::clog << "\rScan lines remaining: " << (imageHeight - j) << ' '
                << std::flush;
      for (int i = 0; i < imageWidth; i++) {
        Color pixelColor(0, 0, 0);
        for (uint32_t sample = 0; sample < samplesPerPixel; ++sample) {
          Ray r = getRay(i, j);
          pixelColor += ray_color(r, maxRays, world);
        }

        write_color(std::cout, pixelColor * pixelSamplesScale);
      }
    }

    std::clog << "\nDone\n";
  }

 private:
  Color ray_color(const Ray& r, int32_t maxRays, const Hittable& world) {
    if (maxRays <= 0) return Color(0, 0, 0);
    HitRecord rec;
    if (world.hit(r, Interval(0.001, INF), rec)) {
      Ray scattered;
      Color attenuation;
      if (rec.material->scatter(r, rec, attenuation, scattered))
        return attenuation * ray_color(scattered, maxRays - 1, world);
      return Color(0, 0, 0);
    }

    Vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
  }
  Ray getRay(int i, int j) const {
    Vec3 offset = sample_square();
    Vec3 pixel_sample = pixel00Loc + ((i + offset.x()) * pixelDeltaU) +
                        ((j + offset.y()) * pixelDeltaV);

    auto ray_origin =
        (defocusAngle <= 0) ? cameraPosition : defocus_disk_sample();
    Vec3 ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
  }

  Vec3 sample_square() const {
    return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
  }

  Point3 defocus_disk_sample() const {
    auto p = random_in_unit_disk();
    return cameraPosition + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
  }

  int32_t imageHeight;
  Point3 cameraPosition;
  Point3 pixel00Loc;
  Vec3 pixelDeltaU;
  Vec3 pixelDeltaV;
  double aspectRatio;
  int32_t imageWidth;
  int32_t samplesPerPixel = 500;
  double pixelSamplesScale;
  int32_t maxRays;
  double fov;
  Point3 lookFrom = Point3(0, 0, 0);
  Point3 lookAt = Point3(0, 0, -1);
  Vec3 vup = Vec3(0, 1, 0);
  Vec3 u, v, w;

  Vec3 defocusDiskU;
  Vec3 defocusDiskV;
  double defocusAngle;
  double focusDist;
};

#endif  // CAMERA_H