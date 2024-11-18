#ifndef COLOUR_H
#define COLOUR_H

#include "../common.h"

using rgb = vec3;
namespace colour_utils {
    void output_colour(std::ostream& out, const rgb& colours);
}

#endif
