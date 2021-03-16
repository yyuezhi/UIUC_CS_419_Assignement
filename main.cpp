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
#include "triangle.h"
#include "bvh.h"

//investigate the color shoted by ray r and return the color obtain
color ray_color(const ray &r, const hittable &world, const pointlight &light_source)
{
    hit_record rec;
    if (world.hit(r, 0, infinity, rec))
    {
        point3 hitpoint = rec.p;
        ///checking whether the light is block
        // if yes then phong illumination model
        // if no then hard shading
        if (!light_source.check_blocking(hitpoint, world)){
            vec3 ambient = light_source.ambient * rec.hit_material.ambient;

            //diffuse
            vec3 surface_unit_norm = unit_vector(rec.normal);
            vec3 light_dir = unit_vector(light_source.light_position - rec.p);
            float diffuse_coef = std::max(dot(surface_unit_norm, light_dir), 0.0);
            vec3 diffuse = light_source.diffuse * (diffuse_coef * rec.hit_material.diffuse);

            //specular
            vec3 view_dir = unit_vector(-1 * r.dir);
            vec3 reflect_dir = 2*(dot(surface_unit_norm,light_dir) * surface_unit_norm) - light_dir;  
            float specular_coef = std::pow(std::max(dot(view_dir, reflect_dir), 0.0), rec.hit_material.shininess);
            vec3 specular = light_source.specular * (specular_coef * rec.hit_material.specular);  
            
            vec3 result = ambient + diffuse + specular;
            return result;
         }
         else{
             // since no light, so only ambient componennt only
             vec3 ambient = light_source.ambient * rec.hit_material.ambient;
             return ambient;
         }
    }
    //if no things is being hit, then produce a transition color for the background
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

//main function which setup parameter and generate ray to sample each pixel
int main()
{ 
    //file parameter
    std::ofstream MyFile("myimage.ppm");

    // Image parameter
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 750;
    const int image_height = static_cast<int>(image_width / aspect_ratio);


    // material
    material brass = material(color(0.32941,0.223529,0.027451),color(0.780392 ,0.568627 ,0.113725),color(0.992157 ,0.941176 ,0.807843),3);
    material silver = material(color(0.3 ,0.3 ,0.2) ,color(0.50754 ,0.50754 ,0.50754),color(0.508273 ,0.508273 ,0.508273),1.5);
    material jade = material(color(0.135 ,0.2225 ,0.1575 ) ,color(0.54 ,0.89 ,	0.63 ),color(0.316228 ,0.316228 ,0.316228 ),1);

    // World
    hittable_list world;

    //read a mesh and add it to the world
    mesh teapot = mesh("teapot_transform.obj");
    world.add(teapot);

    //generate sphere in the expoeriement
    //std::cout<<world.objects.size()<<std::endl;
    // for (int i=0;i<1000;i++){
    //     material temp = material(color(0.1,0.3 ) ,color(0.5,0.7 ),color(0.3,0.8 ),1);
    //     double x_cord = random_double(-150, 150);
    //     double y_cord = random_double(-80, 80);
    //     world.add(make_shared<sphere>(point3(x_cord, y_cord, -100), 3, temp));
    // }


    //light
    pointlight light_source(point3(5,5,5),color(0.4f,0.4f,0.4f), color(0.6f,0.6f,0.6f),color(0.9f,0.9f,0.9f));

    // toogle for orthographic projection
    bool orthographic_toogle = false;

    // toogle for multijittering
    bool multi_jitter_toogle = false;
    int fine_grid_length = 4;
    int rough_grid_length = 2;

    // Camera for perspective view

    camera cam(point3(5,5,0),vec3(-0.2,-0.5,-1),vec3(-0.2,1,-0.5));

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
                    pixel_color += ray_color(r, world,light_source);
                }
                //average the result
                pixel_color.e[0] = pixel_color.e[0] /fine_grid_length;
                pixel_color.e[1] = pixel_color.e[1] /fine_grid_length;
                pixel_color.e[2] = pixel_color.e[2] /fine_grid_length;
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
                pixel_color = ray_color(r, world_bvhtree,light_source);
            }
            write_color(MyFile, pixel_color);
        }
    }
    std::cerr << "\nDone.\n";
}