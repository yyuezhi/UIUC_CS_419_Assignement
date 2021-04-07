#ifndef CAMERA_H
#define CAMERA_H
#include "vec3.h"
class camera {
    public:
        camera(
            point3 lookfrom,
            point3 _lookat,
            vec3   vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio,
            double aperture,
            double focus_dist
        ) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            viewport_height = 2.0 * h;
            viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - _lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

            lens_radius = aperture / 2;
        }

        ray get_ray(double s, double t) const {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(
                origin + offset,
                lower_left_corner + s*horizontal + t*vertical - origin - offset
            );
        }

                // get the ray for each pixel in orthographic view
        ray get_orthographic_ray(double u, double v) const
        {
            return ray(lower_left_corner + u * horizontal *  viewport_width + v * vertical * viewport_height -
                                origin, lookat);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 lookat;
        double viewport_height;
        double viewport_width;
        vec3 u, v, w;
        double lens_radius;

};
#endif