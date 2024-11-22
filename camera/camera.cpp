#include "camera.h"
#include <cmath>
#include <fstream>
#include "../common.h"
#include "../light/light.h"
#include <random>
#include <string>
#include "../ray/bvh/bvh.h"
#include <chrono>
#include <iostream>
#include "camera_utils.h"

using point = vec3;
using namespace vec3_utils;
using namespace std;
using namespace camera_utils;

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
    rgb exposed_col = rgb(
        std::min(1.0, color.x() * expos_fac),
        std::min(1.0, color.y() * expos_fac),
        std::min(1.0, color.z() * expos_fac)
    );
    return exposed_col;
}

rgb camera::apply_tone_mapping(rgb color) {
    double luminance = 0.2126 * color.x() + 0.7152 * color.y() + 0.0722 * color.z();
    double m = luminance / (1.0 + luminance);
    return color * (m / luminance);
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

    double test_rad = 2 * atan(tan(fov_rad/2)/ (double(img.image_width)/double(img.image_height)));

    double vph = 2 * cam_dir.length() * tan(fov_rad/2);
    return vph;
}

double camera::calculate_focal_length(double fov, image img) const {
    return 1;
}

// u horizontal pixels
vec3 camera::calculate_vp_u() const {
    vec3 a = unit_vector(lookAt - camera_center);
    vec3 b = unit_vector(cross(upVector, a));
    return b * viewport_width*1.5;
}
// v vertical pixels
vec3 camera::calculate_vp_v() const {
    vec3 a = unit_vector(lookAt - camera_center);
    vec3 b = unit_vector(cross(upVector, a));
    vec3 c = cross(a, b);
    return -c * viewport_height*1.5;
}

vec3 camera::calculate_pg_u(image img) const {
    return viewport_u / img.image_width;
}

vec3 camera::calculate_pg_v(image img) const {
    return viewport_v / img.image_height;
}

vec3 camera::calculate_ul_pixel() const {
    vec3 cam_dir = lookAt - camera_center;
    vec3 vp_center = camera_center + cam_dir * 1;
    return vp_center - viewport_v/2 - viewport_u/2;
}

vec3 camera::calculate_pixel_0() const {
    return vp_ul_pixel + 0.5 * (pixel_gap_u + pixel_gap_v);
}

// void camera::render(image img, ray_intersectable_list env, light_list lights, std::string filename) {
//     ofstream image(filename += ".ppm");
//     image << "P3\n" << img.image_width << " " << img.image_height << "\n255\n";
//     int samples_per_pixel = 2; // Higher sample count for random sampling
//     double inv_samples = 1.0 / (samples_per_pixel/1.5);
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<> dis(0, 1);

//     for(int i = 0; i < img.image_height; i++) {
//         for(int j = 0; j < img.image_width; j++) {

//             rgb pixel_color(0, 0, 0);

//             // Random sampling within each pixel
//             for (int s = 0; s < samples_per_pixel; s++) {
//                 double u = (j + dis(gen)) / (img.image_width - 1);
//                 double v = (i + dis(gen)) / (img.image_height - 1);

//                 vec3 pixel_center = pixel_0 + (u * (viewport_u)) + (v * (viewport_v));
//                 vec3 ray_direction = pixel_center - camera_center;

//                 ray r = ray(camera_center, ray_direction);
//                 pixel_color += ray_colour(r, env, lights);
//             }

//             pixel_color *= inv_samples;
//             pixel_color = apply_exposure(pixel_color);

//             colour_utils::output_colour(image, pixel_color);
//         }
//     }
// }


double camera::calculate_dynamic_range(const std::vector<rgb> pixel_buffer, double& min, double& max) {
    double min_lum = std::numeric_limits<double>::max();
    double max_lum = std::numeric_limits<double>::min();

    for(const auto pixel: pixel_buffer) {
        double curr_lum = 0.2126 * pixel.x() + 0.7152 * pixel.y() + 0.0722 * pixel.z();
        if(curr_lum < min_lum) {
            min_lum = curr_lum;
        } else if (curr_lum > max_lum) {
            max_lum = curr_lum;
        }
    }
    min = min_lum;
    max = max_lum;
    return max_lum / min_lum;
}



