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
  auto material_ground = std::make_shared<LambertianMaterial>(Color(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<LambertianMaterial>(Color(0.1, 0.2, 0.5));
  auto material_left = std::make_shared<Metal>(Color(0.8, 0.8, 0.8));
  auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2));

  world.push(
      std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.push(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
  world.push(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.push(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

  Camera camera(16.0 / 9.0, 800, 50);
  camera.render(world);

  return 0;
}