#include "vec3.h"
#pragma once

namespace vec3_utils {
    double dot(const vec3& u, const vec3& v);
    vec3 cross(const vec3& u, const vec3& v);

    vec3 operator+(const vec3& u, const vec3& v);
    vec3 operator-(const vec3& u, const vec3& v);

    vec3 operator*(const vec3& u, const vec3& v); // vector x vector
    vec3 operator*(const vec3& u, double t); // vector x scalar
    vec3 operator*(double t, const vec3& u); // scalar x vector (reuse above)

    bool operator<(const vec3& u, const vec3& v); // comparison 
    bool operator>(const vec3& u, const vec3& v); // comparison 


    vec3 operator/(const vec3& u, double t); 

    vec3 unit_vector(const vec3& u);
     
    std::ostream& operator<<(std::ostream& out, const vec3& u);
}