#include "../vec/vector3.h"
#include "../image/image.h"
#include "../ray/ray_intersectable_list.h"
#include "../light/light_list.h"
#include <string>



#ifndef CAMERA_H
#define CAMERA_H

using point = vector3;
class camera{
    public:
        // fl: focal length
        // vp: viewport (vph -> viewport height)
        // cc: camera center
        // pg: pixel gap
        camera(double fl, point cc, vector3 lookAt, vector3 upVector, image img, double expos, std::string rm, double numbounces, rgb bg, double lr, double fd, double numsamples);

        double const focal_length;
        double const field_of_view;
        double const viewport_height;
        double const viewport_width;
        double const distance_to_vp = 1;
        double const exposure;
        const std::string rendermode;
        double const numbounces;
        rgb bg_col;
        double const lens_radius;
        double const focus_dist;
        double const numsamples;


        point const camera_center;

        // Define's camera orientation
        const vector3 lookAt; 
        const vector3 upVector;



        vector3 const viewport_u; // horizontal vp edges
        vector3 const viewport_v; // vertical vp edges

        vector3 const pixel_gap_u; //space between pixels
        vector3 const pixel_gap_v;

        
        vector3 const vp_ul_pixel; // location in 3d
        vector3 const pixel_0; // location in 2d

        // helper initialization functions
        double calculate_vp_width(image img, vector3 cc, vector3 look, double fov, std::string rm) const;
        double calculate_focal_length(double fov, image img) const;
        double calculate_vp_height(image img, vector3 cc, vector3 look, double fov, std::string rm) const;

        vector3 calculate_vp_u() const;
        vector3 calculate_vp_v() const;
        vector3 calculate_pg_u(image img) const;
        vector3 calculate_pg_v(image img) const;

        vector3 calculate_ul_pixel() const;
        vector3 calculate_pixel_0() const;
        
        // Calculate the color of the ray (bg color if not intersection, otherwise calculate lighting) 
        rgb ray_colour(const ray& r, const ray_intersectable& world, const light_list lights);
        // Main render function
        void render(image img, ray_intersectable_list env, light_list lights, std::string filename);
    
        rgb apply_exposure(const rgb& color);
        rgb apply_tone_mapping(rgb color);
        double calculate_dynamic_range(const std::vector<rgb> pixel_buffer, double& min, double& max);
};


#endif