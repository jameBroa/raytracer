#ifndef INPUT_H
#define INPUT_H

#include <string>
#include "../settings/settings.h"
#include "../camera/camera.h"
#include "./json.hpp"
#include "../ray/ray_intersectable_list.h"
#include "../light/light_list.h"  

using namespace std;
using json = nlohmann::json;

class input{
    public:
        input();
        input(string const fp);

        string const file_path;

        settings create_settings();

        camera create_cam_settings(json cam_data, rgb bgcol, std::string rm, int num_bounces);
        ray_intersectable_list create_env_settings(json scene_data);
        light_list create_light_settings(json light_data);
};


#endif