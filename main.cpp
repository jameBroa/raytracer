#include <fstream>
#include "./ray/ray_intersectable.h"
#include "./ray/ray_intersectable_list.h"
#include "./geometry/sphere.h"
#include "./geometry/cylinder.h"
#include "./geometry/triangle.h"
#include "./light/light.h"
#include "./light/light_list.h"
#include "./input/input.h"
#include "./texture/texture.h"
#include <iostream>
#include <filesystem>
#include <string>

using namespace std;
using namespace vec3_utils;
using point = vector3;

int main(int argc, char* argv[]){

    if(argc < 2) {
        std::cout << "Please provide a file path to a json file" << std::endl;
        return 1;
    }

    std::string input_path = argv[1];

    // Gets filename from input file
    size_t last_slash = input_path.find_last_of("/\\");
    std::string filename = input_path.substr(last_slash + 1);
    size_t last_dot = filename.find_last_of(".");
    if (last_dot != std::string::npos) {
        filename = filename.substr(0, last_dot);
    }

    std::cout << "STARTING RENDER" << std::endl;
    input in = input(input_path);
    settings s = in.create_settings();
    s.start(filename);
    std::cout << "FINISHED RENDER!" << std::endl;
}