#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "sphere.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "math.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "camera.h"
#include "pointlight.h"
#include "multigittering.cpp"
#include <algorithm>
#include "material.h"
#include "triangle.h"
#include "bvh.h"
#include "aarect.h"
#include "pdf.h"
#include "box.h"
hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto purple = make_shared<lambertian>(color(.62, .15, .45));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    //objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    //objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<sphere>(point3(400, 80, 80 ),75,  white));
    objects.add(make_shared<sphere>(point3(0, 180, 555 ),150,  red));
    objects.add(make_shared<box>(point3(130, 0, 65), point3(295, 165, 230),  purple));
    objects.add(make_shared<box>(point3(265, 0, 295), point3(430, 330, 460), green));
    objects.add(make_shared<xz_rect>(-100, 15555, -100, 15555, 0, white));
    //objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    //objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    return objects;
}

//investigate the color shoted by ray r and return the color obtain
color ray_color(
    const ray& r, const color& background, const hittable& world,
    shared_ptr<hittable>& lights, int depth
) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
    double pdf_val;
    color albedo;
    if (!rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf_val))
        return emitted;
    hittable_pdf light_pdf(lights, rec.p);
    scattered = ray(rec.p, light_pdf.generate());
    pdf_val = light_pdf.value(scattered.direction());

    return emitted
         + albedo * rec.mat_ptr->scattering_pdf(r, rec, scattered)
                  * ray_color(scattered, background, world, lights, depth-1) / pdf_val;
}

//main function which setup parameter and generate ray to sample each pixel
int main()
{ 
    //file parameter
    std::ofstream MyFile("myimage.ppm");

    // Image parameter
    const auto aspect_ratio = 1.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int max_depth = 50;

    // World
    hittable_list world;

    world = cornell_box();
    shared_ptr<hittable> lights =
        make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>());
    color background = color(0,0,0);
    vec3 lookfrom = point3(278, 278, -800);
    vec3 lookat = point3(278, 278, 0);
    vec3 vup(0,1,0);
    double vfov = 40.0;
    auto aperture = 0.0;
    auto dist_to_focus = 10.0;


    //light
    pointlight light_source(point3(5,5,5),color(0.4f,0.4f,0.4f), color(0.6f,0.6f,0.6f),color(0.9f,0.9f,0.9f));

    // toogle for orthographic projection
    bool orthographic_toogle = false;

    // toogle for multijittering
    bool multi_jitter_toogle = true;
    int fine_grid_length = 16;
    int rough_grid_length = 4;

    // Camera for perspective view
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

    //construct the BVH tree
    bvh_node world_bvhtree = bvh_node(world,100);

    // Render
    MyFile << "P3\n"
           << image_width << " " << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {

            color pixel_color(0, 0, 0);
            int sample_per_pixel;
            if (multi_jitter_toogle){
                //generate the fine sample grids where the random sample should land on
                std::vector <jitter_coordinate> sample_result = multigittering_grid_generator(fine_grid_length,rough_grid_length);
                //generate random point in the sample grid
                for (int s = 0;s<sample_result.size() ;s++){
                    auto u = (i + (random_double()+sample_result[s].x) / fine_grid_length ) / (image_width - 1);
                    auto v = (j + (random_double()+sample_result[s].y) / fine_grid_length) / (image_height - 1);
                    ray r;
                    //orthographic_toogle
                    if (orthographic_toogle){
                       r = cam.get_orthographic_ray(u, v);
                    }
                    else{
                       r = cam.get_ray(u, v);
                    }
                    pixel_color += ray_color(r, background, world, lights, max_depth);
                }
                //average the result
                sample_per_pixel = fine_grid_length;
            }
            else{
                //this is the non jitted version
                auto u = double(i) / (image_width - 1);
                auto v = double(j) / (image_height - 1);
                ray r;
                //orthographic toogle
                if (orthographic_toogle){
                   r = cam.get_orthographic_ray(u, v);
                }
                else{
                   r = cam.get_ray(u, v);
                }
                pixel_color = ray_color(r, background, world, lights, max_depth);
                sample_per_pixel = 1;
            }
            write_color(MyFile, pixel_color, sample_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}


