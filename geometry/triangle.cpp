#include "triangle.h"
#include "common_math.h"
#include "../vec/vec3_utils.h"
#include <cmath>

using namespace vec3_utils;


triangle::triangle(const point a, const point b, const point c, const material& mat) 
: a(a), b(b), c(c), material_settings(mat) {};

const material& triangle::get_material() const {
    return material_settings;
}

const std::string& triangle::get_name() const{
    return "triangle";
}



bool triangle::intersect(const ray&r, 
        double ray_inters_min, 
        double ray_inters_max,
        intersect_history& history
        ) const {
            vec3 ab = b - a;
            vec3 ac = c - a;
            vec3 norm = unit_vector(cross(ab, ac)); // normal to the triangle, pointing outwards
            double denom = dot(norm, r.direction());

            if (std::abs(denom) < 0.000001) { // ray is parallel to the triangle
                return false;
            }

            double t = dot(norm, a - r.origin()) / denom;
            if (t < ray_inters_min || t > ray_inters_max) {
                return false;
            }

            point p = r.position_at(t); //point of intersection
            vec3 ap = p - a;
            vec3 bp = p - b;
            vec3 cp = p - c;

            double ang_ab = acos(dot(ap,bp) / (ap.length() * bp.length()));
            double ang_bc = acos(dot(bp,cp) / (bp.length() * cp.length()));
            double ang_ca = acos(dot(cp,ap) / (cp.length() * ap.length()));

            vec3 v0 = b - a;
            vec3 v1 = c - a;
            vec3 v2 = p - a;

            double dot00 = dot(v0, v0);
            double dot01 = dot(v0, v1);
            double dot02 = dot(v0, v2);
            double dot11 = dot(v1, v1);
            double dot12 = dot(v1, v2);

            double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
            double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
            double v = (dot00 * dot12 - dot01 * dot02) * invDenom;
            
            const double EPSILON = 1e-8;
            if ((u >= -EPSILON) && (v >= -EPSILON) && (u + v <= 1+EPSILON)) {
                history.time = t;
                history.point = p;
                history.set_face_of_norm(r, norm);
                history.set_material(material_settings);
                return true;
            }
            return false;

            // double ang_ab = atan2(ap.length() * bp.length(), dot(ap, bp));
            // double ang_bc = atan2(bp.length() * cp.length(), dot(bp, cp));
            // double ang_ca = atan2(cp.length() * ap.length(), dot(cp, ap));

            // std::cout << "ang_ab: " << ang_ab << " ang_bc: " << ang_bc << " ang_ca: " << ang_ca << "\n";

            if (ang_ab + ang_bc + ang_ca != 2 * pi) {
                return false;
            }

            
            return true;
            
        }

