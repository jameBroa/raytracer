#ifndef SCENE_H
#define SCENE_H


#include "../ray/ray_intersectable_list.h"
#include "../light/light_list.h"

class scene{
    public:
        scene(ray_intersectable_list const world_shapes,
        light_list const l
        );

        ray_intersectable_list const shapes;
        light_list const all_lights;

        

};

#endif