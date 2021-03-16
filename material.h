#ifndef MATERIAL_H
#define MATERIAL_H
#include "vec3.h"

// this class define the material for the primitve hittable object, with its ambient,diffuse,specular,
//shinesss parameters
class material
{
public:
    material(){}
    material(color a, color d, color s,float sh): ambient(a), diffuse(d), specular(s), shininess(sh){}


public:
    color ambient;
    color diffuse;
    color specular;
    float shininess;
};

#endif