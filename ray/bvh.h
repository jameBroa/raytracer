#ifndef BVH_H
#define BVH_H
#include "ray_intersectable.h"

class bvh_node : public ray_intersectable {
    public:
        bvh_node();
        bvh_node(const std::vector<std::shared_ptr<ray_intersectable>>& objects, size_t start, size_t end);

        virtual bool intersect(const ray& r, double t_min, double t_max, intersect_history& history) const override;
        virtual const material& get_material() const override { /* Implement if needed */ }
        virtual const std::string& get_name() const override { /* Implement if needed */ }
        virtual void get_uv(const point& p, double& u, double& v) const override { /* Implement if needed */ }

    
};

#endif