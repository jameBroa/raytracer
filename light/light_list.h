#ifndef LIGHT_LIST_H
#define LIGHT_LIST_H
#include "light.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class light_list {
    public:
        std::vector<shared_ptr<light>> all_lights;

        light_list(shared_ptr<light> lights);
        light_list();

        void clear();
        void add(shared_ptr<light>);

        rgb calculate_lighting(
            const intersect_history& history, 
            const ray_intersectable& world, 
            const ray& r,
            int num_bounces
            ) const;

        rgb pathtrace(const intersect_history& history, 
const ray& r, const ray_intersectable& world, int num_bounces);


};



#endif