void camera::render(image img, ray_intersectable_list env, light_list lights, std::string filename) {
    std::vector<rgb> pixel_buffer(img.image_width * img.image_height);
    bvh_node bvh(env.all_intersectable_objs, 0, env.all_intersectable_objs.size());

    auto start = std::chrono::high_resolution_clock::now();

    // Scene.json WITH NO LENS SAMPLING + limited pixel sampling
    // 9.16 seconds without BVH
    // 8.04, 8.0038 seconds with BVH

    // texture_spheres
    // 2.8381 seconds without BVH
    // 2.1081 seconds with BVH

    int samples_per_pixel = 2; // Higher sample count for random sampling
    double inv_samples = 1.0 / (samples_per_pixel / 1.5);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    double lens_radius = 0.1;
    double focus_dist = 1.0;

    for (int i = 0; i < img.image_height; i++) {
        for (int j = 0; j < img.image_width; j++) {
            rgb pixel_color(0, 0, 0);

            // Random sampling within each pixel
            for (int s = 0; s < samples_per_pixel; s++) {
                double u = (j + dis(gen)) / (img.image_width - 1);
                double v = (i + dis(gen)) / (img.image_height - 1);

                vec3 pixel_center = pixel_0 + (u * (viewport_u)) + (v * (viewport_v));
                vec3 ray_direction = pixel_center - camera_center;
                // vec3 rd = lens_radius * random_in_unit_disk();
                // vec3 focus_point = camera_center + focus_dist * ray_direction;
                // ray r = ray(camera_center + rd, focus_point - (camera_center + rd));

                ray r = ray(camera_center, ray_direction); // Default behaviour!!!!
                pixel_color += ray_colour(r, bvh, lights);
            }

            pixel_color *= inv_samples;
            pixel_buffer[i * img.image_width + j] = pixel_color;
        }
    }

    // Step 2: Analyze the Dynamic Range
    double min_lum, max_lum;
    double dynamic_range = calculate_dynamic_range(pixel_buffer, min_lum, max_lum);

    // Step 3: Apply Adjustments
    double dynamic_range_threshold = 20.0; // Example threshold
    for (auto& pixel : pixel_buffer) {
        if (dynamic_range > dynamic_range_threshold) {
            pixel = apply_exposure(pixel);
            // pixel = apply_tone_mapping(pixel);
        } else {
            pixel = apply_exposure(pixel);
        }
    }

    // Output the adjusted image

    ofstream image(filename += ".ppm");
    image << "P3\n" << img.image_width << " " << img.image_height << "\n255\n";
    for (const auto& pixel : pixel_buffer) {
        colour_utils::output_colour(image, pixel);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Render time: " << duration.count() << " seconds" << std::endl;

}
// No BVH
// void camera::render(image img, ray_intersectable_list env, light_list lights, std::string filename) {
//     std::vector<rgb> pixel_buffer(img.image_width * img.image_height);

//     int samples_per_pixel = 2; // Higher sample count for random sampling
//     double inv_samples = 1.0 / (samples_per_pixel / 1.5);
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<> dis(0, 1);

//     for (int i = 0; i < img.image_height; i++) {
//         for (int j = 0; j < img.image_width; j++) {
//             rgb pixel_color(0, 0, 0);

//             // Random sampling within each pixel
//             for (int s = 0; s < samples_per_pixel; s++) {
//                 double u = (j + dis(gen)) / (img.image_width - 1);
//                 double v = (i + dis(gen)) / (img.image_height - 1);

//                 vec3 pixel_center = pixel_0 + (u * (viewport_u)) + (v * (viewport_v));
//                 vec3 ray_direction = pixel_center - camera_center;

//                 ray r = ray(camera_center, ray_direction);
//                 pixel_color += ray_colour(r, env, lights);
//             }

//             pixel_color *= inv_samples;
//             pixel_buffer[i * img.image_width + j] = pixel_color;
//         }
//     }

//     // Step 2: Analyze the Dynamic Range
//     double min_lum, max_lum;
//     double dynamic_range = calculate_dynamic_range(pixel_buffer, min_lum, max_lum);
//     std::cout << "dynamic range: " << dynamic_range << std::endl;

//     // Step 3: Apply Adjustments
//     double dynamic_range_threshold = 20.0; // Example threshold
//     for (auto& pixel : pixel_buffer) {
//         pixel = apply_exposure(pixel);
//         if (dynamic_range > dynamic_range_threshold) {
//             pixel = apply_exposure(pixel);
//             pixel = apply_tone_mapping(pixel);
//         } else {
//             pixel = apply_exposure(pixel);
//         }
//     }

//     // Output the adjusted image
//     ofstream image(filename += ".ppm");
//     image << "P3\n" << img.image_width << " " << img.image_height << "\n255\n";
//     for (const auto& pixel : pixel_buffer) {
//         colour_utils::output_colour(image, pixel);
//     }
// }

rgb camera::ray_colour(const ray& r, const ray_intersectable& world, const light_list lights) {
    intersect_history history;
    if(world.intersect(r, 0, inf, history)) {
        // return rgb(1.0,0,0);
        // Add number of bounces here!!!!!!!
        rgb colour = lights.calculate_lighting(history, world, r, 3);
        return colour;
    }
    // Make this be whatever background colour is!
    return rgb(0.25,0.25,0.25);
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