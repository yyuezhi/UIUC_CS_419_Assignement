#ifndef CAMERA_H
#define CAMERA_H
#include "rtweekend.h"
class camera
{
public:
    camera(){};
    camera(point3 cam,vec3 at, vec3 up,double vh = 2.0 ,double foc = 1.0, double ar = 16.0 / 9.0) :camera_position(cam),lookat(at), lookup(up), 
    focal_length(foc), aspect_ratio(ar), viewport_height(vh){
        viewport_width = aspect_ratio * viewport_height;

        //configuring the unit vector for camera
        lookat = unit_vector(lookat);
        lookside = unit_vector(cross(lookat,lookup));
        lookup = unit_vector(cross(lookside,lookat));
        vec3 screen_center = camera_position + lookat * focal_length;
        lower_left_corner = screen_center - (viewport_width / 2) * lookside - (viewport_height / 2) * lookup ;
    }
    // get the ray for each pixel in perspective view
    ray get_ray(double u, double v) const
    {
        return ray(camera_position, lower_left_corner + u * lookside *  viewport_width+ v * lookup * viewport_height -
                               camera_position);
    }
    // get the ray for each pixel in orthographic view
    ray get_orthographic_ray(double u, double v) const
    {
        return ray(lower_left_corner + u * lookside *  viewport_width + v * lookup * viewport_height -
                               camera_position, lookat);
    }

private:
    //camera extrinsic parameter
    point3 camera_position;
    vec3 lookat;
    vec3 lookup;
    vec3 lookside;

    //camera intrinsic parameter
    point3 lower_left_corner;
    double focal_length;
    double viewport_height;
    double viewport_width;
    double aspect_ratio;
};
#endif