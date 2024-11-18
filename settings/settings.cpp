#include "settings.h"


settings::settings(std::string rm, camera cam, int bounces, scene scene) : 
rendermode(rm), scene_camera(cam), num_bounces(bounces), rendered_scene(scene) {};
