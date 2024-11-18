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

const material& ray_intersectable_list::get_material() const {

    if(last_intersected_obj) {
                
        // std::cout << "getting vals" << std::endl;
        // std::cout << last_intersected_obj->get_material().get_diffuse_colour() << std::endl;

        return last_intersected_obj->get_material();
    } else {
        // If no object has been intersected, return a default material
        material default_material = material();
        return default_material;
    }

    // const material& mat = material();
    // return mat;
}

const std::string& ray_intersectable_list::get_name() const {

    if(last_intersected_obj) {
        return last_intersected_obj->get_name();
    } else {
        // If no object has been intersected, return no name
        return "no object intersected";
    }

    // const material& mat = material();
    // return mat;
}


bool ray_intersectable_list::intersect(
    const ray& r, double ray_inters_min, double ray_inters_max, intersect_history& history 
    
    ) const {
    intersect_history temp_history;
    bool did_hit = false;
    double closest = ray_inters_max;

    for(const shared_ptr<ray_intersectable>& intersectable_obj : all_intersectable_objs) {
        if(intersectable_obj->intersect(r, ray_inters_min, closest, temp_history) == true) {
            // std::cout << "in ray_intersectable_list::intersect()" << std::endl;
            
            // if(temp_history.time < closest) {
                closest = temp_history.time;
                did_hit = true;
                history = temp_history;
                // last_intersected_obj = intersectable_obj;
                // std::cout << "history diff color: " << history.mat.get_diffuse_colour() << std::endl;

            
            // return true; // When uncommented, works for reflection scene
        }

    }
    return did_hit;
}