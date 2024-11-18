#ifndef RAY_H
#define RAY_H
#include "../vec/vec3.h"
#include "../vec/vec3_utils.h"

class ray {
    public:
        ray();
        ray(const point& origin, const vec3& direction);

        const point& origin() const;
        const vec3& direction() const;

        const point position_at(double t) const;

    private:
        point org;
        vec3 dir;
};

#endif