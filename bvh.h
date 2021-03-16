#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"
#include <algorithm>
#include "hittable.h"
#include "hittable_list.h"

//compare two node based on their respective bounding boxes
inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(box_a) || !b->bounding_box( box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}

//compare based on x coordinate of the bounding boxes
bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

//compare based on y coordinate of the bounding boxes
bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

//compare based on x coordinate of the bounding boxes
bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

//structure of each bvh node
class bvh_node : public hittable {
    public:
        bvh_node();

        bvh_node(const hittable_list& list,int k)
            : bvh_node(list.objects, 0, list.objects.size(),k)
        {}

        bvh_node(
            const std::vector<shared_ptr<hittable>>& src_objects,
            size_t start, size_t end, int k);
        
    
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(aabb& output_box) const override;

    public:
        //left and right children node
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        //bouning box
        aabb box;
        //default maximum number of primitive in each leaf node
        int minimal_span = 3;
};

//generate the bounding box for this bvh node
//notice that this bvh node is an internal node, Not an leaf node
bool bvh_node::bounding_box( aabb& output_box) const {
    output_box = box;
    return true;
}

//devide wheher the bounding box of children node are hit
//return boolean
bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

//constructor to recursively consturct the bvh tree
bvh_node::bvh_node(
    const std::vector<shared_ptr<hittable>>& src_objects,
    size_t start, size_t end, int k
) {
    auto objects = src_objects; // Create a modifiable array of the source scene objects
    minimal_span = k;

    aabb temp_box1, temp_box2;
    int spanx,spany,spanz;

    //calculate the span of primitives under this node over x,y,z axis
    std::sort(objects.begin() + start, objects.begin() + end, box_x_compare);
    if (objects[start]->bounding_box(temp_box1) && objects[end-1]->bounding_box(temp_box2)){
    spanx = temp_box2.maximum.x() - temp_box1.minimum.x();
    }
    else{
        std::cout<<"Fatal error"<<std::endl;
        exit(2);
    }
    std::sort(objects.begin() + start, objects.begin() + end, box_y_compare);
    if (objects[start]->bounding_box(temp_box1) && objects[end-1]->bounding_box(temp_box2)){
    spany = temp_box2.maximum.y() - temp_box1.minimum.y();
    }
    else{
        std::cout<<"Fatal error"<<std::endl;
        exit(2);
    }
    std::sort(objects.begin() + start, objects.begin() + end, box_z_compare);
    if (objects[start]->bounding_box(temp_box1) && objects[end-1]->bounding_box(temp_box2)){
    spanz = temp_box2.maximum.z() - temp_box1.minimum.z();
    }
    else{
        std::cout<<"Fatal error"<<std::endl;
        exit(2);
    }

    //split over the largerest span axis
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;
   
    //sort all the primitives by axis with longest span
    size_t object_span = end - start;
    std::sort(objects.begin() + start, objects.begin() + end, comparator);
    if (object_span < minimal_span) {
        //left and right child will be the same
        left = right = make_shared<hittable_list>(std::vector<shared_ptr<hittable>>(objects.begin() + start, objects.begin() + end));
    } else if (object_span >= minimal_span && object_span < 2* minimal_span) {
        //split the primitives by medium 
        auto mid = start + object_span/2;
        left = make_shared<hittable_list>(std::vector<shared_ptr<hittable>>(objects.begin() + start, objects.begin() + mid));
        right = make_shared<hittable_list>(std::vector<shared_ptr<hittable>>(objects.begin() + mid, objects.begin() + end));
    } else {
        //split the primitives by medium and construct children node recursively
        auto mid = start + object_span/2;
        left = make_shared<bvh_node>(objects, start, mid,k);
        right = make_shared<bvh_node>(objects, mid, end,k);
    }

    aabb box_left, box_right;
    
    //get the bounding boxees for left and right children node
    if (  !left->bounding_box (box_left)
       || !right->bounding_box(box_right)
    ){
        std::cerr << "No bounding box in bvh_node constructor.\n";
        exit(1);
        }

    //merge two boxes in to one and get the bigger boxes for this node
    box = surrounding_box(box_left, box_right);
}


#endif