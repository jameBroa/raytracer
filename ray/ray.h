#ifndef RAY_H
#define RAY_H
#include "../vec/vector3.h"
#include "../vec/vec3_utils.h"

class ray {
    public:
        ray();
        ray(const point& origin, const vector3& direction);

        const point& origin() const;
        const vector3& direction() const;

        const point position_at(double t) const;

    private:
        point org;
        vector3 dir;
};

#endif