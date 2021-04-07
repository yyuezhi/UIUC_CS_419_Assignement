#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H
#include "hittable.h"
#include "vec3.h"
#include <vector>
class tetrahedron : public hittable
{
public:
    tetrahedron() {}
    tetrahedron(std::vector <point3> ps, material m, double e = 1e-5) : point_set(ps), mater(m), epsilon(e){
        find_outward_normal(point_set,outward_normal);
    }
    tetrahedron(point3 a,point3 b, point3 c, point3 d,material m, double e = 1e-5) : mater(m), epsilon(e){
        point_set.push_back(a);
        point_set.push_back(b);
        point_set.push_back(c);
        point_set.push_back(d);
        find_outward_normal(point_set,outward_normal);
    }

    virtual bool hit(
        const ray &r, double t_min, double t_max, hit_record &rec) const
        override;
    
    void find_outward_normal(std::vector <point3> &point_set, std::vector <vec3> &outward_normal){
        vec3 e1; 
        vec3 e2;
        vec3 n;
        vec3 p;
        
        e1 = point_set[1] - point_set[2];
        e2 = point_set[1] - point_set[3];
        n = unit_vector(cross(e1,e2));
        p = point_set[0] - point_set[1];
        if (dot(n,p) >0){
           outward_normal.push_back(-n);
        }
        else{
           outward_normal.push_back(n);
        }


        e1 = point_set[0] - point_set[3];
        e2 = point_set[0] - point_set[2];
        n = unit_vector(cross(e1,e2));
        p = point_set[1] - point_set[0];
        if (dot(n,p) >0){
           outward_normal.push_back(-n);
        }
        else{
           outward_normal.push_back(n);
        }


        e1 = point_set[0] - point_set[1];
        e2 = point_set[0] - point_set[3];
        n = unit_vector(cross(e1,e2));
        p = point_set[2] - point_set[0];
        if (dot(n,p) >0){
           outward_normal.push_back(-n);
        }
        else{
           outward_normal.push_back(n);
        }


        e1 = point_set[0] - point_set[1];
        e2 = point_set[0] - point_set[2];
        n = unit_vector(cross(e1,e2));
        p = point_set[3] - point_set[0];
        if (dot(n,p) >0){
           outward_normal.push_back(-n);
        }   
        else{
           outward_normal.push_back(n);
        }

        for (auto n:outward_normal){
            std::cout<<" "<<n.x()<<" "<<n.y()<<" "<<n.z()<<std::endl;

        }

    }

public:
    std::vector <point3> point_set;
    std::vector <vec3> outward_normal;
    material mater;
    double epsilon;  // the epsilon for hit detection
};

struct triangle_intersection_ret{
    double t;
    double u;
    double v;
};

bool triangle_intersection(point3 p0,point3 p1, point3 p2, vec3 d,vec3 o ,double epsilon, triangle_intersection_ret& ret){
    vec3 e1 = p1 - p0;
    vec3 e2 = p2 - p0;
    vec3 q = cross(d,e2);
    double a = dot(e1,q);
    if (abs(a) < epsilon){
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

bool tetrahedron::hit(const ray &r, double t_min, double t_max, hit_record &rec)
    const
{
    triangle_intersection_ret ret;
    triangle_intersection_ret temp_ret;
    temp_ret.t = infinity;
    vec3 normal;
    bool hit_anything = false;
    //check four face of the tetrahedon
    if (triangle_intersection(point_set[1],point_set[2],point_set[3],unit_vector(r.dir),r.orig,epsilon,ret)){
        temp_ret = ret;
        normal =  outward_normal[0];
        hit_anything = true;
        //std::cout<<"a"<<std::endl;
    }
    if (triangle_intersection(point_set[0],point_set[2],point_set[3],unit_vector(r.dir),r.orig,epsilon,ret) && (ret.t < temp_ret.t)){
        temp_ret = ret;
        normal =  outward_normal[1];
        hit_anything = true;
        //std::cout<<"b"<<std::endl;
    }
    if (triangle_intersection(point_set[0],point_set[1],point_set[3],unit_vector(r.dir),r.orig,epsilon,ret) && (ret.t < temp_ret.t)){
        temp_ret = ret;
        normal =  outward_normal[2];
        hit_anything = true;
        //std::cout<<"c"<<std::endl;
    }
    if (triangle_intersection(point_set[0],point_set[1],point_set[2],unit_vector(r.dir),r.orig,epsilon,ret) && (ret.t < temp_ret.t)){
        temp_ret = ret;
        normal =  outward_normal[3];
        hit_anything = true;
        //std::cout<<"d"<<std::endl;
    }
    if (!hit_anything){
        return false;
    }

    double root = temp_ret.t;
    if (root < t_min || t_max < root){
        return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    rec.set_face_normal(r, normal);
    rec.hit_material = mater;
    //std::cout<<"true"<<std::endl;
    return true;
}




#endif