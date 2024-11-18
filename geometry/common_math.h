#ifndef COMMON_MATH_H
#define COMMON_MATH_H

using namespace std;
#include <cmath>

const double pi = 3.1415926535897932384626433;
const double inf = std::numeric_limits<double>::infinity();
inline double deg_to_rad(double deg){
    return deg * pi / 180.0;
}

#endif