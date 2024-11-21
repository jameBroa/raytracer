#include "ray_intersectable_list.h"
#include "../vec/vec3_utils.h"
using namespace vec3_utils;
ray_intersectable_list::ray_intersectable_list() {

}

ray_intersectable_list::ray_intersectable_list(shared_ptr<ray_intersectable> intersectable_obj) {
    add(intersectable_obj);
}

void ray_intersectable_list::clear(){
    all_intersectable_objs.clear();
}

void ray_intersectable_list::add(shared_ptr<ray_intersectable> intersectable_obj) {
    all_intersectable_objs.push_back(intersectable_obj);
}

bool ray_intersectable_list::bounding_box(aabb& bbox) const {
    // Not implemented (no need to)
}


const material& ray_intersectable_list::get_material() const {

    if(last_intersected_obj) {
                
        return last_intersected_obj->get_material();
    } else {
        material default_material = material();
        return default_material;
    }
}

const std::string& ray_intersectable_list::get_name() const {

    if(last_intersected_obj) {
        return last_intersected_obj->get_name();
    } else {
        return "no object intersected";
    }
}

void ray_intersectable_list::get_uv(const point& p, double& u, double& v) const {
     if(last_intersected_obj) {
        last_intersected_obj->get_uv(p, u, v);
    } 
};

bool ray_intersectable_list::intersect(
    const ray& r, double ray_inters_min, double ray_inters_max, intersect_history& history 
    
    ) const {
    intersect_history temp_history;
    bool did_hit = false;
    double closest = ray_inters_max;

    for(const shared_ptr<ray_intersectable>& intersectable_obj : all_intersectable_objs) {
        if(intersectable_obj->intersect(r, ray_inters_min, closest, temp_history) == true) {
            closest = temp_history.time;
            did_hit = true;
            history = temp_history;
        }

    }
    return did_hit;
}