#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../ray/ray_intersectable.h"
#include "../vec/vector3.h"
#include "../material/material.h"

class triangle : public ray_intersectable {
    public:
        triangle(const point a, const point b, const point c, const material& mat);

        const material& get_material() const override;
        const std::string& get_name() const override;
        void get_uv(const point& p, double& u, double& v) const override;
        bool bounding_box(aabb& bbox) const override;

        bool intersect(const ray&r, 
        double ray_inters_min, 
        double ray_inters_max,
        intersect_history& history
        ) const;

    private:
        point a;
        point b;
        point c;
        material material_settings;
};

#endif