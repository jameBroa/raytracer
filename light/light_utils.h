#ifndef LIGHT_UTILS
#define LIGHT_UTILS
#include "../vec/vector3.h";
#include "../material/material.h"

namespace light_utils{
    vector3 reflect(vector3 incident, vector3 normal);
    bool does_refract(vector3 incident, vector3 normal, double eta, vector3& refrac_dir);
    vector3 sample_microfacet(vector3 V, double ax, double ay, double r1, double r2);
    vector3 to_local(vector3 x, vector3 y, vector3 z, vector3 v);
    vector3 to_world(vector3 x, vector3 y, vector3 z, vector3 v);
    rgb mix(vector3 a, rgb albedo, double metallic);
    rgb schlick_colour(vector3 f0, double theta);
    double schlick_scalar(double f0, double f90, double theta);
    double microfacet_likelihood(double NoH, double NoV, double roughness);
    vector3 sample_hemisphere(vector3 n);
    vector3 disney_diffuse(material mat, double NdL, double NdV, double LdH, double roughness);
    vector3 disney_specular(material mat, vector3 F, double NdH, double NdV, double NdL);

    double NDF(double roughness, double NoH, double k);
    double microfacet_visibility_cl(double NDotV, double alphaG);
    vector3 clamp_vec(vector3 in, vector3 min, vector3 max);

    double microfacet_visibility(double NdL, double NdV, double roughness);

    rgb DisneyBRDF(vector3 v, vector3 n, material mat, vector3& outdir, double& pdf);
    void basis(vector3 n, vector3& b1, vector3& b2);
}

#endif