#include "camera.h"
#include <cmath>
#include <fstream>
#include "../common.h"
#include "../light/light.h"
#include <random>

using point = vec3;
using namespace vec3_utils;
using namespace std;

// constructor
camera::camera(double fov, point cc, vec3 look, vec3 up, image img, double expos) : 
    field_of_view(fov),
    camera_center(cc),
    lookAt(look),
    upVector(up),
    exposure(expos),
    viewport_width(calculate_vp_width(img, cc, look, fov)),
    viewport_height(calculate_vp_height(img, cc, look, fov)), 
    focal_length(calculate_focal_length(fov, img)), 
    viewport_u(calculate_vp_u()), 
    viewport_v(calculate_vp_v()),
    pixel_gap_u(calculate_pg_u(img)), 
    pixel_gap_v(calculate_pg_v(img)),
    vp_ul_pixel(calculate_ul_pixel()), 
    pixel_0(calculate_pixel_0())
     {};

rgb camera::apply_exposure(const rgb& color) {
    double expos_fac = std::pow(2.0, exposure);
    return rgb(
        std::min(1.0, color.x() * expos_fac),
        std::min(1.0, color.y() * expos_fac),
        std::min(1.0, color.z() * expos_fac)
    );
}


double camera::calculate_vp_width(image img, vec3 cc, vec3 look, double fov) const {

    double vph = calculate_vp_height(img, cc, look, fov);
    return vph * (double(img.image_width)/double(img.image_height));


    // Horizontal FOV
    // vec3 cam_dir = (lookAt - cc);
    // cout << cam_dir * 1 << endl;
    // vec3 vp_center = camera_center + (cam_dir * 1);
    // double fov_rad = (fov * 3.14159265) / 180.0;
    // double vpw = 2 * 1 * tan(fov_rad / 2);
    // return vpw;

    // Original att.
    // return viewport_height * (double(img.image_width)/double(img.image_height));
}

double camera::calculate_vp_height(image img, vec3 cc, vec3 look, double fov) const {
    // double vpw = calculate_vp_width(img, cc, look, fov);
    vec3 cam_dir = (lookAt - cc);
    vec3 vp_center = camera_center + (cam_dir * 1);
    double fov_rad = (fov*3.14159265) / 180.0;
    double vph = 2 * 1 * tan(fov_rad / 2);
    cout << "vph: " << vph << endl;
    return vph;

    // If horizontal FOV
    // return vpw / (double(img.image_width) / double(img.image_height));

    // double fov_rad = fov * 3.1415926 / 180.0;
    // return 2 * tan(fov_rad / 2);
}

double camera::calculate_focal_length(double fov, image img) const {
    // double fov_rad = fov * 3.1415926 / 180.0;
    // double vph = calculate_vp_height(fov);
    // double vpw = calculate_vp_width(img);
    // double x = std::sqrt(vph * vph + vpw * vpw);
    // return vpw / (2 * tan(fov_rad / 2));
    return 1;
}



// u horizontal pixels
vec3 camera::calculate_vp_u() const {
    vec3 a = unit_vector(lookAt - camera_center);
    vec3 b = unit_vector(cross(upVector, a));

    return b * viewport_width * 1.4;
}
// v vertical pixels
vec3 camera::calculate_vp_v() const {
    vec3 a = unit_vector(lookAt - camera_center);
    vec3 b = unit_vector(cross(upVector, a));
    vec3 c = cross(a, b);
    return -c * viewport_height * 1.4;
    // return vec3(0, -viewport_height, 0);
}

vec3 camera::calculate_pg_u(image img) const {
    return viewport_u / img.image_width;
}

vec3 camera::calculate_pg_v(image img) const {
    return viewport_v / img.image_height;
}

    // double fov_rad = field_of_view * 3.1415926 / 180.0;

    // double distance = viewport_height / (2 * tan(fov_rad / 2));

    // vec3 vp_center = camera_center + unit_vector((camera_center - lookAt)) * focal_length;

    // vec3 vp_up = cross(viewport_u, vp_center);
    // vec3 vp_left = viewport_u;
    // vec3 ul = vp_center + (vp_up * viewport_height/2) + (vp_left * viewport_width/2);
    
    // std::cout << "UL: " << ul << "\n";
    // // return ul;
    // vec3 new_pos = camera_center - unit_vector(lookAt - camera_center)*focal_length - viewport_u/2 - viewport_v/2;

vec3 camera::calculate_ul_pixel() const {
    // vec3 lookDir = unit_vector(lookAt - camera_center);
    // vec3 vp_center = camera_center + lookDir * 1;
    // vec3 vp_ul = vp_center - viewport_v/2 - viewport_u/2;
    // vec3 original_pos = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    
    // std::cout << "original pos: " << original_pos << "\n";
    // std::cout << "camera pos: " << vp_ul << "\n";

    // return original_pos; 

    vec3 cam_dir = lookAt - camera_center;
    vec3 vp_center = camera_center + cam_dir * 1;

    return vp_center - viewport_v/2 - viewport_u/2;

    // return vp_center - 0.5 * (viewport_u + viewport_v);
}

