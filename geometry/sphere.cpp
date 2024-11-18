#include "sphere.h"
#include "../vec/vec3_utils.h"

using namespace vec3_utils;

sphere::sphere(const point center, double radius, const material& mat) : center(center), radius(std::fmax(0, radius)), material_settings(mat) {};

const material& sphere::get_material() const {
    return material_settings;
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
            // history.point += outward_norm * 1e-4;
            history.set_face_of_norm(r, outward_norm);
            // std::cout << "diffuse colour: " << material_settings.get_diffuse_colour() << std::endl;
            history.set_material(material_settings);
            // std::cout << "from history: " << history.mat.get_diffuse_colour() << std::endl;




            return true;
        }

const std::string& sphere::get_name() const {
    return "sphere";
}
