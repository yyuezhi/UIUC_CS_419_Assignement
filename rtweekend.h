#ifndef RTWEEKEND_H
#define RTWEEKEND_H
#include <cmath>
#include <limits>
#include <memory>
#include <random>

// Usings
using std::make_shared;
using std::shared_ptr;
using std::sqrt;
// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
// Utility Functions

//convert degree to radians
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

// Returns a random real in [0,1).
inline double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}

//generate a random int between [min,max]
inline int random_int(int min,int max)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(min,max);
    auto random_integer = uni(rng);
    return random_integer;
}

// Returns a random real in [min,max).
inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

//clamp the output of x to range(min,max)
inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

#endif