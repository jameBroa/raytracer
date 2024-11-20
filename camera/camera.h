#include "../vec/vec3.h"
#include "../image/image.h"
#include "../ray/ray_intersectable_list.h"
#include "../light/light_list.h"
#include <string>



#ifndef CAMERA_H
#define CAMERA_H

using point = vec3;
class camera{
    public:
        // fl: focal length
        // vp: viewport (vph -> viewport height)
        // cc: camera center
        // pg: pixel gap
        camera(double fl, point cc, vec3 lookAt, vec3 upVector, image img, double expos);

        double const focal_length;
        double const field_of_view;
        double const viewport_height;
        double const viewport_width;
        double const distance_to_vp = 1;
        double const exposure;
    

        point const camera_center;

        // Define's camera orientation
        const vec3 lookAt; 
        const vec3 upVector;


        vec3 const viewport_u; // horizontal vp edges
        vec3 const viewport_v; // vertical vp edges

        vec3 const pixel_gap_u; //space between pixels
        vec3 const pixel_gap_v;

        
        vec3 const vp_ul_pixel; // location in 3d
        vec3 const pixel_0; // location in 2d

        // helper initialization functions
        double calculate_vp_width(image img, vec3 cc, vec3 look, double fov) const;
        double calculate_focal_length(double fov, image img) const;
        double calculate_vp_height(image img, vec3 cc, vec3 look, double fov) const;

        vec3 calculate_vp_u() const;
        vec3 calculate_vp_v() const;
        vec3 calculate_pg_u(image img) const;
        vec3 calculate_pg_v(image img) const;

        vec3 calculate_ul_pixel() const;
        vec3 calculate_pixel_0() const;
        rgb ray_colour(const ray& r, const ray_intersectable& world, const light_list lights);
        void render(image img, ray_intersectable_list env, light_list lights, std::string filename);
        rgb apply_exposure(const rgb& color);
};


#endif