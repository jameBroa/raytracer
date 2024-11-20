#include "sphere.h"
#include <cmath>
#include "../vec/vec3_utils.h"

using namespace vec3_utils;

sphere::sphere(const point center, double radius, const material& mat) : center(center), radius(std::fmax(0, radius)), material_settings(mat) {};

const material& sphere::get_material() const {
    return material_settings;
}

void sphere::get_uv(const point& p, double& u, double& v) const {
    // u = 0.5;
    // v= 0.5;

    // vec3 norm = unit_vector(p - center);
    
    // // Replace trig functions with faster approximations
    // u = 0.5 + std::atan2(norm.z(), norm.x()) / (2 * M_PI);
    // v = 0.5 - std::asin(norm.y()) / M_PI;
    
    vec3 norm = unit_vector(p - center);
    double phi = std::atan2(norm.z(), norm.x());
    double theta = std::asin(norm.y());
    u = 1 - (phi + M_PI) / (2*M_PI);
    v = (theta + M_PI/2) / M_PI;
    // std::cout << "u: " << u << std::endl;
    // std::cout << "v: " << v << std::endl;
    
}

bool sphere::intersect(const ray&r, 
        double ray_inters_min, 
        double ray_inters_max,
        intersect_history& history
        ) const {
            vec3 oc = center - r.origin();
            double a = r.direction().length_squared();
            double h = dot(r.direction(), oc);
            double c = oc.length_squared() - radius*radius;
            double discriminant = h*h - a*c;

            if(discriminant < 0){
                return false;
            }

            double discriminant_sqrtd = std::sqrt(discriminant); 
            double root = (h - discriminant_sqrtd) / a;
            // std::cout << "root: " << root << " min: " << ray_inters_min << " max: " << ray_inters_max << "\n";
            
            if (root <= ray_inters_min || root >= ray_inters_max) {
                root = (h + discriminant_sqrtd) / a;

                if(root <= ray_inters_min || root >= ray_inters_max) {
                    return false;
                }
            }

            history.time = root;
            history.point = r.position_at(root);
            vec3 outward_norm = (history.point - center) / radius;
            history.set_face_of_norm(r, outward_norm);
            history.set_material(material_settings);
            double u, v;
            get_uv(history.point, u, v);
            history.u = u;
            history.v = v;
            return true;
        }

const std::string& sphere::get_name() const {
    return "sphere";
}
