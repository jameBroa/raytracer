#include "vec3.h"
#include "vec3_utils.h"
#include <iostream>

namespace vec3_utils{
    double dot(const vec3& u, const vec3& v) {
        return 
        u.coords[0]*v.coords[0] +
        u.coords[1]*v.coords[1] +
        u.coords[2]*v.coords[2];
    }

    vec3 cross(const vec3& u, const vec3& v) {
        return vec3(
            u.coords[1] * v.coords[2] - u.coords[2] * v.coords[1],
            u.coords[2] * v.coords[0] - u.coords[0] * v.coords[2],
            u.coords[0] * v.coords[1] - u.coords[1] * v.coords[0]
        );
    }

    vec3 operator+(const vec3& u, const vec3& v){
        return vec3(
            u.coords[0] + v.coords[0],
            u.coords[1] + v.coords[1],
            u.coords[2] + v.coords[2]
        );
    }

    vec3 operator-(const vec3& u, const vec3&v){
        return vec3(
            u.coords[0] - v.coords[0],
            u.coords[1] - v.coords[1],
            u.coords[2] - v.coords[2]
        );
    }

    vec3 operator*(const vec3& u, const vec3& v){
        return vec3(
            u.coords[0] * v.coords[0],
            u.coords[1] * v.coords[1],
            u.coords[2] * v.coords[2]
        );
    }
    vec3 operator*(double t, const vec3& u){
        return vec3(
            t*u.coords[0],
            t*u.coords[1],
            t*u.coords[2]
        );

    }

    bool operator<(const vec3& u, const vec3& v) {
        return u.coords[0] < v.coords[0] && u.coords[1] < v.coords[1] && u.coords[2] < v.coords[2];
    }
    bool operator>(const vec3& u, const vec3& v) {
        return u.coords[0] > v.coords[0] && u.coords[1] > v.coords[1] && u.coords[2] > v.coords[2];
    }


    std::ostream& operator<<(std::ostream& out, const vec3& u) {
        out << "vec3(" << u.x() << ", " << u.y() << ", " << u.z() << ")";
        return out;
    }

    vec3 operator*(const vec3& u, double t){
        return t*u;
    }

    vec3 operator/(const vec3& u, double t){
        return (1/t) * u;
    }

    vec3 unit_vector(const vec3& u) {
        return u / u.length();
    }

}