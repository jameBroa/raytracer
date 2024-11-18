#ifndef SPHERE_H
#define SPHERE_H

#include "../ray/ray_intersectable.h"
#include "../vec/vec3.h"
#include <string>

class sphere : public ray_intersectable {
    public:
        sphere(const point center, double radius, const material& mat);

        bool intersect(const ray&r, 
        double ray_inters_min, 
        double ray_inters_max,
        intersect_history& history
        ) const;

        const material& get_material() const override;
        const std::string& get_name() const override;
   
    private:
        point center;
        double radius; 
        material material_settings;
};

#endif