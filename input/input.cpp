#include "input.h"
#include <fstream>
#include "./json.hpp"
#include "../geometry/sphere.h"
#include "../geometry/cylinder.h"
#include "../geometry/triangle.h"
#include "../vec/vec3_utils.h"
#include <string>

using namespace vec3_utils;
using json = nlohmann::json;

input::input(std::string const fp) : file_path(fp) {};

settings input::create_settings() {
    std::ifstream f(file_path);
    json data;
    f >> data;
    int num_bounces = data.value("nbounces", 1);
    std::string render_mode = data["rendermode"];
    camera cam_settings = create_cam_settings(data["camera"], 
    rgb(data["scene"]["backgroundcolor"][0], data["scene"]["backgroundcolor"][1],data["scene"]["backgroundcolor"][2]), 
    render_mode, 
    num_bounces);
    image img = image(data["camera"]["width"], data["camera"]["height"]);

    ray_intersectable_list env = create_env_settings(data["scene"]["shapes"]);
    light_list lights = create_light_settings(data["scene"]["lightsources"]);
    scene input_scene = scene(env, lights);
    return settings(render_mode, cam_settings, img, num_bounces, input_scene);
}

light_list input::create_light_settings(json light_data){
    light_list lights;
    for(const auto& light_data: light_data) {
        lights.add(make_shared<light>(
            point(light_data["position"][0], light_data["position"][1], light_data["position"][2]),
            vector3(light_data["intensity"][0], light_data["intensity"][1], light_data["intensity"][2])
        ));
    }
    return lights;
}

ray_intersectable_list input::create_env_settings(json scene_data) {
    ray_intersectable_list env;
    for(const auto& shape_data: scene_data) {

        std::string env_type = shape_data["type"];

        json material_data = shape_data.value("material", json::object());
        material mat;


        double ks, kd, specularexponent, roughness, metallic, emissive, reflectivity, refractiveindex;
        bool isreflective, isrefractive;
        rgb diffise_colour, specular_colour;


        if(material_data.empty()) {
            ks = 0.1;
            kd = 0.1;
            specularexponent = 10;
            diffise_colour = rgb(1.0,0,0);
            specular_colour = rgb(1,1,1);
            isreflective = false;
            reflectivity = 1.0;
            isrefractive = false;
            refractiveindex =1.0;
            roughness = 0.5;
            metallic = 0.5;
            emissive = 0.0;
        } else {
            ks = material_data["ks"];
            kd = material_data["kd"];
            specularexponent = material_data["specularexponent"];
            diffise_colour = rgb(material_data["diffusecolor"][0], material_data["diffusecolor"][1], material_data["diffusecolor"][2]);
            specular_colour = rgb(material_data["specularcolor"][0], material_data["specularcolor"][1], material_data["specularcolor"][2]);
            isreflective = material_data["isreflective"];
            reflectivity = material_data["reflectivity"];
            isrefractive = material_data["isrefractive"];
            refractiveindex = material_data["refractiveindex"];
            roughness = material_data.value("roughness", 0.9);
            metallic = material_data.value("metallic", 0.1);
            emissive = material_data.value("emissive", 0.0);
        }
        if(shape_data.contains("hastexture")) {
            if(shape_data["hastexture"] == true) {
            std::string txt_path = shape_data["texturepath"];
            mat = material(ks, kd, specularexponent, diffise_colour, specular_colour, isreflective, reflectivity, isrefractive, refractiveindex,
            true, std::make_shared<texture>(txt_path), roughness, metallic, emissive
            );
            } else {
                mat = material(ks, kd, specularexponent, diffise_colour, specular_colour, isreflective, reflectivity, isrefractive, refractiveindex,
                false, std::make_shared<texture>("\0"), roughness, metallic, emissive);
            }
        } else {
            mat = material(ks, kd, specularexponent, diffise_colour, specular_colour, isreflective, reflectivity, isrefractive, refractiveindex,
            false, std::make_shared<texture>("\0"), roughness, metallic, emissive);
        }

        if (env_type == "sphere") {
            env.add(make_shared<sphere>(
                point(shape_data["center"][0], shape_data["center"][1], shape_data["center"][2]),
                shape_data["radius"],
                mat
            ));
        } else if (env_type == "cylinder") {
            env.add(make_shared<cylinder>(
                point(shape_data["center"][0], shape_data["center"][1], shape_data["center"][2]),
                vector3(shape_data["axis"][0], shape_data["axis"][1], shape_data["axis"][2]),
                shape_data["radius"],
                shape_data["height"],
                mat
            ));
        } else if (env_type == "triangle") {
            env.add(make_shared<triangle>(
                point(shape_data["v0"][0], shape_data["v0"][1], shape_data["v0"][2]),
                point(shape_data["v1"][0], shape_data["v1"][1], shape_data["v1"][2]),
                point(shape_data["v2"][0], shape_data["v2"][1], shape_data["v2"][2]),
                mat
            ));
        }
    }
    return env;
}

camera input::create_cam_settings(json cam_data, rgb bgcol, std::string rm, int num_bounces) {
    std::string type = cam_data["type"];
    int width = cam_data["width"];
    double fov = cam_data["fov"];
    vector3 lookAt = vector3(cam_data["lookAt"][0], cam_data["lookAt"][1], cam_data["lookAt"][2]);
    vector3 upVector = vector3(cam_data["upVector"][0], cam_data["upVector"][1], cam_data["upVector"][2]);
    double exposure = cam_data["exposure"];
    image img = image(cam_data["width"], cam_data["height"]);
    point position = point(cam_data["position"][0], cam_data["position"][1], cam_data["position"][2]);
    // double lensradius = cam_data["lensradius"];
    double lensradius = cam_data.value("lensradius", 0.0);
    double focus_dist = cam_data.value("focusdist", 0.0);
    std::cout << "focus_dist: " << focus_dist << std::endl;
    std::cout << "lensradius: " << lensradius << std::endl;
    double num_samples = cam_data.value("nsamples", 32);
    return camera(fov, position, lookAt, upVector, img, exposure, rm, num_bounces, bgcol, lensradius, focus_dist, num_samples);
}