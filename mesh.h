#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#ifndef MESH_H
#define MESH_H
#include "tiny_object_loader.h"
#include "triangle.h"
#include <fstream>
#include <algorithm>
#include "material.h"
class mesh 
{
    public: 
       mesh(std::string mesh_name);


    public:
       std::vector<point3> vertices;
       std::vector<int> faces;
       std::vector<vec3> verticesnormal;
       std::vector<vec3> facesnormal;
       std::string meshname;
       //default mesh material
       material matter = material(color(0.3 ,0.3 ,0.2) ,color(0.50754 ,0.50754 ,0.50754),color(0.508273 ,0.508273 ,0.508273),1.5);
};

//construct which read a mesh in obj format
mesh::mesh(std::string mesh_name){
    meshname = mesh_name;
    std::string line, lead_char, x,y,z;
    std::ifstream MyReadFile(mesh_name);
    vertices.push_back(point3(0,0,0));
    //read the vertices and faces
    while (getline (MyReadFile, line)) {
        std::istringstream temp_stream(line);
        temp_stream >> lead_char >>x >>y>>z;
        if (lead_char == "v"){
            vertices.push_back(point3(stod(x),stod(y),stod(z)));
        }
        else if (lead_char == "f"){
            faces.push_back(stoi(x));
            faces.push_back(stoi(y));
            faces.push_back(stoi(z));
        }
    }
    std::cout<<"read mesh"<<std::endl;
    if (faces.size()%3!=0){
        std::cerr<<"not a triangle mesh"<<std::endl;
        exit(1);
    }
    auto nfaces = faces.size()/3;
    auto nvertices = vertices.size();
    vec3 v1,v2,v3,e1,e2,n;
    double area;

    //calculate face normal vector, wieght by triangle area
    for (int i=0;i<nfaces;i++){
        v1 = vertices[faces[3*i]];
        v2 = vertices[faces[3*i+1]];
        v3 = vertices[faces[3*i+2]];
        e1 = v2 - v1;
        e2 = v3 - v1;
        n = cross(e1,e2);
        facesnormal.push_back(n/2);
    }
    //calculate vertice normal based on triangle weighting
    std::cout<<"calculate surface normal vector"<<std::endl;
    verticesnormal.push_back(point3(0,0,0));
    for (int i=1;i<nvertices;i++){
        int found_time = 0;
        auto start_it = begin(faces);
        vec3 base(0,0,0);
        while (start_it != end(faces)) {
            start_it = std::find(start_it, end(faces), i);
            if (start_it != end(faces)) {
                auto const pos = std::distance(begin(faces), start_it);
                base += facesnormal[pos/3];
                ++start_it;
                found_time++;
            }
        }
        verticesnormal.push_back(unit_vector(base));
    }
    std::cout<<"calculate normal vector"<<std::endl;
}
#endif
