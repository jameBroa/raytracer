#include "scene.h"

scene::scene(ray_intersectable_list const world_shapes, light_list const l) : shapes(world_shapes), all_lights(l) {};

