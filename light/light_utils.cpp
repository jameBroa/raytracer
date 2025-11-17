#include "light_utils.h"
#include "../vec/vec3_utils.h"
#include <random>

using namespace vec3_utils;

namespace light_utils{

    // Keep colours in [0,1] range
    vector3 clamp_vec(vector3 in, vector3 min, vector3 max) {
        return vector3(
                std::max(min.x(), std::min(in.x(), max.x())),
                std::max(min.y(), std::min(in.y(), max.y())),
                std::max(min.z(), std::min(in.z(), max.z()))
        );
    }

    inline double clamp(double x, double min, double max) {
        return x < min ? min : (x > max ? max : x);
    }

    // From pixar paper
    void basis(vector3 n, vector3& b1, vector3& b2) {
        if(n.z() < 0) {
            double a = 1.0 / (1.0 -n.z());
            double b = n.x()*n.y() * a;
            b1 = vector3(1.0 - n.x()*n.x()*a, -b, n.x());
            b2 = vector3(b, n.y()*n.y()*a-1.0, -n.y());
        } else {
            double a = 1.0 / (1.0 + n.z());
            double b = -n.x() * n.y() * a;
            b1 = vector3(1.0 - n.x() * n.x() * a, b, -n.x());
            b2 = vector3(b, 1.0 - n.y() * n.y() * a, -n.y());
        }
    }

    // Move from local reference frames to global reference frame
    vector3 to_world(vector3 x, vector3 y, vector3 z, vector3 v) {
        return v.x()*x + v.y()*y + v.z()*z;
    }
    // Move from global reference frames to local reference frame
    vector3 to_local(vector3 x, vector3 y, vector3 z, vector3 v) {
        return vector3(dot(v, x), dot(v, y), dot(v, z));
    }

