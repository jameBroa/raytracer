#include "vec3.h"


// In C++, the :: operator is used as a scope resolution
// Therefore, with 'vec3::vec3()' we're saying, 'looking at vec3 and specifically the function vec3' 
// which in this case happens to be a constructor, implement below.

// Could also be thought of like: vec3:: -> within the scope of the vec3 class
vec3::vec3() {
    coords[0] = 0.0;
    coords[1] = 0.0;
    coords[2] = 0.0;
}

vec3::vec3(double coord_x, double coord_y, double coord_z) {
    coords[0] = coord_x;
    coords[1] = coord_y;
    coords[2] = coord_z;
}

double vec3::x() const {
    return coords[0];
}

double vec3::y() const {
    return coords[1];
}

double vec3::z() const {
    return coords[2];
}

vec3 vec3::operator-() const {
    return vec3(
        -coords[0],
        -coords[1],
        -coords[2]
    );
}

double vec3::operator[](int i) const {
    return coords[i];
}

double& vec3::operator[](int i) {
    return coords[i];
}

vec3& vec3::operator+=(const vec3& v){
    coords[0] += v.coords[0];
    coords[1] += v.coords[1];
    coords[2] += v.coords[2];
    return *this; //*this keyword is a reference to the curent object instance/reference
}

vec3& vec3::operator*=(double t) {
    coords[0] *= t;
    coords[1] *= t;
    coords[2] *= t;
    return *this;
}

vec3& vec3::operator/=(double t) {
    coords[0] /= t;
    coords[1] /= t;
    coords[2] /= t;
    return *this;
}

/*
Could implement operator/=(double t) similarly to below:

    vec3 vec3::operator/=(double t){
        return *this *= 1/t;
    }
My implementation still works, but the above is more C++ like apparantly
*/

double vec3::length() const {
    return std::sqrt(length_squared());
}

double vec3::length_squared() const {
    return coords[0]*coords[0] + coords[1]*coords[1] + coords[2]*coords[2];
}





