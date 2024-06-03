cmake_minimum_required (VERSION 3.27.0)
set (CMAKE_CXX_STANDARD 20)
project (rat_tracing_in_one_weekend LANGUAGES CXX)

set (ONE_WEEKEND
  src/main.cpp
  src/classes/camera.h
  src/classes/color.h
  src/classes/hittable.h
  src/classes/hittable-list.h
  src/classes/interval.h
  src/classes/material.h
  src/classes/ray.h
  src/classes/utils.h
  src/classes/sphere.h
  src/classes/vec3.h
)

include_directories(src)
set (EXTERNAL src/external/stb_image.h)

add_executable(ray_tracing ${EXTERNAL} ${ONE_WEEKEND})