#include "./aabb.h"

aabb::aabb(const point a, const point b) : min(a), max(b) {}

bool aabb::intersect(const ray& r, double t_min, double t_max) const {
    // looping through all three axis
    for (int a = 0; a < 3; a++) {
        double invD = 1.0 / r.direction()[a]; // Accesses the axis direction with [a]
        double t0 = (min[a] - r.origin()[a]) * invD;
        double t1 = (max[a] - r.origin()[a]) * invD;
        if (invD < 0.0) {
            std::swap(t0, t1);
        }
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min) {
            return false;
        }
    }
    return true;
}

// Combines two bboxs into one
aabb aabb::surrounding_bbox(const aabb& box_a, const aabb& box_b) const {
    point small(
        fmin(box_a.min.x(), box_b.min.x()),
        fmin(box_a.min.y(), box_b.min.y()),
        fmin(box_a.min.z(), box_b.min.z())
    );
    point big(
        fmax(box_a.max.x(), box_b.max.x()),
        fmax(box_a.max.y(), box_b.max.y()),
        fmax(box_a.max.z(), box_b.max.z())
    );
    return aabb(small, big);
}
