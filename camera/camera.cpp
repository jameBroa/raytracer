#include "camera.h"
#include <cmath>
#include <fstream>
#include "../light/light.h"
#include <random>
#include <string>
#include "../ray/bvh/bvh.h"
#include <chrono>
#include <iostream>
#include "camera_utils.h"

using point = vector3;
using namespace vec3_utils;
using namespace camera_utils;

// constructor
camera::camera(double fov, point cc, vector3 look, vector3 up, image img, double expos, std::string rm, double nb, rgb bg, double lr, double fd, double ns) : 
    field_of_view(fov),
    camera_center(cc),
    lookAt(look),
    upVector(up),
    exposure(expos),
    rendermode(rm),
    viewport_width(calculate_vp_width(img, cc, look, fov, rm)),
    viewport_height(calculate_vp_height(img, cc, look, fov, rm)), 
    focal_length(calculate_focal_length(fov, img)), 
    viewport_u(calculate_vp_u()), 
    viewport_v(calculate_vp_v()),
    pixel_gap_u(calculate_pg_u(img)), 
    pixel_gap_v(calculate_pg_v(img)),
    vp_ul_pixel(calculate_ul_pixel()), 
    pixel_0(calculate_pixel_0()),
    numbounces(nb),
    bg_col(bg),
    lens_radius(lr),
    focus_dist(fd),
    numsamples(ns)
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


double camera::calculate_vp_width(image img, vector3 cc, vector3 look, double fov, std::string rm) const {
    double vph = calculate_vp_height(img, cc, look, fov, rm);
    return vph * (double(img.image_width)/double(img.image_height));
}

double camera::calculate_vp_height(image img, vector3 cc, vector3 look, double fov, std::string rm) const {
    vector3 cam_dir = (look - cc);
    vector3 vp_center = camera_center + (cam_dir * 1);
    double fov_rad = (fov*3.14159265) / 180.0;
    double test_rad = 2 * atan(tan(fov_rad/2)/ (double(img.image_width)/double(img.image_height)));
    double vph = 2 * cam_dir.length() * tan(fov_rad/2);

    return vph;
}

// Deprecated
double camera::calculate_focal_length(double fov, image img) const {
    return 1;
}

// u horizontal pixels
vector3 camera::calculate_vp_u() const {
    vector3 a = unit_vector(lookAt - camera_center);
    vector3 b = unit_vector(cross(upVector, a));
    return b * viewport_width*1;
}
// v vertical pixels
vector3 camera::calculate_vp_v() const {
    vector3 a = unit_vector(lookAt - camera_center);
    vector3 b = unit_vector(cross(upVector, a));
    vector3 c = cross(a, b);
    return -c * viewport_height*1;
}

vector3 camera::calculate_pg_u(image img) const {
    return viewport_u / img.image_width;
}

vector3 camera::calculate_pg_v(image img) const {
    return viewport_v / img.image_height;
}

vector3 camera::calculate_ul_pixel() const {
    vector3 cam_dir = lookAt - camera_center;
    vector3 vp_center = camera_center + cam_dir * 1;
    return vp_center - viewport_v/2 - viewport_u/2;
}

vector3 camera::calculate_pixel_0() const {
    return vp_ul_pixel + 0.5 * (pixel_gap_u + pixel_gap_v);
}

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

    int samples_per_pixel = numsamples; // Higher sample count for random sampling
    // std::cout << "numsamples: " << samples_per_pixel << std::endl;
    double inv_samples = 1.0 / (samples_per_pixel / 1.5);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);


    for (int i = 0; i < img.image_height; i++) {
        for (int j = 0; j < img.image_width; j++) {
            std::cout << "Rendering pixel: " << i << " " << j << std::endl;
            // std::cout << lens_radius << std::endl;
            rgb pixel_color(0, 0, 0);

            // Random sampling within each pixel
            for (int s = 0; s < samples_per_pixel; s++) {
                double u = (j + dis(gen)) / (img.image_width - 1);
                double v = (i + dis(gen)) / (img.image_height - 1);

                vector3 pixel_center = pixel_0 + (u * (viewport_u)) + (v * (viewport_v));
                vector3 ray_direction = pixel_center - camera_center;
                ray r;
                if(lens_radius == 0 and focus_dist == 0 ) {
                    // No lens sampling
                   r = ray(camera_center, ray_direction); // Default behaviour!!!!
                } else {
                    vector3 rd = lens_radius * random_in_unit_disk();
                    vector3 focus_point = camera_center + focus_dist * ray_direction;
                    r = ray(camera_center + rd, focus_point - (camera_center + rd)); 
                }
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
            pixel = apply_tone_mapping(pixel);
        } else {
            pixel = apply_exposure(pixel);
        }
    }

    // Output the adjusted image
    std::ofstream image(filename += ".ppm");
    image << "P3\n" << img.image_width << " " << img.image_height << "\n255\n";
    for (const auto& pixel : pixel_buffer) {
        double red_convert = int(255.9999 * pixel.x());
        double green_convert = int(255.9999 * pixel.y());
        double blue_convert = int(255.9999 * pixel.z());
        image << red_convert << ' ' << green_convert << ' ' << blue_convert << '\n';
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Render time: " << duration.count() << " seconds" << std::endl;

}

rgb camera::ray_colour(const ray& r, const ray_intersectable& world, light_list lights) {
    intersect_history history;
    if(world.intersect(r, 0, std::numeric_limits<double>::infinity(), history)) {
        // return rgb(1.0,0,0);
        rgb colour;
        if(rendermode == "disney") {
            // std::cout << "numbounces: " << numbounces << std::endl;
            colour = lights.pathtrace(history, r, world, numbounces);
            // std::cout << "Camera output: " << colour << std::endl;
        } else if(rendermode == "phong") {
            colour = lights.calculate_lighting(history, world, r, numbounces);
        } else if(rendermode == "binary") {
            return rgb(1.0,0,0);
        }
        return colour;
    }
    return bg_col;
} 