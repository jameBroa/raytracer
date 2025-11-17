#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include "../vec/vector3.h"

class texture {
    public:
        texture(const std::string filename);
        vector3 sample(float u, float v);
        void load_ppm(const std::string filename);
        std::string dirname;
        int width;
        int height;
        std::vector<vector3> data;
};

#endif