#include "colour.h"
#include "../vec/vec3.h"

#include <iostream>


using rgb = vec3;
namespace colour_utils {
    // we expect each x, y, z in colours to be [0, 1]
    void output_colour(std::ostream& out, const rgb& colours) {
        double red = colours.x();
        double green = colours.y();
        double blue = colours.z();

        // convert from [0,1] -> [0,255] for PPM output
        double redConvert = int(255.9999 * red);
        double greenConvert = int(255.9999 * green);
        double blueConvert = int(255.9999 * blue);

        out << redConvert << ' ' << greenConvert << ' ' << blueConvert << '\n';
    }
}