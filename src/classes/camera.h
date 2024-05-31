#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"

class Camera {
 public:
  Camera(double aspectRatio, int32_t imageWidth)
      : aspectRatio(aspectRatio), imageWidth(imageWidth) {
    imageHeight = int32_t(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    double focalLength = 1.0;
    double viewportHeight = 2.0;
    double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
    cameraPosition = Point3(0, 0, 0);

    Vec3 viewportU = Vec3(viewportWidth, 0, 0);
    Vec3 viewportV = Vec3(0, -viewportHeight, 0);
    pixelDeltaU = viewportU / imageWidth;
    pixelDeltaV = viewportV / imageHeight;

    Vec3 viewportUpperLeft = cameraPosition - Vec3(0, 0, focalLength) -
                             viewportU / 2 - viewportV / 2;
    pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    pixelSamplesScale = 1.0 / samplesPerPixel;
  }

  void render(const Hittable& world) {
    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++) {
      std::clog << "\rScan lines remaining: " << (imageHeight - j) << ' '
                << std::flush;
      for (int i = 0; i < imageWidth; i++) { 
        auto pixelCenter = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
        auto rayDirection = pixelCenter - cameraPosition;
        Ray r(cameraPosition, rayDirection);

        Color pixelColor(0, 0, 0);
        for (uint32_t sample = 0; sample < samplesPerPixel; ++sample) {
          Ray r = getRay(i, j);
          pixelColor += ray_color(r, world);
        }

        write_color(std::cout, pixelColor * pixelSamplesScale);
      }
    }

    std::clog << "\nDone\n";
  }

 private:
  Color ray_color(const Ray& r, const Hittable& world) {
    HitRecord rec;
    if (world.hit(r, Interval(0, INF), rec)) {
      return 0.5 * (rec.normal + Color(1, 1, 1));
    }

    Vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
  }
  Ray getRay(int i, int j) const {
    Vec3 offset = sample_square();
    Vec3 pixel_sample = pixel00Loc + ((i + offset.x()) * pixelDeltaU) +
                        ((j + offset.y()) * pixelDeltaV);

    Point3 ray_origin = cameraPosition;
    Vec3 ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
  }

  Vec3 sample_square() const {
    return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
  }

  int32_t imageHeight;
  Point3 cameraPosition;
  Point3 pixel00Loc;
  Vec3 pixelDeltaU;
  Vec3 pixelDeltaV;
  double aspectRatio;
  int32_t imageWidth;
  int32_t samplesPerPixel = 10;
  double pixelSamplesScale;
};

#endif  // CAMERA_H