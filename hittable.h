#ifndef HITTABLE_H
#define HITTABLE_H
#include "ray.h"
#include "material.h"
#include "rtweekend.h"
#include "aabb.h"

//the struct which defines the neceesary parameter for hit record, if a ray hits the hittable object
struct hit_record
{
    point3 p;  //hit point
    vec3 normal;  // the surface normal of the hit point
    double t;    // the t of thr ray at which it is hit
    bool front_face;  // whether the ray hit the front or back of the surface
    material hit_material; // the matetrial of surface being hit
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

// the parent abstruct class for all the hitable primitive.
class hittable
{
public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
    virtual bool bounding_box( aabb& output_box) const = 0;
};
#endif