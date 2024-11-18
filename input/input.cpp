#include "input.h"
#include <fstream>
#include "./json.hpp"
#include "../geometry/sphere.h"
#include "../geometry/cylinder.h"
#include "../geometry/triangle.h"

using json = nlohmann::json;

input::input(string const fp) : file_path(fp) {};

settings input::create_settings() {
    std::ifstream f(file_path);
    json data = json::parse(file_path);
    int num_bounces = data["nbounces"];
    string render_mode = data["rendermode"];
    camera cam_settings = create_cam_settings(data["camera"]);
    ray_intersectable_list env = create_env_settings(data["scene"]["shapes"]);
}

ray_intersectable_list input::create_env_settings(json scene_data) {
    ray_intersectable_list env;
    for(const auto& shape_data: scene_data) {
        string env_type = shape_data["type"];

        json material_data = shape_data["material"];
        double ks = material_data["ks"];
        double kd = material_data["kd"];
        double specularexponent = material_data["specularexponent"];
        rgb diffise_colour = rgb(material_data["diffusecolour"][0], material_data["diffusecolour"][1], material_data["diffusecolour"][2]);
        rgb specular_colour = rgb(material_data["specularcolour"][0], material_data["specularcolour"][1], material_data["specularcolour"][2]);
        bool isreflective = material_data["isreflective"];
        double reflectivity = material_data["reflectivity"];
        bool isrefractive = material_data["isrefractive"];
        double refractiveindex = material_data["refractiveindex"];

        material mat = material(ks, kd, specularexponent, diffise_colour, specular_colour, isreflective, reflectivity, isrefractive, refractiveindex);
        
        if (env_type == "sphere") {
            env.add(make_shared<sphere>(
                point(shape_data["center"][0], shape_data["center"][1], shape_data["center"][2]),
                shape_data["radius"],
                mat
            ));

        } else if (env_type == "cylinder") {
            env.add(make_shared<cylinder>(
                point(shape_data["center"][0], shape_data["center"][1], shape_data["center"][2]),
                vec3(shape_data["axis"][0], shape_data["axis"][1], shape_data["axis"][2]),
                shape_data["radius"],
                shape_data["height"],
                mat
            ));
        } else {
            env.add(make_shared<triangle>(
                point(shape_data["v1"][0], shape_data["v1"][1], shape_data["v1"][2]),
                point(shape_data["v2"][0], shape_data["v2"][1], shape_data["v2"][2]),
                point(shape_data["v3"][0], shape_data["v3"][1], shape_data["v3"][2]),
                mat
            ));
        }
    }
    return env;
}

camera input::create_cam_settings(json cam_data) {
    string type = cam_data["type"];
    int width = cam_data["width"];
    double fov = cam_data["fov"];
    vec3 lookAt = vec3(cam_data["lookAt"][0], cam_data["lookAt"][1], cam_data["lookAt"][2]);
    vec3 upVector = vec3(cam_data["upVector"][0], cam_data["upVector"][1], cam_data["upVector"][2]);
    double exposure = cam_data["exposure"];
    image img = image(16.0/9.0, 1200);

    return camera(fov, point(0,0,0), lookAt, upVector, img, exposure);
}