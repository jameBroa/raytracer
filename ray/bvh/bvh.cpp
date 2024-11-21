#include "bvh.h"
#include <algorithm>
bvh_node::bvh_node(const std::vector<std::shared_ptr<ray_intersectable>>& objects, size_t start, size_t end) {
    auto objects_copy = objects;

    // Use custom defined comparators so BVH splits on all axis 
    // Splitting axis will be done randomly
    int axis = rand() % 3;
    auto comparator = [this, axis](const std::shared_ptr<ray_intersectable>& a, const std::shared_ptr<ray_intersectable>& b) {
        return 
            (axis == 0) ? this->box_x_compare(a,b) 
            : (axis == 1) ? this->box_y_compare(a,b) 
            : this->box_z_compare(a,b);
    };
    size_t num_obj = end - start;
    if(num_obj == 1) {
        // base case #1:

        left=right=objects_copy[start];

    } else if (num_obj == 2) {
        // base case #2:
        if(comparator(objects_copy[start], objects_copy[start+1])) {
            left = objects_copy[start];
            right = objects_copy[start+1];
        } else {
            left = objects_copy[start+1];
            right = objects_copy[start];
        }

    } else {
        // Recursive call to split the objects

        std::sort(objects_copy.begin() + start, objects_copy.begin() + end, comparator);
        double mid = start + num_obj / 2;
        left = std::make_shared<bvh_node>(objects_copy, start, mid);
        right = std::make_shared<bvh_node>(objects_copy, mid, end);

    }

    //Create bounding box for this node

    aabb box_l, box_r;
    if (!left->bounding_box(box_l) || !right->bounding_box(box_r)) {
        std::cerr << "No bounding box in bvh_node constructor" << std::endl;
    }

    bbx = box_l.surrounding_bbox(box_l, box_r);
}


bool bvh_node::box_x_compare(const std::shared_ptr<ray_intersectable>& a, const std::shared_ptr<ray_intersectable>& b){
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(box_a) || !b->bounding_box(box_b)) {
        std::cerr << "No bounding box in bvh_node constructor" << std::endl;
        return false;
    }
    return box_a.min.x() < box_b.min.x();
}

bool bvh_node::box_y_compare(const std::shared_ptr<ray_intersectable>& a, const std::shared_ptr<ray_intersectable>& b){
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(box_a) || !b->bounding_box(box_b)) {
        std::cerr << "No bounding box in bvh_node constructor" << std::endl;
        return false;
    }
    return box_a.min.y() < box_b.min.y();
}
bool bvh_node::box_z_compare(const std::shared_ptr<ray_intersectable>& a, const std::shared_ptr<ray_intersectable>& b){
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(box_a) || !b->bounding_box(box_b)) {
        std::cerr << "No bounding box in bvh_node constructor" << std::endl;
        return false;
    }
    return box_a.min.z() < box_b.min.z();
}

bool bvh_node::intersect(const ray& r, double t_min, double t_max, intersect_history& history) const {
    if(!bbx.intersect(r, t_min, t_max)) {
        return false;
    }

    bool hit_left = left->intersect(r, t_min, t_max, history);
    bool hit_right = right->intersect(r, t_min, hit_left ? history.time : t_max, history);

    return hit_left || hit_right;
}

bool bvh_node::bounding_box(aabb& bbox) const {
    bbox = bbx;
    return true;
}

// const material& bvh_node::get_material() const {
//     return material();
// }
// const std::string& bvh_node::get_name() const {
//     return "\0";
// }



