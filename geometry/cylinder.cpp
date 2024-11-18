#include "cylinder.h"
#include "../vec/vec3_utils.h"
#include "common_math.h"

using namespace vec3_utils;
using namespace std;


cylinder::cylinder(const point ctr,
                    const vec3 ax,
                    const double r,
                    const double h,
                    const material& mat) :
                    center(ctr), axis(ax), radius(r), height(h), material_settings(mat) {};

const material& cylinder::get_material() const {
    return material_settings;
}

bool cylinder::intersect(const ray& r,
                    double ray_inters_min, 
        double ray_inters_max,
        intersect_history& history
        ) const {
             // D: r.direction()
             // X: oc: cap_2 - r.origin()
             // V: Axis
            point cap_1 = center - (axis * (height)); // bottom cap
            point cap_2 = center + (axis * (height)); // top top

            // vec3 oc = cap_2 - r.origin();
            vec3 oc = center - r.origin();  // Use cylinder center as reference

            double a = r.direction().length_squared() - (dot(r.direction(), axis)*dot(r.direction(), axis));
            double h = dot(r.direction(), oc) - (dot(r.direction(), axis)*dot(oc, axis));
            // double b = dot(r.direction() - axis*dot(r.direction(), axis), oc - axis * dot(oc, axis));
            
            
            double c = oc.length_squared() - dot(oc, axis)*dot(oc, axis) - radius*radius;
            // return false; // If uncommented then scene is green
            double closest_t = ray_inters_max;
            bool did_hit = false;

            // return false; // return statement here makes everything gray?
            // intersect_history temp_history;

            double discriminant = h*h - a*c;
            if(discriminant < 0){
                // history = intersect_history();
                return false;
            }

            double discriminant_sqrtd = std::sqrt(discriminant); 
            
            // double root = (h - discriminant_sqrtd) / a;
            

            //  if (root <= ray_inters_min || root >= ray_inters_max) {

            //     double root = (h + discriminant_sqrtd) / a;

            //     if(root <= ray_inters_min || root >= ray_inters_max) {
            //         return false;
            //     }

            // }

            // This works for the reflection scene
            double t0 = (h - discriminant_sqrtd) / a;
            double t1 = (h + discriminant_sqrtd) / a;
            double root;

            // Ensure t0 is the smaller root and t1 is the larger root
            if (t0 > t1) std::swap(t1, t0);

            // Check if the smallest root (t0) is within bounds
            if (t0 > ray_inters_min && t0 < ray_inters_max) {
                root = t0;
            } else if (t1 > ray_inters_min && t1 < ray_inters_max) {
                // If t0 is out of bounds, check the larger root (t1)
                root = t1;
            } else {
                // Both roots are out of bounds
                // history = intersect_history();
                return false;
            }

           


            // General notes:
            // Something is going wrong with cap rendering. the colour is wrong

             

            double t_cap_2;
            bool called = false;
            if (intersect_plane(r, cap_2, -axis, t_cap_2, ray_inters_min, ray_inters_max)) {
                
                point p = r.position_at(t_cap_2) - cap_2;
                    if (p.length_squared() <= radius*radius && t_cap_2 < closest_t - 1e-4) {
                        std::cout << "top cap called" << std::endl;
                        // std::cout << p << std::endl;
                        // std::cout << "t_cap_2: " << t_cap_2 << std::endl;
                        called = true;
                        history.time = t_cap_2;
                        history.point = r.position_at(t_cap_2);
                        history.set_face_of_norm(r, axis);
                        history.set_material(material_settings);
                        // history.point += axis * 1e-4;
                        did_hit = true;
                        closest_t = t_cap_2;
                        return true;
                        // history.time = t_cap_2;
                        // history.point = r.position_at(t_cap_2);
                        // history.set_face_of_norm(r, axis);
                        // // history.point += axis * 1e-4;
                        // history.set_material(material_settings);


                        // return true; // this returning true now makes scene.json work
                    }
                
            }

            // cap_2 or cap_1 made this work in reflected

            point p = r.position_at(root);
            vec3 cp = p - center;  // Vector from center to hit point
            double projection = dot(cp, axis);  // Project onto axis
            if(projection < -height || projection > (height)) {
                return false;
            }
            
            // return false;
            if(0 < 1) {
                // double proj_on_axis = dot(r.position_at(root) - cap_1, axis);
                // if (proj_on_axis <= 0 || proj_on_axis >= height) {
                //     // std::cout << "failing proj." << std::endl;
                //     return false;
                // }
                
                double m = dot(r.direction(), axis*root) + dot(oc, axis);
                // vec3 outward_norm = ((r.direction() - cap_2) - (axis * m));
                // vec3 p = r.position_at(root);
                // vec3 a = dot(history.point - center, axis) * axis;
                // vec3 outward_norm = p - (center + a);
                // outward_norm = unit_vector(outward_norm);
                
                history.time = root;
                history.point = p;
                vec3 closest_point_on_axis = center + axis * projection;
                vec3 outward_norm = unit_vector(p - closest_point_on_axis);
                // vec3 outward_norm = unit_vector(p-center - dot(p - center, axis)*axis);
                history.point += outward_norm * 1e-4;
                history.set_face_of_norm(r, outward_norm);
                history.set_material(material_settings);

                closest_t = root;
                did_hit = true;
                
                return true;
            }

            double t_cap_1;

            if (intersect_plane(r, cap_1, axis, t_cap_1, ray_inters_min, ray_inters_max)) {
                    point p = r.position_at(t_cap_1) - cap_1;
                    if (p.length_squared() <= radius*radius && t_cap_1 < closest_t - 1e-4) {
                        history.time = t_cap_1;
                        history.point = r.position_at(t_cap_1);
                        history.set_face_of_norm(r, -axis);
                        history.set_material(material_settings);
                        did_hit = true;
                        closest_t = t_cap_1;

                        return true; //when uncommented, mirror.json or whatever doesn't work :(
                    }
                
            }  


            if(did_hit == true) {
                if (called == true) {
                    // std::cout << "returning now" << std::endl;
                    // std::cout << "point: " << history.point << " normal: " << history.normal << std::endl; 
                    // std::cout << "closest_t: " << closest_t << std::endl;
                }
                // history = temp_history;
                // std::cout << "point: " << history.point << " normal: " << history.normal << std::endl;
                // std::cout << "material col: " << history.mat->get_diffuse_colour() << std::endl;
                return true;
            }
        }

bool cylinder::intersect_plane(const ray& r, const point plane_point, const vec3 plane_normal, double& t, double min, double max) const {
    double denom = dot(r.direction(), plane_normal);
    if (std::abs(denom) < 1e-6) {  // Ray is parallel to the plane
        return false;
    }
    t = dot(plane_point - r.origin(), plane_normal) / denom;
    return (t >= min + 1e-4 && t <= max - 1e-4);
}

const std::string& cylinder::get_name() const {
    return "cylinder";
}

