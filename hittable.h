#ifndef HITTABLE_H
#define HITTABLE_H
#include "ray.h"
#include "rtweekend.h"
#include "aabb.h"

class material;
//the struct which defines the neceesary parameter for hit record, if a ray hits the hittable object
struct hit_record
{
    point3 p;  //hit point
    vec3 normal;  // the surface normal of the hit point
    double t;    // the t of thr ray at which it is hit
    double u;   //u v coordinate for texture mappping
    double v;
    bool front_face;  // whether the ray hit the front or back of the surface
    shared_ptr<material> mat_ptr;
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
    virtual double pdf_value(const point3& o, const vec3& v) const {
        return 0.0;
    }

    virtual vec3 random(const vec3& o) const {
        return vec3(1, 0, 0);
    }
};

class flip_face : public hittable {
    public:
        flip_face(shared_ptr<hittable> p) : ptr(p) {}

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override {

            if (!ptr->hit(r, t_min, t_max, rec))
                return false;

            rec.front_face = !rec.front_face;
            return true;
        }

        virtual bool bounding_box( aabb& output_box) const override {
            return ptr->bounding_box(output_box);
        }

        virtual double pdf_value(const point3& o, const vec3& v) const {
            return 0.0;
        }

        virtual vec3 random(const vec3& o) const {
            return vec3(1, 0, 0);
        }

    public:
        shared_ptr<hittable> ptr;
};
#endif