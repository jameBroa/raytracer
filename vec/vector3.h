#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

class vector3 {
    public:
        double coords[3];
        vector3();
        vector3(double coord_x, double coord_y, double coord_z);

        double x() const;
        double y() const;
        double z() const;

        vector3 operator-() const;
        double operator[](int i) const;
        // The & symbol indicates reference like in Java. Not a pointer!!!!
        double& operator[](int i);

        vector3& operator+=(const vector3& v);
        vector3& operator*=(double t);
        vector3& operator/=(double t);

        double length() const;
        double length_squared() const;   
};

using point = vector3;
using rgb = vector3;

#endif