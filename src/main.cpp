#include <iostream>
#include <memory>

#include "classes/camera.h"
#include "classes/hittable-list.h"
#include "classes/sphere.h"

int main() {
  HittableList world;
  world.push(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  world.push(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  Camera camera(16.0 / 9.0, 800, 50);
  camera.render(world);

  return 0;
}