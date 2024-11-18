#ifndef RAY_INTERSECTABLE_LIST
#define RAY_INTERSECTABLE_LIST

#include "ray_intersectable.h"
#include "../material/material.h"
#include "../vec/vec3_utils.h"
#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

// Pretty much same implementation as ray_intersectable, but implements a list of them
class ray_intersectable_list : public ray_intersectable {
    public:
        std::vector<shared_ptr<ray_intersectable>> all_intersectable_objs;
        mutable std::shared_ptr<ray_intersectable> last_intersected_obj;

        // Constructors
        ray_intersectable_list();
        ray_intersectable_list(shared_ptr<ray_intersectable> object);

        // Member functions
        void clear();
        void add(shared_ptr<ray_intersectable>);
        const material& get_material() const override;
        const virtual std::string& get_name() const override;


        bool intersect(const ray& r, 
        double ray_inters_min, // min time the ray intersects
        double ray_inters_max, // max time the ray intersects
        intersect_history& history // history of ray intersections
        ) const override;
};


#endif