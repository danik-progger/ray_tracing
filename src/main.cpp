#include <iostream>
#include <memory>

#include "classes/camera.h"
#include "classes/hittable-list.h"
#include "classes/hittable.h"
#include "classes/material.h"
#include "classes/sphere.h"
#include "classes/utils.h"

int main() {
  HittableList world;

  auto ground_material = std::make_shared<LambertianMaterial>(Color(0.5, 0.5, 0.5));
  world. push(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
        std::shared_ptr<Material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = Color::random() * Color::random();
          sphere_material = std::make_shared<LambertianMaterial>(albedo);
          world. push(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          //  Metal
          auto albedo = Color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = std::make_shared< Metal>(albedo, fuzz);
          world. push(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = std::make_shared< Dielectric>(1.5);
          world. push(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = std::make_shared< Dielectric>(1.5);
  world. push(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<LambertianMaterial>(Color(0.4, 0.2, 0.1));
  world. push(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared< Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world. push(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  double aspectRatio = 16.0 / 9.0;
  int32_t imageWidth = 1200;
  int32_t maxRays = 50;
  double fov = 20;

  Point3 lookFrom(13, 2, 3);
  Point3 lookAt(0, 0, 0);
  Vec3 vup(0, 1, 0);

  double defocusAngle = 0.6;
  double focusDist = 10.0;

  Camera cam(aspectRatio, imageWidth, maxRays, fov, lookFrom, lookAt, vup, defocusAngle, focusDist);
  cam.render(world);
}