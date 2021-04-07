#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "hittable.h"
#include "vec3.h"
#include <vector>
#include <cstdlib>
class triangle : public hittable
{
public:
    triangle() {}
    // triangle(std::vector <point3> ps, material m, double e = 1e-5) : point_set(ps), mater(m), epsilon(e){
    //     find_outward_normal(point_set);
    // }
    triangle(point3 a,point3 b, point3 c,double e = 1e-5) :  epsilon(e){
        point_set.push_back(a);
        point_set.push_back(b);
        point_set.push_back(c);
        find_outward_normal(point_set);
    }
    triangle(point3 a,point3 b, point3 c,vec3 n, vec3 p, vec3 q,double e = 1e-5) :  epsilon(e){
        point_set.push_back(a);
        point_set.push_back(b);
        point_set.push_back(c);
        outward_normal.push_back(n);
        outward_normal.push_back(p);
        outward_normal.push_back(q);
    }
    virtual bool bounding_box(aabb& output_box) const override;
    virtual bool hit(
        const ray &r, double t_min, double t_max, hit_record &rec) const
        override;
    
    // find the outward_normal of the triangle
    //here define for what ever face facing the camera as outward faces
    void find_outward_normal(std::vector <point3> &point_set){
        vec3 e1 = point_set[0] - point_set[2];
        vec3 e2 = point_set[0] - point_set[1];
        vec3 n = unit_vector(cross(e1,e2));
        vec3 p = point3(0,0,0) - point_set[0]; 
        if (dot(n,p) >0){
           outward_normal.push_back(n);
           outward_normal.push_back(n);
           outward_normal.push_back(n);
        }
        else{
           outward_normal.push_back(-n);
           outward_normal.push_back(-n);
           outward_normal.push_back(-n);
        }
    }

public:
    std::vector <point3> point_set;
    std::vector <vec3> outward_normal;
    double epsilon;  // the epsilon for hit detection
};

//the return structure for triangle_intersection test
struct triangle_intersection_ret{
    double t;
    double u;
    double v;
};

// this function is the one discussed in p15 of the baycentric coordinate lecture
// It detect whether ray o+td intersect with the triangel form by p0,p1,p2 and return t for ray
// two of the three bacentric coordinate, u,v
bool triangle_intersection(point3 p0,point3 p1, point3 p2, vec3 d,vec3 o ,double epsilon, triangle_intersection_ret& ret){
    vec3 e1 = p1 - p0;
    vec3 e2 = p2 - p0;
    vec3 q = cross(d,e2);
    double a = dot(e1,q);
    if (a < epsilon && a > -epsilon){
        return false;
    }
    double f = 1/a;
    vec3 s = o - p0;
    double u = f*(dot(s,q));
    if (u<0.0){
        return false;
    }
    vec3 r = cross(s,e1);
    double v = f*dot(d,r);
    
    if (v<0.0 or u+v >1.0){
        return false;
    }

    double t = f*dot(e2,r);
    ret.t = t;
    ret.v = v;
    ret.u = u;
    return true;
}

//this function test whether the triangle is being hit 
bool triangle::hit(const ray &r, double t_min, double t_max, hit_record &rec)
    const
{
    triangle_intersection_ret ret;
    if (triangle_intersection(point_set[0],point_set[1],point_set[2],r.dir,r.orig,epsilon,ret)){
        double root = ret.t;
        if (root < t_min || t_max < root){
            return false;
        }
        rec.t = root;
        rec.p = r.at(rec.t);

        //triangular interpolation here
        auto normal = (1 - ret.u - ret.v) * outward_normal[0] + ret.u * outward_normal[1] + ret.v * outward_normal[2];
        rec.normal = normal;

        return true;
    }
   
    return false;
}

bool triangle::bounding_box( aabb& output_box) const {
    double max_x = -infinity, min_x = infinity, max_y = -infinity , min_y = infinity,
                  max_z = -infinity,min_z = infinity;
    for (auto point : point_set){
        if (point.e[0] > max_x){
            max_x = point.e[0];
        }
        if (point.e[0] < min_x){
            min_x = point.e[0];
        }
        if (point.e[1] > max_y){
            max_y = point.e[1];
        }
        if (point.e[1] < min_y){
            min_y = point.e[1];
        }
        if (point.e[2] > max_z){
            max_z = point.e[2];
        }
        if (point.e[2] < min_z){
            min_z = point.e[2];
        }
    }
    output_box = aabb(point3(min_x,min_y,min_z),point3(max_x,max_y,max_z));
    return true;
}
#endif