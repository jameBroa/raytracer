#ifndef MATERIAL_H
#define MATERIAL_H
#include "../vec/vec3.h"
#include "../texture/texture.h"
#include <memory>

class material {
    public:
    
        material();
        material(double ks, double kd, double a, rgb dc, rgb sc, bool isrefl, double r, bool isrefr, double ri,
        bool hast, std::shared_ptr<texture> t);

        double get_ks() const { return k_s; }
        double get_kd() const { return k_d; }
        double get_alpha() const { return alpha; }
        rgb get_diffuse_colour() const { return diffuse_colour; }
        rgb get_specular_colour() const { return specular_colour; }
        bool get_isreflective() const { return isreflective; }
        double get_reflectivity() const { return reflectivity; }
        bool get_isrefractive() const { return isrefractive; }
        double get_refractiveindex() const { return refractiveindex; }
        bool get_hastexture() const { return hastexture; }

        std::shared_ptr<texture> get_texture() const { return txt; }
        
        material(const material& other);

        
        material& operator=(const material& other) {
        if (this != &other) {
            // Copy non-const members
            k_s = other.k_s;
            (k_d) = other.k_d;
            (alpha) = other.alpha;
            (diffuse_colour) = other.diffuse_colour;
            (specular_colour) = other.specular_colour;
            (isreflective) = other.isreflective;
            (reflectivity) = other.reflectivity;
            (isrefractive) = other.isrefractive;
            (refractiveindex) = other.refractiveindex;
            (txt) = other.txt;
        }
        return *this;
    }


    private: // Private to ensure immutability
         double k_s;
         double k_d;
         double alpha;
         rgb diffuse_colour;
         rgb specular_colour;
         bool isreflective;
         double reflectivity;
         bool isrefractive;
         double refractiveindex;
        bool hastexture;
        std::shared_ptr<texture> txt;
};

#endif
