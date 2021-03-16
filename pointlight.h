#ifndef LIGHT_H
#define LIGHT_H
#include "rtweekend.h"
#include "ray.h"
#include "hittable.h"
class pointlight
{
public:
    pointlight(){}
    pointlight (point3 point,color a, color d, color s) : light_position(point),ambient(a), diffuse(d), specular(s){};
    vec3 get_direction(point3 hitpoint) const
    {
        return hitpoint - light_position;
    }

    bool check_blocking(point3 hitpoint, const hittable &world) const
    {
        ray r = ray(light_position,get_direction(hitpoint));
        hit_record rec;
        if (world.hit(r, 0, infinity, rec)){
           if (rec.t >= 1.0-1e-5){
               return false;
           }
           //hit other object first
           else{
               return true;
           }
        }
        //hit nothing
        return true;
    }

public:
    point3 light_position;
    color ambient;
    color diffuse;
    color specular;
};
#endif