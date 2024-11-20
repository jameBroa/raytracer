#ifndef SETTINGS_H
#define SETTINGS_H

#include "../common.h"
#include "../scene/scene.h"
#include <string>


class settings{
    public:
        settings(std::string rm, camera cam, image i, int bounces, scene scene);
         std::string rendermode;
         camera scene_camera;
         int num_bounces;
         scene world_objects;
         image img;

        void start(std::string filename);

};


#endif