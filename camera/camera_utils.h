#ifndef CAMERA_UTILS_H
#define CAMERA_UTILS_H
#include "camera.h"
#include "../ray/ray.h"
#include "../ray/ray_intersectable.h"
#include "../ray/ray_intersect_history.h"
#include "../vec/vector3.h"

namespace camera_utils {
    bool is_in_sight(point pt, ray r, const ray_intersectable& env);
    vector3 random_in_unit_disk();
}

#endif