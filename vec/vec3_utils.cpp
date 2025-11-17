#include "vector3.h"
#include "vec3_utils.h"
#include <iostream>

namespace vec3_utils{
    double dot(const vector3& u, const vector3& v) {
        return 
        u.coords[0]*v.coords[0] +
        u.coords[1]*v.coords[1] +
        u.coords[2]*v.coords[2];
    }

    vector3 cross(const vector3& u, const vector3& v) {
        return vector3(
            u.coords[1] * v.coords[2] - u.coords[2] * v.coords[1],
            u.coords[2] * v.coords[0] - u.coords[0] * v.coords[2],
            u.coords[0] * v.coords[1] - u.coords[1] * v.coords[0]
        );
    }

    vector3 operator+(const vector3& u, const vector3& v){
        return vector3(
            u.coords[0] + v.coords[0],
            u.coords[1] + v.coords[1],
            u.coords[2] + v.coords[2]
        );
    }

    vector3 operator-(const vector3& u, const vector3&v){
        return vector3(
            u.coords[0] - v.coords[0],
            u.coords[1] - v.coords[1],
            u.coords[2] - v.coords[2]
        );
    }

    vector3 operator*(const vector3& u, const vector3& v){
        return vector3(
            u.coords[0] * v.coords[0],
            u.coords[1] * v.coords[1],
            u.coords[2] * v.coords[2]
        );
    }
    vector3 operator*(double t, const vector3& u){
        return vector3(
            t*u.coords[0],
            t*u.coords[1],
            t*u.coords[2]
        );

    }

    bool operator<(const vector3& u, const vector3& v) {
        return u.coords[0] < v.coords[0] && u.coords[1] < v.coords[1] && u.coords[2] < v.coords[2];
    }
    bool operator>(const vector3& u, const vector3& v) {
        return u.coords[0] > v.coords[0] && u.coords[1] > v.coords[1] && u.coords[2] > v.coords[2];
    }


    std::ostream& operator<<(std::ostream& out, const vector3& u) {
        out << "vec3(" << u.x() << ", " << u.y() << ", " << u.z() << ")";
        return out;
    }

    vector3 operator*(const vector3& u, double t){
        return t*u;
    }

    vector3 operator/(const vector3& u, double t){
        return (1/t) * u;
    }

    vector3 unit_vector(const vector3& u) {
        return u / u.length();
    }

}