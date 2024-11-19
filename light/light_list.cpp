#include "light_list.h"
#include "../vec/vec3_utils.h"
#include "../camera/camera_utils.h"
#include "light_utils.h"
#include "../geometry/common_math.h"

using namespace vec3_utils;
using namespace camera_utils;
using namespace light_utils;
using namespace std;

light_list::light_list(shared_ptr<light> lights) {
    add(lights);
}
light_list::light_list() {

}

void light_list::clear() {
    all_lights.clear();
}

void light_list::add(shared_ptr<light> light) {
    all_lights.push_back(light);
}

rgb light_list::calculate_lighting(
    const intersect_history& history, 
    const ray_intersectable& world, 
    const ray& r,
    int num_bounces
    ) const {
        // return rgb(0.8,0.,0.);
    rgb total_light = rgb(0, 0, 0);
    vec3 I_A = rgb(0.05,0.05,0.05);

    if(num_bounces <= 0) {
        return I_A;
    }

    if(1 > 0) {
        for(const shared_ptr<light>& light : all_lights) {
            // small intolerance to prevent self-collision
            bool in_shadow = light->is_in_shadow(history.point + history.normal*1e-5, world);

            //light->location: position of light source
            double dist_to_light = (light->location - history.point).length();

            vec3 light_dir = unit_vector(light->location - history.point);
            vec3 cam_dir = unit_vector(r.direction());

            // material* mat = history.mat;
            material mat = history.mat;
            vec3 H = unit_vector(light_dir + cam_dir);


            // vec3 I_D = mat.get_kd() * light->intensity * (1 / (dist_to_light * dist_to_light));

            vec3 I_D = 
            mat.get_kd() * 
            (light->intensity * (1 / ((1*1))))*
            std::max(0.0, dot(history.normal, light_dir)) 
            * mat.get_diffuse_colour();

            // vec3 I_D = history.mat->get_diffuse_colour();
            // std::cout << mat->get_diffuse_colour() << ", " << history.mat->get_diffuse_colour() << std::endl;
            // vec3 I_D = mat->get_diffuse_colour();
            // std::cout << mat.get_diffuse_colour() << ", " << history.mat.get_diffuse_colour() << std::endl;
            // vec3 I_D = mat.get_diffuse_colour();
            // std::cout << "I_D (dereference): " << I_D << std::endl;
            // std::cout << "I_D: " << history.mat->get_diffuse_colour() << std::endl;
            

            // vec3 I_D = 
            // mat.get_kd() * 
            // std::max(0.0, dot(history.normal, light_dir)) 
            // * mat.get_diffuse_colour();
                    
                    

            vec3 I_S = 
            mat.get_ks() * 
            (light->intensity * (1 / ((1*1)))) *
            std::pow(std::max(0.0, dot(H, history.normal)), mat.get_alpha()) * 
            mat.get_specular_colour();

            // vec3 I_S = 
            // mat.get_ks() * 
            // std::pow(std::max(0.0, dot(H, history.normal)), mat.get_alpha()) * 
            // mat.get_specular_colour();



            // Shadow should be darkened colour of whatever is in shadow
            // total_light += (I_D+I_S);
            if(in_shadow == true) {
                // std::cout<<"Shadow"<<std::endl;
                if (mat.get_isreflective() && mat.get_reflectivity() > 0) {
                    vec3 reflect_dir = unit_vector(reflect(r.direction(), history.normal));
                    ray refl_ray = ray(history.point + reflect_dir * 1e-4, reflect_dir);
                    intersect_history refl_hist;
                    if (world.intersect(refl_ray, 0, inf, refl_hist)) {
                        // Calculate the lighting for the reflected ray
                        vec3 reflected_light = calculate_lighting(refl_hist, world, refl_ray, num_bounces - 1);
                        total_light += reflected_light * mat.get_reflectivity(); // Accumulate reflected color
                        return reflected_light;
                    } else {
                        // Handle case where reflection ray does not hit any object
                        total_light = rgb(0.25,0.25,0.25); // Add ambient light scaled by reflectivity
                        return rgb(0.25,0.25,0.25);
                    }
                    // std::cout << "Shadow" << std::endl;
                } else {
                    // Darken the color for shadows
                    // total_light = rgb(0.8,0.5,0.8);
                    total_light += (I_D+I_S) * 0.5;
                }
            } else {

                total_light += (I_D+I_S);
            }
                 
        }

        material obj_material = history.mat;
        rgb reflected_light = rgb(0,0,0);
        for(const shared_ptr<light>& light : all_lights) {
            if(obj_material.get_isreflective() && obj_material.get_reflectivity() > 0){
                vec3 reflect_dir = unit_vector(reflect(r.direction(), history.normal));
                ray refl_ray = ray(history.point + reflect_dir * 1e-4, reflect_dir);
                intersect_history refl_hist;
                if(world.intersect(refl_ray, 0, inf, refl_hist)){
                    rgb refl_light = calculate_lighting(refl_hist, world, refl_ray, num_bounces-1);
                    if(light->is_in_shadow(refl_hist.point + refl_hist.normal * 1e-4, world)) {
                        refl_light *= 0.95;
                    }
                    reflected_light = refl_light * obj_material.get_reflectivity(); // Accumulate reflected color
                } else {
                    reflected_light = rgb(0.25, 0.25, 0.25);
                }
            }

            if(obj_material.get_isrefractive() && obj_material.get_refractiveindex() > 0) {
                vec3 refrac_dir;
                bool did_refract = does_refract(r.direction(), history.normal, obj_material.get_refractiveindex(), refrac_dir);

                if(did_refract){
                    ray refr_ray = ray(history.point + refrac_dir * 1e-4, refrac_dir);
                    intersect_history refr_hist;
                    if(world.intersect(refr_ray, 0, inf, refr_hist)){
                        rgb refr_light = calculate_lighting(history, world, refr_ray, num_bounces-1);
                        total_light += refr_light;
                    } else {
                        total_light = rgb(0.25, 0.25, 0.25);
                    }
                }
            }

        }
        if(history.mat.get_isreflective() == true){
            return reflected_light;
            // total_light = reflected_light;
        }

        

        

        
        total_light += I_A;

        return total_light;
    } else {
       return rgb(0,0,0);
    }
}
