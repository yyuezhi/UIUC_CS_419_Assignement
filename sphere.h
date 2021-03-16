#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"
#include "vec3.h"
//class structure for sphere
class sphere : public hittable
{
public:
    sphere() {}
    sphere(point3 cen, double r,material m) : center(cen), radius(r), mater(m){}
    virtual bool hit(
        const ray &r, double t_min, double t_max, hit_record &rec) const
        override;

    virtual bool bounding_box( aabb& output_box) const override;

public:
    point3 center;
    double radius;
    material mater;
};

//this functino detect whether the sphere is being hit by ray r with t in range(t_min,t_max)
bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec)
    const
{
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return false;
    auto sqrtd = sqrt(discriminant);
    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.hit_material = mater;
    return true;
}

//generate the bounding boxes for sphere
//return true if generateation is successful
bool sphere::bounding_box( aabb& output_box) const {
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}
#endif