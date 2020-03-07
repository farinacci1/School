#ifndef _TYPES_H_
#define _TYPES_H_
#include <iostream>
#include <limits>


struct BoundingBox
{
	float minX = std::numeric_limits<float>::infinity();
	float maxX = -std::numeric_limits<float>::infinity();
	float minY = std::numeric_limits<float>::infinity();
	float maxY = -std::numeric_limits<float>::infinity();
	float minZ = std::numeric_limits<float>::infinity();
	float maxZ = -std::numeric_limits<float>::infinity();

};
struct Vertex
{
	float x;
	float y;
	float z;
};
struct Color
{
	unsigned char r ;
	unsigned char g ;
	unsigned char b ;
};



#endif