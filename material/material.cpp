#include "material.h"
#include <memory>


material::material() : 
k_s(0), 
k_d(0), 
alpha(0), 
diffuse_colour(rgb(0,0,0)), 
specular_colour(rgb(0,0,0)), 
isreflective(false),
reflectivity(0),
isrefractive(false),
refractiveindex(0),
hastexture(false),
txt(std::make_shared<texture>("\0")) {
};

material::material(double ks, double kd, double a, rgb dc, rgb sc, bool isrefl, double r, bool isrefr, double ri, bool hast, std::shared_ptr<texture> t) : 
    k_s(ks), 
    k_d(kd), 
    alpha(a), 
    diffuse_colour(dc), 
    specular_colour(sc), 
    isreflective(isrefl), 
    reflectivity(r),
    isrefractive(isrefr), 
    refractiveindex(ri),
    hastexture(hast),
    txt(t)
     {};


// Copy constructor
material::material(const material& other) 
    : k_s(other.k_s), 
      k_d(other.k_d), 
      alpha(other.alpha), 
      diffuse_colour(other.diffuse_colour), 
      specular_colour(other.specular_colour), 
      isreflective(other.isreflective), 
      reflectivity(other.reflectivity),
      isrefractive(other.isrefractive), 
      refractiveindex(other.refractiveindex),
      hastexture(other.hastexture),
      txt(other.txt) // Use the copy constructor of shared_ptr
{
}