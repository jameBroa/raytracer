#ifndef BVH_H
#define BVH_H
#include "../ray_intersectable.h"
#include "../aabb/aabb.h"

class bvh_node : public ray_intersectable {
    public:
        aabb bbx;
        bvh_node() = default;
        bvh_node(const std::vector<std::shared_ptr<ray_intersectable>>& objects, size_t start, size_t end);

        // Children nodes
        std::shared_ptr<ray_intersectable> left;
        std::shared_ptr<ray_intersectable> right;

        //Comparators
        bool box_x_compare(const std::shared_ptr<ray_intersectable>& a, const std::shared_ptr<ray_intersectable>& b);
        bool box_y_compare(const std::shared_ptr<ray_intersectable>& a, const std::shared_ptr<ray_intersectable>& b);
        bool box_z_compare(const std::shared_ptr<ray_intersectable>& a, const std::shared_ptr<ray_intersectable>& b);

        bool intersect(const ray& r, double t_min, double t_max, intersect_history& history) const override;
        const material& get_material() const override { /* Implement if needed */ }
        const std::string& get_name() const override { /* Implement if needed */ }
        void get_uv(const point& p, double& u, double& v) const override {  }
        bool bounding_box(aabb& bbox) const override; // Axis aligned bounding box

    
};

#endif