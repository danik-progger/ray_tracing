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

        Color pixelColor = ray_color(r, world);
        write_color(std::cout, pixelColor);
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

  int32_t imageHeight;
  Point3 cameraPosition;
  Point3 pixel00Loc;
  Vec3 pixelDeltaU;
  Vec3 pixelDeltaV;
  double aspectRatio;
  int32_t imageWidth;
};

#endif // CAMERA_H