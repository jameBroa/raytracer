#ifndef COMMON_H
#define COMMON_H

// Common libraries
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// Common headers
#include "./vec/vec3.h"
#include "./ray/ray.h"
#include "./colour/colour.h"
#include "./camera/camera.h"
#include "./image/image.h"
#include "./vec/vec3_utils.h"

using std::make_shared;
using std::shared_ptr;

// Some useful maths
const double pi = 3.1415926535897932384626433;
const double inf = std::numeric_limits<double>::infinity();
inline double deg_to_rad(double deg){
    return deg * pi / 180.0;
}
#endif