#ifndef _TYPES_H_
#define _TYPES_H_
#include <iostream>
#include <limits>


struct BoundingBox
{
	double minX = std::numeric_limits<double>::infinity();
	double maxX = -std::numeric_limits<double>::infinity();
	double minY = std::numeric_limits<double>::infinity();
	double maxY = -std::numeric_limits<double>::infinity();
	double minZ = std::numeric_limits<double>::infinity();
	double maxZ = -std::numeric_limits<double>::infinity();

};
struct Vertex
{
	double x;
	double y;
	double z;
};
struct Color
{
	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;
};
#endif