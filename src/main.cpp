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
  auto ground = std::make_shared<LambertianMaterial>(Color(0.8, 0.8, 0.0));
  auto center = std::make_shared<LambertianMaterial>(Color(0.1, 0.2, 0.5));
  auto left = std::make_shared<Dielectric>(1.5);
  auto bubble = std::make_shared<Dielectric>(1.0 / 1.5);
  auto right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

  world.push(
      std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, ground));
  world.push(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, center));
  world.push(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, left));
  world.push(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, bubble));
  world.push(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, right));

  Camera camera(16.0 / 9.0, 800, 50);
  camera.render(world);

  return 0;
}