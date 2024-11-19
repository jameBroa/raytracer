#include "settings.h"


settings::settings(std::string rm, camera cam, image i, int bounces, scene scene) : 
rendermode(rm), scene_camera(cam), img(i), num_bounces(bounces), world_objects(scene) {};

void settings::start() {
    scene_camera.render(img, world_objects.shapes, world_objects.all_lights);
}