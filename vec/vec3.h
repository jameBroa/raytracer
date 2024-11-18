#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
    public:
        double coords[3];
        vec3();
        vec3(double coord_x, double coord_y, double coord_z);

        double x() const;
        double y() const;
        double z() const;

        vec3 operator-() const;
        double operator[](int i) const;
        // The & symbol indicates reference like in Java. Not a pointer!!!!
        double& operator[](int i);

        vec3& operator+=(const vec3& v);
        vec3& operator*=(double t);
        vec3& operator/=(double t);

        double length() const;
        double length_squared() const;   
};

using point = vec3;
using rgb = vec3;

#endif