    // Rnadom number generator [0,1]
    double random_double() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }

    // Implementation of the normal distribution function
    // computes density of microfacets oriented in the direction of the halfway vector.
    // low roughness -> high density (no scatter)
    // high roughenss -> low density (more scatter)
    double NDF(double roughness, double NdH, double k) {
        double a2 = roughness * roughness;
        double term = (NdH*NdH)*(a2-1.0)+1.0;

        return a2 / (M_PI * term * term);
        // return a2 / (M_PI * std::pow(term, k));
    }

    // Calculates visibility of a microfacet to the camera and light source
    // depending on what calls it. (Both are used in GeometryTerm)
    double microfacet_visibility_cl(double NdX, double alphaG) {
        double a = alphaG * alphaG;
        double b = NdX * NdX;
        double denominator = NdX + std::sqrt(a + b - a * b);
        double result = (2.0 * NdX) / denominator;
    
        return result;
    }

    // Combines the visibility of the microfacet to the camera and light source
    // by calculating the SmithG value based on camera direction and light direction
    double microfacet_visibility(double NdL, double NdV, double roughness){
        double a2 = roughness*roughness;
        double G1 = microfacet_visibility_cl(NdV, a2); // camera direction
        double G2 = microfacet_visibility_cl(NdL, a2); // light direction
        return G1*G2;
    }


    // Returns the likelihood of sampling a specific microfacet normal through
    // importance sampling.
    // Necessary for monte-carlo type rendering
    double microfacet_likelihood(double NdH, double NdV, double roughness) {
        double NdHt = clamp(NdH, 1e-7, 0.99);
        double D = NDF(roughness, NdHt, 2.);
        double G1 = microfacet_visibility_cl(NdV, roughness*roughness);
        // Prevents division by zero
        return (D * G1) / std::max(0.00001, 4.0f * NdV);
    }

    // Mix two colours based on metallic value
    rgb mix(vector3 a, rgb albedo, double metallic) {
        return (1.0 - metallic) * a + metallic * albedo;
    }

    // Returns the reflectance of a surface based on the angle of incidence being 0.
    // Do approximation bc its computationally cheaper
    rgb schlick_colour(vector3 f0, double theta) {
        // std::cout << "theta before: " << theta << std::endl;
        double thetat = clamp(theta, 0, 1);
        // std::cout << "theta after: " << thetat << std::endl;
        // double thetat = std::max(theta, 0.0);
        return f0 + (vector3(1,1,1)-f0) * pow(1.0-thetat, 5.);
    }

    // Randomly sample point from hemisphere based on 
    // point of intersection through light scattering
    // (cosine-weighted hemisphere sampling)
    vector3 sample_hemisphere(vector3 n)
    {
        double rnd_x = random_double();
        double rnd_y = random_double();

        double a = M_PI * 2.0 * rnd_x;
        double b = 2.0 * rnd_y - 1.0;

        vector3 dir = vector3(sqrt(1.0 - b * b) * cos(a), sqrt(1.0 - b * b) * sin(a), b);
        return unit_vector(n + dir);
    }

    // Single chanel Schlick approximation for Fresnel reflectance
    // which has greater flexibility on the reflectance value based on 
    // incidence of 0 and 90.
    // Used for diffuse reflection and refraction
    double schlick_scalar(double f0, double f90, double theta) {
        double thetat = clamp(theta, 0.0, 1.0);
        double power_term = pow(1.0-thetat, 5.0);
        
        double result = f0 + (f90 - f0) * power_term;
        
        return result;
    }


    vector3 disney_specular(material mat, vector3 F, double NdH, double NdV, double NdL) {
        
        // Prevents NaN
        double NdHt = clamp(NdH, 1e-7, 1.0);
        double NdVt = clamp(NdV, 1e-7, 1.0);
        double NdLt = clamp(NdL, 1e-7, 1.0);

        double roughness = pow(mat.get_roughness(), 2.);
        double D = NDF(roughness, NdHt, 2.);
        double G = microfacet_visibility(NdLt, NdVt, std::pow(0.5+mat.get_roughness()*.5,2.));
        vector3 spec = D*F*G / (4. * NdLt * NdVt);
        
        return spec;
    }

    // Samples the microfacet normal using the NDF
    // Takes two ansiotropic roughness params (a,b) and two random numbers

    vector3 sample_microfacet(vector3 V, double a, double b, double rand1, double rand2) {
        vector3 Vh = unit_vector(vector3(a * V.x(), b * V.y(), V.z()));

        double lensq = Vh.x() * Vh.x() + Vh.y() * Vh.y();
        vector3 T1 = lensq > 0. ? vector3(-Vh.y(), Vh.x(), 0) * (1/std::sqrt(lensq)) : vector3(1, 0, 0);
        vector3 T2 = cross(Vh, T1);

        double r = sqrt(rand1);
        double phi = 2.0 * M_PI * rand2;
        double t1 = r * cos(phi);
        double t2 = r * sin(phi);
        double s = 0.5 * (1.0 + Vh.z());
        t2 = (1.0 - s) * sqrt(1.0 - t1 * t1) + s * t2;

        vector3 Nh = t1 * T1 + t2 * T2 + std::sqrt(std::max(0.0, 1.0 - t1 * t1 - t2 * t2)) * Vh;

        return unit_vector(vector3(a * Nh.x(), b * Nh.y(), std::max(0.0, Nh.z())));
    }

    vector3 disney_diffuse(material mat, double NdL, double NdV, double LdH, double roughness) {
        // Clamp values to avoid NaNs
        double LdHt = clamp(LdH, 0.0001, 1.0);
        double NdVt = clamp(NdV, 0.0001, 1.0);
        double NdLt = clamp(NdL, 0.0001, 1.0);


        // Disney diffuse equation
        double FD90 = 0.5 + 2. * roughness * pow(LdHt,2.);
        // std::cout << "FD90: " << FD90 << std::endl;
        FD90 = clamp(FD90, 0, 1);
        // std::cout << "FD90 (a): " << FD90 << std::endl;
        double a = schlick_scalar(1.,FD90, NdLt);
        double b = schlick_scalar(1.,FD90, NdVt);
        rgb res = mat.get_diffuse_colour() * (a * b / M_PI);
        // std:: cout << "res: " << res << std::endl;
        
        return res;
    }

    rgb DisneyBRDF(vector3 v, vector3 n, material mat, vector3& outdir, double& pdf) {
        double roughness = std::pow(mat.get_roughness(), 2);

        vector3 t,b;
        basis(n,t,b);
        // Uses normal vector with a basis function from pixar 
        vector3 V = to_local(t,b,n,v);
        // Assume asiotropic roughness (ax,ay) are the same
        vector3 h = sample_microfacet(V, roughness, roughness, random_double(), random_double());
        // if (h.z() < 0){
        //     h = -h;
        // }

        h = to_world(t,b,n,h);
        double metallic = mat.get_metallic();
        vector3 f0; // reflectance at normal incident
        // if(roughness == 1.0){
        //     f0 = mix(vector3(0.04,0.04,0.04), mat.get_diffuse_colour(), metallic);
        // } else {
        //     f0 = mat.get_diffuse_colour(); // For metallic objects, reflectance is base colour 
        // }

        // std::cout << "f0: " << f0 << std::endl;
        if(metallic == 1.0) {
            f0 = mat.get_diffuse_colour(); // For metallic objects, reflectance is base colour 
        } else {
            f0 = mix(vector3(0.04,0.04,0.04), mat.get_diffuse_colour(), metallic);
        }
        
        vector3 F = schlick_colour(f0, dot(v,h));
        

        // Create system for randomly calculating diffusion or specular
        // based on metallic value to simulate real random scattering of light.
        // An implementation of importance sampling
        double diffW, specW;
        // lobe weight probability
        diffW = (1.-metallic);
        // specW = dot(F, vector3(0.299,0.587,0.114));
        specW = dot(F, vector3(1,1,1));
        
        double invW = 1./(diffW + specW);
        diffW *= invW;
        specW *= invW;

        vector3 brdf = vector3(0,0,0);
        double rnd = random_double();
        // std::cout << "diffW: " << diffW << std::endl;
        // std::cout << "specW: " << specW << std::endl;
        // std::cout << "rnd: " << rnd << std::endl;
        // rnd [0,1]
        if (rnd < diffW) // diffuse calculation
        {
            // Random direction sampled from hemisphere at point of
            // intersection and assings it to variable outdir
            outdir = sample_hemisphere(n);
            h = unit_vector(outdir+v);
            
            double NdL = dot(n,outdir);
            double NdV = dot(n,v);
            double LdH = dot(outdir,h);

            // Avoid NaNs
            if ( NdL <= 0. || NdV <= 0. || LdH <= 0.) { 
                return vector3(0,0,0); 
            }
            pdf = NdL/M_PI;

            
            // Diffuse equation!
            vector3 diff = disney_diffuse(mat, NdL, NdV, LdH, roughness);
            brdf = diff * NdL;
        } 
        else // specular calculation
        {

            // sets outdir as the reflection of the view vector
            outdir = reflect(-v,h);
            vector3 h = unit_vector(outdir+v);
            double NdL = dot(n,outdir);
            double NdV = dot(n,v);
            if ( NdL <= 0. || NdV <= 0. ) {  
                return vector3(0,0,0); 
            }
            // double NdH = std::max(dot(n,h),0.0);
           double NdH = dot(n,h);

            // value from pdf function calculated based on normal, view angle, and roughness value
            // (used for final specular calculation)
            pdf = microfacet_likelihood(NdH, NdV, roughness);

            // prevents NaN errors
            if(pdf < 1e-6) {
                return vector3(0,0,0);
            }
            
            // Final specular calculation
            vector3 spec = disney_specular(mat, F, NdH, NdV, NdL);

            // Divide by 4*NdV to reduce intensity of specular highlights
            brdf = spec * NdL / (4*NdV);
            // brdf = spec * NdL;
        }
        return brdf;
    }

    vector3 reflect(vector3 incident, vector3 normal) {
        return incident - 2 * dot(incident, normal) * normal;
    }

    // refrac_dir pass by reference so we can see if it refracts and then 
    // set refraction direction at the same time.
    bool does_refract(vector3 incident, vector3 normal, double refractive_index, vector3& refrac_dir) {
       double n1 = 1.0;  // Air
        double n2 = refractive_index;  // Your material
        double eta = n1 / n2;  // This is the ratio we need
        
        double cos_i = -dot((normal), (incident));
        
        // If we're exiting the material (cos_i < 0), flip everything
        if (cos_i < 0) {
            cos_i = -cos_i;
            normal = -normal;
            // Flip the ratio since we're going from material to air
            eta = n2 / n1;
        }
        
        double sin_t2 = eta * eta * (1.0 - cos_i * cos_i);
        if (sin_t2 > 1.0) {
            return false;  // Total internal reflection
        }
        
        double cos_t = sqrt(1.0 - sin_t2);
        refrac_dir = eta * incident + (eta * cos_i - cos_t) * normal;
        refrac_dir = unit_vector(refrac_dir);
        return true;
    }

}