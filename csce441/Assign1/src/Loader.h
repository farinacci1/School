#pragma once
#ifndef _Loader_H_
#define _Loader_H_

#include <iostream>
#include <string>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Types.h"
#include "Type_functions.h"
#include "Image.h"
class Loader{
	public:
		Loader()
		{

		}
		void objLoader(std::shared_ptr<Image>& image,std::string meshName,std::vector<float>& posBuf)
		{
			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string errStr;

			BoundingBox modelBounds;
			

			bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
			if (!rc) {
				std::cerr << errStr << std::endl;

			}
			else {
				// Some OBJ files have different indices for vertex positions, normals,
				// and texture coordinates. For example, a cube corner vertex may have
				// three different normals. Here, we are going to duplicate all such
				// vertices.
				// Loop over shapes
				for (size_t s = 0; s < shapes.size(); s++) {
					// Loop over faces (polygons)
					size_t index_offset = 0;
					for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
						size_t fv = shapes[s].mesh.num_face_vertices[f];
						// Loop over vertices in the face.
						for (size_t v = 0; v < fv; v++) {
							// access to vertex
							tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
							posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 0]);//x
							posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 1]); // y
							posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 2]);// z

							int lastIndex =  posBuf.size() -1;
							Vertex v1 = Utils::makeVertex(posBuf[lastIndex -2],posBuf[lastIndex - 1],posBuf[lastIndex]);
							Utils::computeBounds(modelBounds,v1);
						}
						index_offset += fv;
						// per-face material (IGNORE)
						shapes[s].mesh.material_ids[f];
					}
				}
			}
			
			//Utils::printBounds(modelBounds);
			
			process(image,modelBounds, posBuf);
			
		}
	private:
		void process(std::shared_ptr<Image>& image,const BoundingBox& modelBounds, std::vector<float>& posBuf)
		{
			//computations
			Vertex modelCenter = computeModelCenter(modelBounds);
			float scale = computeScaleFactor(image,modelBounds);
			computeScaledCenter(modelCenter, scale);

			
			for(unsigned int i = 0; i < posBuf.size(); i = i + 3)
			{
				
				Vertex vert = Utils::makeVertex((float)posBuf[i],(float)posBuf[i + 1],(float)posBuf[i + 2]);
				transform(vert,scale, modelBounds,modelCenter);
				posBuf[i] = vert.x;
				posBuf[i + 1] = vert.y;
				posBuf[i + 2] = vert.z;

			}
		}


		void transform(Vertex& vert, const float scale,const BoundingBox& modelBounds,const Vertex& modelCenter )
		{
			//shift model  to + axis
			Vertex offset = offsetModel(modelBounds);
			vert.x -= offset.x;
			vert.y -= offset.y;
			vert.z -= offset.z;

			//scale model
			vert.x *= scale;
			vert.y *= scale;
			vert.z *= scale;

			
			
		}

		Vertex computeModelCenter(const BoundingBox& modelBounds)
		{

			return Utils::makeVertex(
				(modelBounds.maxX - modelBounds.minX)/2,
				(modelBounds.maxY - modelBounds.minY)/2,
				(modelBounds.maxZ - modelBounds.minZ)/2
			);
		}
		Vertex offsetModel(const BoundingBox& modelBounds)
		{
			return Utils::makeVertex(
					modelBounds.minX,
					modelBounds.minY,
					modelBounds.minZ
			);
		}
		void computeScaledCenter( Vertex& modelCenter,const float scale)
		{
			modelCenter.x = scale * modelCenter.x;
			modelCenter.y = scale * modelCenter.y;
			modelCenter.z = scale * modelCenter.z;
	
		}

		float computeScaleFactor( std::shared_ptr<Image>& image,const BoundingBox& modelBounds)
		{
			return Utils::min(
				(float)image->getWidth() / (modelBounds.maxX - modelBounds.minX), 
				(float)image->getHeight() / (modelBounds.maxY - modelBounds.minY)
			);
		}
	
};
#endif