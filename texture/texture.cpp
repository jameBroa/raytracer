#include "texture.h"
#include <fstream>
#include <sstream>
#include <iostream>
texture::texture(const std::string filename) {
    if(filename == "\0") {
        dirname = "\0";
        // No texture passed
        return;
    }

    dirname = filename;
    load_ppm(filename);
}

vec3 texture::sample(float u, float v) {
    u = u - std::floor(u);
    v = v - std::floor(v);

    // Avoid float-to-int conversion in tight loops
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);
    
    return data[y * width + x];
}

void texture::load_ppm(const std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    if(!file) {
        std::cerr << "Could not open file " << filename << std::endl;
        return;
    }
    std::string header;
    file >> header;
    if(header != "P3" && header != "P6") {
        std::cerr << "Invalid PPM file: '" << filename << "'" << std::endl;
        return;
    }

    file >> width >> height;
    int max;
    file >> max;
    file.ignore(255, '\n');

    data.resize(width * height);
    
    if(header == "P3") {
        for(int i = 0; i < width * height; i++) {
            int r, g, b;
            file >> r >> g >> b;
            data[i] = vec3(r/255.0, g/255.0, b/255.0);
        }
    } else if (header == "P6") {
        for (int i = 0; i < width * height; i++) {
            unsigned char r, g, b;
            file.read(reinterpret_cast<char*>(&r), 1);
            file.read(reinterpret_cast<char*>(&g), 1);
            file.read(reinterpret_cast<char*>(&b), 1);
            data[i] = vec3(r / 255.0, g / 255.0, b / 255.0);
        }
    }
    if(!file) {
        std::cerr << "Error reading file " << filename << std::endl;
    }

}

