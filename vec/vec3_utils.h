#include "vector3.h"
#pragma once

namespace vec3_utils {
    double dot(const vector3& u, const vector3& v);
    vector3 cross(const vector3& u, const vector3& v);

    vector3 operator+(const vector3& u, const vector3& v);
    vector3 operator-(const vector3& u, const vector3& v);

    vector3 operator*(const vector3& u, const vector3& v); // vector x vector
    vector3 operator*(const vector3& u, double t); // vector x scalar
    vector3 operator*(double t, const vector3& u); // scalar x vector (reuse above)

    bool operator<(const vector3& u, const vector3& v); // comparison 
    bool operator>(const vector3& u, const vector3& v); // comparison 

    vector3 operator/(const vector3& u, double t); 

    vector3 unit_vector(const vector3& u);
    
    // Allows printing of vector3 objects
    std::ostream& operator<<(std::ostream& out, const vector3& u);
}