vec3 camera::calculate_pixel_0() const {
    return vp_ul_pixel + 0.5 * (pixel_gap_u + pixel_gap_v);
}

void camera::render(image img, ray_intersectable_list env, light_list lights) {
    ofstream image("example.ppm");
    image << "P3\n" << img.image_width << " " << img.image_height << "\n255\n";
    int samples_per_pixel = 8; // Higher sample count for random sampling
    double inv_samples = 1.0 / (samples_per_pixel/1.5);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for(int i = 0; i < img.image_height; i++) {
        for(int j = 0; j < img.image_width; j++) {
            rgb pixel_color(0, 0, 0);

            // Random sampling within each pixel
            for (int s = 0; s < samples_per_pixel; s++) {
                double u = (j + dis(gen)) / (img.image_width - 1);
                double v = (i + dis(gen)) / (img.image_height - 1);

                vec3 pixel_center = pixel_0 + (u * viewport_u) + (v * viewport_v);
                vec3 ray_direction = pixel_center - camera_center;

                ray r = ray(camera_center, ray_direction);

                pixel_color += ray_colour(r, env, lights);
            }

            pixel_color *= inv_samples;
            pixel_color = apply_exposure(pixel_color);

            colour_utils::output_colour(image, pixel_color);


            // vec3 pixel_center = pixel_0 + (j * pixel_gap_u) + (i * pixel_gap_v);
            // vec3 ray_direction = pixel_center - camera_center;
            // ray r = ray(camera_center, ray_direction);             

            // rgb pixel_colour = ray_colour(r, env, light_1);
            // colour_utils::output_colour(image, pixel_colour);
        }
    }
}

rgb camera::ray_colour(const ray& r, const ray_intersectable& world, const light_list lights) {
    intersect_history history;
    if(world.intersect(r, 0, inf, history)) {
        // return rgb(0.8,0.3,0.3);
        rgb colour = lights.calculate_lighting(history, world, r, 8);

        return colour;

        // bool in_shadow = light_1.is_in_shadow(history.point, world);

        // vec3 light_dir = unit_vector(light_1.location - history.point);
        // vec3 cam_dir = r.direction();
        // material mat = world.get_material();

        // // if(in_shadow){
        // //     return rgb(0, 1.0, 0) * 0.3 + rgb(0.01, 0.1, 0.01);
        // // }
        // rgb L_a = rgb(0.1,0.1,0.1); 
        // vec3 L_i = light_1.intensity;
        // double k_s = mat.get_ks();
        // double k_d = mat.get_kd();

        // vec3 H = (light_dir + cam_dir)/ (light_dir + cam_dir).length();

        // vec3 I_A = L_a;
        // vec3 I_D = k_d * L_i * std::max(0.0, dot(history.normal, light_dir)) * mat.get_diffuse_colour();
        // vec3 I_S = k_s * L_i * std::pow(std::max(0.0, dot(H, history.normal)), mat.get_alpha()) * mat.get_specular_colour();

        // rgb colour = I_A + I_D + I_S;

        // if(in_shadow){
        //     return colour * 0.2;
        // }
        // return colour;


        // double distance_to_light = (light_1.location - history.point).length();

        // // Apply attenuation (inverse square law)
        // double attenuation = 1.0 / (distance_to_light * distance_to_light);

        // // Calculate final color based on light intensity and attenuation
        // rgb object_color(1.0, 0, 0);  // Base color of the object (red)
        // vec3 light_intensity = light_1.intensity * attenuation;

        // // Modulate object color by attenuated light intensity
        // return object_color * light_intensity;
    }

    return rgb(0.25, 0.25, 0.25);
} 

// Should be used for binary primitives functionality

// rgb camera::ray_colour(const ray& r, const ray_intersectable& world, light light_1) {
//     intersect_history history;
//     if(world.intersect(r, 0, inf, history)) {
//         bool in_shadow = light_1.is_in_shadow(history.point, world);

//         if(in_shadow){
//             return rgb(0.5, 0, 0);
//         }

//         return rgb(1.0,0,0);

//         // return 0.5*(history.normal + rgb(1.0, 1.0, 1.0));
//     }

//     return rgb(0, 0, 0);

//     vec3 unit_dir = unit_vector(r.direction());
//     double a = 0.5 * (unit_dir.y() + 1.0);
//     return (1.0-a)*rgb(1.0,1.0,1.0) + a*rgb(0.5, 0.7, 1.0);
// } 