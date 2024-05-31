#ifndef UTILS_H
#define UTILS_H

#include <limits>

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.14159265358979323846;

inline double deg_to_rad(double deg) { return deg * PI / 180.0; }

inline double random_double() { return rand() / (RAND_MAX + 1.0); }

inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}

#endif  // UTILS_H