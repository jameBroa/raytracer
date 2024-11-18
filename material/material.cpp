#include "material.h"


material::material() : 
k_s(0), 
k_d(0), 
alpha(0), 
diffuse_colour(rgb(0,0,0)), 
specular_colour(rgb(0,0,0)), 
isreflective(false),
reflectivity(0),
isrefractive(false),
refractiveindex(0) {};

material::material(double ks, double kd, double a, rgb dc, rgb sc, bool isrefl, double r, bool isrefr, double ri) : 
    k_s(ks), 
    k_d(kd), 
    alpha(a), 
    diffuse_colour(dc), 
    specular_colour(sc), 
    isreflective(isrefl), 
    reflectivity(r),
    isrefractive(isrefr), 
    refractiveindex(ri) {};