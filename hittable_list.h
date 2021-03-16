#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include "hittable.h"
#include "mesh.h"
#include <memory>
#include <vector>
#include "aabb.h"
using std::make_shared;
using std::shared_ptr;

//this is a list of hiiitble object
class hittable_list : public hittable
{
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }
    hittable_list(std::vector<shared_ptr<hittable>> objectslist) { objects = objectslist; }
    //clear the hittable list
    void clear() { objects.clear(); }

    //add hitable object to the list
    void add(shared_ptr<hittable> object) { objects.push_back(object); }
    void add(mesh m);
    virtual bool hit(
        const ray &r, double t_min, double t_max, hit_record &rec) const
        override;
    virtual bool bounding_box( aabb& output_box) const override;

public:
    std::vector<shared_ptr<hittable>> objects;
};

//add the primitives in the mesh structure to the hittalbe list
void hittable_list::add (mesh m){
    for (int i=0;i<m.faces.size()/3;i++){
        objects.push_back(make_shared<triangle>(m.vertices[m.faces[3*i]], m.vertices[m.faces[3*i + 1]] , m.vertices[m.faces[3*i + 2]]
        ,m.verticesnormal[m.faces[3*i]], m.verticesnormal[m.faces[3*i + 1]] , m.verticesnormal[m.faces[3*i + 2]],m.matter));
    }
}

//This function check whether at least one of the object in the hittable list is being hit 
//by the ray r with range (t_min,t_max), return boolean value
bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    for (const auto &object : objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

//construct the bounding boxs for all the primitives in the list
//return boolean indicating whether such bounding boxes is succesfully
//constructed
bool hittable_list::bounding_box( aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box( temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}
#endif