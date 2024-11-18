#include <fstream>
#include "./ray/ray_intersectable.h"
#include "./ray/ray_intersectable_list.h"
#include "./geometry/sphere.h"
#include "./geometry/cylinder.h"
#include "./geometry/triangle.h"
#include "./common.h"
#include "./light/light.h"
#include "./light/light_list.h"
#include <iostream>

using namespace std;
using namespace vec3_utils;
using point = vec3;

int main() 
{

    //Image things:
    image img = image(1200, 800);

    //Camera things:
    double angle = 0 * 3.1415926 / 180.0; // Convert to radians
    // vec3 lookAt = vec3(0, sin(angle), cos(angle)); 
    // Reflective scene   
    // vec3 lookAt = (vec3(0.25, -0.05, 1.0));
    // vec3 upVector = vec3(0,1,0);
    // camera cam = camera(80, point(-1, 0.5, -1.5), lookAt, upVector, img, 0.1);

    // Scene.json
    // vec3 lookAt = (vec3(0, 0.35, 1.0));
    // vec3 upVector = vec3(0,1,0);
    // camera cam = camera(45, point(0, 0.75, -0.25), lookAt, upVector, img, 0.1);


    // cout << "vp_height: " << cam.viewport_height << endl;
    // cout << "vp_width: " << cam.viewport_width << endl; 

    // World set-up!
    // ray_intersectable_list env;
    // light_list lights;

    // ###########################
    // # Input from mirror image #
    // ###########################

    // lights.add(make_shared<light>(point(0,1.1,0.5), rgb(0.75, 0.75, 0.75)));

    // material sphere_mat = material(0.1,0.9,20,rgb(0.8,0.5,0.5), rgb(1,1,1), false, 1.0, false, 1.0);
    // env.add(make_shared<sphere>(point(-0.35, -0.2, 1), 0.3, sphere_mat));

    // material sphere_mat_2 = material(0.1,0.9,20,rgb(0.8,0.5,0.8), rgb(1,1,1), false, 1.0, false, 1.0);
    // env.add(make_shared<sphere>(point(0,0.2,-1.25), 0.2, sphere_mat_2));

    // material cyl_mat = material(0.1, 0.9, 20, rgb(0.5,0.5,0.8), rgb(1,1,1), false, 1.0,false,1.0);
    // env.add(make_shared<cylinder>(point(0.3, 0, 1), vec3(0,1,0), 0.25, 0.5, cyl_mat));

    // material tri_mat_1 = material(0.1,0.9,20,rgb(0.5,0.8,0.5), rgb(1,1,1), false, 1.0, false, 1.0);
    // env.add(make_shared<triangle>(
    //     point(-1,-0.5,2),
    //     point(1,-0.5,2),
    //     point(1,-0.5,0),
    //     tri_mat_1
    // ));
    // env.add(make_shared<triangle>(
    //     point(-1,-0.5,0),
    //     point(-1,-0.5,2),
    //     point(1,-0.5,0),
    //     tri_mat_1
    // ));

    // material tri_mat_2 = material(0.1,0.9,20,rgb(0.5,0.8,0.5), rgb(1,1,1), true, 1.0, false, 1.0);


    // env.add(make_shared<triangle>(
    //     point(-1,-0.5,2),
    //     point(1,2.5,2),
    //     point(1,-0.5,2),
    //     tri_mat_2
    // ));
    // env.add(make_shared<triangle>(
    //     point(-1,-0.5,2),
    //     point(-1,2.5,2),
    //     point(1,2.5,2),
    //     tri_mat_2
    // ));


    // ################################
    // # Input from binary primitives #
    // ################################

    // env.add(make_shared<sphere>(point(-0.3,0.19,-1), 0.2));

    // vec3 cylinder_center = vec3(-0.3, -0.2, -1);
    // vec3 cylinder_axis = vec3(1,0,0);
    // double cylinder_rad = 0.15;
    // double cylinder_height = 0.4; //whole height, on input height is half
    // env.add(make_shared<cylinder>(cylinder_center, cylinder_axis, cylinder_rad, cylinder_height));
    
    // point v1 = point(0, 0, -1);
    // point v2 = point(0.5, 0, -1);
    // point v3 = point(0.25, 0.25, -1);


    // env.add(make_shared<triangle>(
    //     v1,v2,v3
    // ));

    // #############################
    // # End of  binary primitives #
    // #############################


    // #####################
    // # Input from scene  #
    // #####################

    // Lights
    // lights.add(make_shared<light>(point(0, 1.0, 0.5), vec3(0.5, 0.5, 0.5)));
    // lights.add(make_shared<light>(point(0,1.0,-0.5), vec3(0.5, 0.5, 0.5)));


    // // Objects

    // material sphere_material = material(0.1, 0.9, 10, rgb(0.5,1.0,0.5), rgb(1,1,1), false, 1.0, false, 1.0);
    // sphere sphere_1 = sphere(point(0,0.25,-1), 0.25, sphere_material);

    // env.add(make_shared<sphere>(point(0,-25.0, 0), 25.1, sphere_material));

    // material cyl_material = material(0.1, 0.9, 20, rgb(0.5,0.5,0.8), rgb(1,1,1), false, 1, false, 1);
    // env.add(make_shared<cylinder>(point(-0.3,0.19,1), vec3(0,1,0), 0.15, 0.2, cyl_material));
    // // env.add(make_shared<sphere>(point(0.40, 0.4, 0.6), 0.01, sphere_material));
    // // env.add(make_shared<sphere>(point(0.43, 0.4, 0.7), 0.01, sphere_material));
    // // env.add(make_shared<sphere>(point(0.46, 0.4, 0.8), 0.01, sphere_material));
    // // env.add(make_shared<sphere>(point(0.49, 0.4, 0.9), 0.01, sphere_material));
    // // env.add(make_shared<sphere>(point(0.52, 0.4, 1.0), 0.01, sphere_material));
    // // env.add(make_shared<sphere>(point(0.55, 0.4, 1.1), 0.01, sphere_material));
    // // env.add(make_shared<sphere>(point(0.58, 0.4, 1.2), 0.01, sphere_material));
    // // env.add(make_shared<sphere>(point(0.61, 0.4, 1.3), 0.01, sphere_material));


    // material tri_mat = material(0.3,0.9,2,rgb(0.8,0.5,0.8),rgb(1,1,1),true,1.0,false,1.0);
    // env.add(make_shared<triangle>(
    //     point(0, 0, 2.25),
    //     point(0.75, 0, 2),
    //     point(0, 0.75, 2.25),
    //     tri_mat
    // ));
    // env.add(make_shared<triangle>(
    //     point(0.75, 0.75, 2),
    //     point(0.75, 0.0, 2),
    //     point(0, 0.75, 2.25),
    //     tri_mat
    // ));
    
    // material sphere_mat_2 = material(0.1, 0.9, 20, rgb(0.8,0.5,0.5), rgb(1,1,1), false, 1 , false, 1);
    // env.add(make_shared<sphere>(point(-0.3, 0.59, 1), 0.2, sphere_mat_2));

    // material sphere_mat_3 = material(0.1, 0.9, 20, rgb(0.8, 0.5, 0.5), rgb(1,1,1), true, 1.0, false, 1.0);
    // env.add(make_shared<sphere>(point(0.3, 0.29, 1), 0.2, sphere_mat_3));

    // ##################
    // # end from scene #
    // ##################

    // Render time!!!
    // cam.render(img, env, lights);

    cout << "DONE!" << "\n";
}