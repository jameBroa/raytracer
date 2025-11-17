#include "settings.h"
#include "../vec/vec3_utils.h"

using namespace vec3_utils;

settings::settings(std::string rm, camera cam, image i, int bounces, scene scene) : 
rendermode(rm), scene_camera(cam), img(i), num_bounces(bounces), world_objects(scene) {};

void settings::start(std::string filename) {
    std::cout << "(settings )look at: " << scene_camera.lookAt << std::endl;
    scene_camera.render(img, world_objects.shapes, world_objects.all_lights, filename);
}