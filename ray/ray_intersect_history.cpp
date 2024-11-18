#include "ray_intersect_history.h"
#include "../vec/vec3_utils.h"

using namespace vec3_utils;

void intersect_history::set_face_of_norm(const ray& r, const vec3& outward_norm) {
    face_of_norm = dot(r.direction(), outward_norm) < 0;
    normal = face_of_norm ? outward_norm : -outward_norm;
}

// void intersect_history::set_material(material curr_mat) {
//     std::cout << "in intersect_history::set_material()" << std::endl;
//     std::cout << curr_mat.get_diffuse_colour() << std::endl;
//     mat = &curr_mat;
//     std::cout << mat->get_diffuse_colour() << std::endl;
// }

void intersect_history::set_material(const material& curr_mat) {
    // std::cout << "in intersect_history::set_material()" << std::endl;
    // std::cout << curr_mat.get_diffuse_colour() << std::endl;
    mat = curr_mat;
    // std::cout << mat.get_diffuse_colour() << std::endl;

}