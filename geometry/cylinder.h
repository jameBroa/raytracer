#ifndef CYLINDER_H
#define CYLINDER_H

#include "../ray/ray_intersectable.h"
#include "../vec/vec3.h"
#include "../material/material.h"
#include <string>

class cylinder : public ray_intersectable {
    public: 
        cylinder(const point center, const vec3 axis, 
        const double radius, const double height, const material& mat);

        const material& get_material() const override;

        const std::string& get_name() const override;
        void get_uv(const point& p, double& u, double& v) const override;
        bool bounding_box(aabb& bbox) const override;

        bool intersect(const ray&r, 
        double ray_inters_min, 
        double ray_inters_max,
        intersect_history& history
        ) const;

        bool intersect_plane(const ray& r, 
        const point plane_point, 
        const vec3 plane_normal, 
        double& t,
        double min,
        double max
        ) const;

    private:
        point center;
        vec3 axis;
        double radius;
        double height;
        material material_settings;
};

#endif