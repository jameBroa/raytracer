#ifndef SETTINGS_H
#define SETTINGS_H

#include "../common.h"
#include "../scene/scene.h"


class settings{
    public:
        settings(std::string rm, camera cam, int bounces, scene scene);
        const std::string rendermode;
        const camera scene_camera;
        const int num_bounces;
        const scene rendered_scene;
};


#endif