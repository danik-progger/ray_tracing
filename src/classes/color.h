#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

using Color = Vec3;

void write_color(std::ostream& out, const Color& pixel_color) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  static const Interval intensity(0.000, 0.999);
  int rByte = int(256 * intensity.clamp(r));
  int gByte = int(256 * intensity.clamp(g));
  int bByte = int(256 * intensity.clamp(b));

  out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif  // COLOR_H
