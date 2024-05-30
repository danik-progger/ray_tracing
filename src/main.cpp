#include <iostream>

#include "classes/color.h"
#include "classes/ray.h"
#include "classes/vec3.h"

bool hit_sphere(const Point3& center, double radius, const Ray& r) {
  Vec3 oc = center - r.origin();
  double a = dot(r.direction(), r.direction());
  double b = -2.0 * dot(r.direction(), oc);
  double c = dot(oc, oc) - radius * radius;
  double discriminant = b * b - 4 * a * c;
  return (discriminant >= 0);
}

Color ray_color(const Ray& r) {
  if (hit_sphere(Point3(0, 0, -1), 0.5, r)) return Color(1, 0, 0);
  Vec3 unitDirection = unit_vector(r.direction());
  double a = 0.5 * (unitDirection.y() + 1.0);
  return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main() {
  double aspectRatio = 16.0 / 9.0;
  int imageWidth = 800;

  int imageHeight = int(imageWidth / aspectRatio);
  imageHeight = (imageHeight < 1) ? 1 : imageHeight;

  double focalLength = 1.0;
  double viewportHeight = 2.0;
  double viewportWidth =
      viewportHeight * (double(imageWidth) / imageHeight);
  Point3 cameraCenter = Point3(0, 0, 0);

  Vec3 viewportU = Vec3(viewportWidth, 0, 0);
  Vec3 viewportV = Vec3(0, -viewportHeight, 0);
  Vec3 pixelDeltaU = viewportU / imageWidth;
  Vec3 pixelDeltaV = viewportV / imageHeight;

  Vec3 viewportUpperLeft = cameraCenter - Vec3(0, 0, focalLength) -
                             viewportU / 2 - viewportV / 2;
  Vec3 pixel00Loc =
      viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

  std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

  for (int j = 0; j < imageHeight; j++) {
    std::clog << "\rScan lines remaining: " << (imageHeight - j) << ' '
              << std::flush;
    for (int i = 0; i < imageWidth; i++) {
      auto pixelCenter =
          pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
      auto rayDirection = pixelCenter - cameraCenter;
      Ray r(cameraCenter, rayDirection);

      Color pixelColor = ray_color(r);
      write_color(std::cout, pixelColor);
    }
  }

  return 0;
}