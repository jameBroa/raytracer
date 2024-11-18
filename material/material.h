#ifndef MATERIAL_H
#define MATERIAL_H
#include "../vec/vec3.h"

class material {
    public:
    
        material();
        material(double ks, double kd, double a, rgb dc, rgb sc, bool isrefl, double r, bool isrefr, double ri);

        double get_ks() const { return k_s; }
        double get_kd() const { return k_d; }
        double get_alpha() const { return alpha; }
        rgb get_diffuse_colour() const { return diffuse_colour; }
        rgb get_specular_colour() const { return specular_colour; }
        bool get_isreflective() const { return isreflective; }
        double get_reflectivity() const { return reflectivity; }
        bool get_isrefractive() const { return isrefractive; }
        double get_refractiveindex() const { return refractiveindex; }
        
        material& operator=(const material& other) {
        if (this != &other) {
            // Copy non-const members
            const_cast<double&>(k_s) = other.k_s;
            const_cast<double&>(k_d) = other.k_d;
            const_cast<double&>(alpha) = other.alpha;
            const_cast<rgb&>(diffuse_colour) = other.diffuse_colour;
            const_cast<rgb&>(specular_colour) = other.specular_colour;
            const_cast<bool&>(isreflective) = other.isreflective;
            const_cast<double&>(reflectivity) = other.reflectivity;
            const_cast<bool&>(isrefractive) = other.isrefractive;
            const_cast<double&>(refractiveindex) = other.refractiveindex;
        }
        return *this;
    }


    private: // Private to ensure immutability
        const double k_s;
        const double k_d;
        const double alpha;
        const rgb diffuse_colour;
        const rgb specular_colour;
        const bool isreflective;
        const double reflectivity;
        const bool isrefractive;
        const double refractiveindex;
};

#endif
