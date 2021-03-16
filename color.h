#ifndef COLOR_H
#define COLOR_H
#include "vec3.h"
#include "rtweekend.h"
#include <iostream>
//PPM file wirter which wirte color to each pixel
void write_color(std::ostream &out, color pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
#endif