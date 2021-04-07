#ifndef PLANE_H
#define PLANE_H
#include "hittable.h"
#include "vec3.h"
#include <iostream>
class plane : public hittable
{
public:
    plane() {}
    plane(vec3 normal_value, double d_value,material m, double e = 1e-5) : normal(normal_value), d(d_value), mater(m),epsilon(e){}
    virtual bool hit(
        const ray &r, double t_min, double t_max, hit_record &rec) const
        override;

public:
    vec3 normal;
    double d;
    material mater;
    double epsilon;  // the epsilon for hit detection
};

bool plane::hit(const ray &r, double t_min, double t_max, hit_record &rec)
    const
{
    if (abs(dot(unit_vector(r.dir),unit_vector(normal))) < epsilon){
        return false;
    }
    else{
        auto root = (d - dot(normal,r.orig)) / dot(normal,r.dir);
        if (root < t_min || t_max < root){
            return false;
        }
        else{
            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = normal;
            rec.set_face_normal(r, outward_normal);
            rec.hit_material = mater;
            return true;
        }
    }
}
#endif