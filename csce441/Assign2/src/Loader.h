#ifndef LOADER_H_
#define LOADER_H_

#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLSL.h"
#include "MatrixStack.h"


class Loader
{
    public:
        void load(std::string meshName,std::vector<float>& posBuf,std::vector<float>& norBuf,int& indCount)
        {      
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string errStr;
            int rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
            if(!rc) {
                std::cerr << errStr << std::endl;
            } else {
                // Some OBJ files have different indices for vertex positions, normals,
                // and texture coordinates. For example, a cube corner vertex may have
                // three different normals. Here, we are going to duplicate all such
                // vertices.
                // Loop over shapes
                for(size_t s = 0; s < shapes.size(); s++) {
                    // Loop over faces (polygons)
                    size_t index_offset = 0;
                    for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                        size_t fv = shapes[s].mesh.num_face_vertices[f];
                        // Loop over vertices in the face.
                        for(size_t v = 0; v < fv; v++) {
                            // access to vertex
                            tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                            posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
                            posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
                            posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
                            if(!attrib.normals.empty()) {
                                norBuf.push_back(attrib.normals[3*idx.normal_index+0]);
                                norBuf.push_back(attrib.normals[3*idx.normal_index+1]);
                                norBuf.push_back(attrib.normals[3*idx.normal_index+2]);
                            }
                        }
                        index_offset += fv;
                        // per-face material (IGNORE)
                        shapes[s].mesh.material_ids[f];
                    }
                }
            }
            indCount = posBuf.size()/3; // number of indices to be rendered

            assert(norBuf.size() == posBuf.size());
            GLSL::checkError(GET_FILE_LINE);  
        }
};

#endif