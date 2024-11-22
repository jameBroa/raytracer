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
    vec3 I_A = rgb(0.05,0.05,0.05);

    if(num_bounces <= 0) {
        return I_A;
    }

    rgb default_light = rgb(0, 0, 0);
    rgb reflected_light = rgb(0, 0, 0);
    rgb refracted_light = rgb(0, 0, 0);

    for(const shared_ptr<light>& light : all_lights) {
        bool in_shadow = light->is_in_shadow(history.point + history.normal*1e-5, world);
        double dist_to_light = (light->location - history.point).length();

        vec3 light_dir = unit_vector(light->location - history.point);
        vec3 cam_dir = unit_vector(r.direction());

        material mat = history.mat;
        vec3 H = unit_vector(light_dir + cam_dir);


        // Blinn-phong shading
        // vec3 I_D = 
        // mat.get_kd() * 
        // (light->intensity * (1 / ((1*1))))*
        // std::max(0.0, dot(history.normal, light_dir)) 
        // * mat.get_diffuse_colour();


        double cos_theta_d = dot(history.normal, light_dir);
        double cos_theta_v = dot(history.normal, cam_dir);
        double roughness = 0.01; //ranges [0,1]
        double diffuse_reflectance = 0.4; //ranges [0,1]
        double FD90 = 0.5 + 2 * (roughness) * (cos_theta_d * cos_theta_d);
        vec3 DI = (diffuse_reflectance / M_PI) * (1+ (FD90 - 1) *(std::pow(1-cos_theta_d, 5.0))) * ((1.0 + (FD90 - 1.0)) * (std::pow(1-cos_theta_v, 5.0))) * mat.get_diffuse_colour();

        vec3 I_D = 
        DI * 
        (light->intensity * (1 / ((1*1))))*
        std::max(0.0, dot(history.normal, light_dir));



        // Blinn-phong shading
        // vec3 I_S = 
        // mat.get_ks() * 
        // (light->intensity * (1 / ((1*1)))) *
        // std::pow(std::max(0.0, dot(H, history.normal)), mat.get_alpha()) * 
        // mat.get_specular_colour();

        // GGX
        vec3 N = history.normal;
        vec3 L = light_dir;
        vec3 V = cam_dir;


        double NdotV = std::max(dot(N, V), 0.0);
        double NdotL = std::max(dot(N, L), 0.0);
        double NdotH = std::max(dot(N, H), 0.0);
        double HdotV = std::max(dot(H, V), 0.0);
        double LdotH = std::max(dot(L, H), 0.0);

        // std::cout << "NdotV: " << NdotV << std::endl;
        // std::cout << "NdotL: " << NdotL << std::endl;
        // std::cout << "NdotH: " << NdotH << std::endl;
        // std::cout << "HdotV: " << HdotV << std::endl;
        // std::cout << "LdotH: " << LdotH << std::endl;

        // Normal Distribution Function
        double a = roughness * roughness;
        double a2 = a * a;
        double NdotH2 = NdotH * NdotH;
        double denom = (NdotH2 * (a2 - NdotH) * NdotH + 1.0);
        double D = a2 / (M_PI * denom * denom);

        // Geometry Function
        double ro = (roughness + 1.0);
        double k = (ro * ro) / 0.5;
        double ggx1 = NdotV / (NdotV * (1.0 - k) + k);
        double ggx2 = NdotL / (NdotL * (1.0 - k) + k);
        double G = ggx1 * ggx2;

        // Fresnel Term
        vec3 F0 = mat.get_specular_colour();
        vec3 F = F0 + ((rgb(1.0, 1.0, 1.0) - F0) * std::pow(1.0 - LdotH, 10.0));

        // GGX BRDF
        vec3 I = (D * G * F) / (4.0 * std::max(NdotV * NdotL, 1e-6));

        // std::cout << "D: " << D << std::endl;
        // std::cout << "G: " << G << std::endl;
        // std::cout << "F: " << F << std::endl;
        // std::cout << "I: " << I << std::endl;

        // Specular lighting
        vec3 I_S = I + NdotL/M_PI * light->intensity * std::pow(std::max(0.0, dot(H, history.normal)), mat.get_alpha()); 

        
        
        if(mat.get_texture()->dirname != "\0") {
            vec3 txt_color = mat.get_texture()->sample(history.u, history.v);
            I_D = I_D * txt_color;
            I_S = I_S * txt_color;
        }

        if(in_shadow == true) {
            if (mat.get_isreflective() && mat.get_reflectivity() > 0) {
                vec3 reflect_dir = unit_vector(reflect(r.direction(), history.normal));
                ray refl_ray = ray(history.point + reflect_dir * 1e-4, reflect_dir);
                intersect_history refl_hist;
                if (world.intersect(refl_ray, 0, inf, refl_hist)) {
                    vec3 reflected_light = calculate_lighting(refl_hist, world, refl_ray, num_bounces - 1);
                    default_light += reflected_light;
                } else {
                    default_light = rgb(0.25,0.25,0.25); // Add ambient light scaled by reflectivity
                    return rgb(0.25,0.25,0.25);
                }
                default_light += (I_D + I_S) * (1.0 - mat.get_reflectivity());
            } else {
                // Darken the color for shadows
            default_light += (I_D+I_S) * NdotL * light->intensity * 0.5;
            }
        } else {
            default_light += (I_D+I_S) * NdotL * light->intensity;
        }

        if(mat.get_isreflective() && mat.get_reflectivity() > 0){
            vec3 reflect_dir = unit_vector(reflect(r.direction(), history.normal));
            ray refl_ray = ray(history.point + reflect_dir * 1e-4, reflect_dir);
            intersect_history refl_hist;
            if(world.intersect(refl_ray, 0, inf, refl_hist)){
                rgb refl_light = calculate_lighting(refl_hist, world, refl_ray, num_bounces-1);
                if(light->is_in_shadow(refl_hist.point + refl_hist.normal * 1e-4, world)) {
                    refl_light *= 0.95;
                }
                reflected_light = refl_light * mat.get_reflectivity(); // Accumulate reflected color
            } else {
                reflected_light = rgb(0.25, 0.25, 0.25);
            }
            reflected_light += (I_D + I_S) * (1.0 - mat.get_reflectivity());
        }

        if(mat.get_isrefractive() && mat.get_refractiveindex() > 0) {
            vec3 refrac_dir;
            bool did_refract = does_refract(r.direction(), history.normal, mat.get_refractiveindex(), refrac_dir);
            if(did_refract){
                ray refr_ray = ray(history.point + refrac_dir * 1e-4, refrac_dir);
                intersect_history refr_hist;
                if(world.intersect(refr_ray, 0, inf, refr_hist)){
                    rgb curr_refr_light = calculate_lighting(refr_hist, world, refr_ray, num_bounces-1);
                    refracted_light += curr_refr_light;
                } else {
                    refracted_light += rgb(0.25, 0.25, 0.25);
                }
            } else {
                refracted_light += (I_D + I_S);
            }
        }
    }

    rgb final_light;

    if(history.mat.get_isreflective() == true and history.mat.get_isrefractive() == true) {
        double fresnel_factor = 0.5;
        return fresnel_factor * reflected_light + (1.0 - fresnel_factor) * refracted_light;
    } else if (history.mat.get_isreflective() == true) {
        return reflected_light;
    } else if(history.mat.get_isrefractive() == true) {
        return refracted_light;
    } else{
        return default_light + I_A;
    }

}
