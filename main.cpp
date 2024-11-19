#include <fstream>
#include "./ray/ray_intersectable.h"
#include "./ray/ray_intersectable_list.h"
#include "./geometry/sphere.h"
#include "./geometry/cylinder.h"
#include "./geometry/triangle.h"
#include "./common.h"
#include "./light/light.h"
#include "./light/light_list.h"
#include "./input/input.h"
#include <iostream>

using namespace std;
using namespace vec3_utils;
using point = vec3;

int main() 
{
    input in = input("./multiple_reflect_balls.json");
    settings s = in.create_settings();
    s.start();
    std::cout << "FINISHED RENDER!" << std::endl;
}