#include "vector3.h"


// In C++, the :: operator is used as a scope resolution
// Therefore, with 'vec3::vec3()' we're saying, 'looking at vec3 and specifically the function vec3' 
// which in this case happens to be a constructor, implement below.

// Could also be thought of like: vec3:: -> within the scope of the vec3 class
vector3::vector3() {
    coords[0] = 0.0;
    coords[1] = 0.0;
    coords[2] = 0.0;
}

vector3::vector3(double coord_x, double coord_y, double coord_z) {
    coords[0] = coord_x;
    coords[1] = coord_y;
    coords[2] = coord_z;
}

double vector3::x() const {
    return coords[0];
}

double vector3::y() const {
    return coords[1];
}

double vector3::z() const {
    return coords[2];
}

vector3 vector3::operator-() const {
    return vector3(
        -coords[0],
        -coords[1],
        -coords[2]
    );
}

vector3& vector3::operator+=(const vector3& v){
    coords[0] += v.coords[0];
    coords[1] += v.coords[1];
    coords[2] += v.coords[2];
    return *this; //*this keyword is a reference to the curent object instance/reference
}

vector3& vector3::operator*=(double t) {
    coords[0] *= t;
    coords[1] *= t;
    coords[2] *= t;
    return *this;
}

vector3& vector3::operator/=(double t) {
    coords[0] /= t;
    coords[1] /= t;
    coords[2] /= t;
    return *this;
}

double vector3::length() const {
    return std::sqrt(length_squared());
}

double vector3::length_squared() const {
    return coords[0]*coords[0] + coords[1]*coords[1] + coords[2]*coords[2];
}

// Added for AABB support
// Allows random sampling of axis for BVH
double vector3::operator[](int i) const {
    return coords[i];
}

double& vector3::operator[](int i) {
    return coords[i];
}





