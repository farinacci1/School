#pragma once
#ifndef _Raterizer_H_
#define _Raterizer_H_
#include <iostream>
#include <string>
#include <vector>
#include "Types.h"


static unsigned long seed = 543;

class RasterizationEngine
{
public:
	RasterizationEngine()
	{
	}

	Vertex makeVertex(double x = 0, double y = 0, double z = 0)
	{
		return Vertex{x,y,z};
	}
	Vertex computeBarycentricCoordinate(const Vertex& V1 , const Vertex& V2,const Vertex& V3 ,const Vertex& P )
	{
		double totalArea = areaOfTriangle (V1,V2,V3);
		if (totalArea == 0) return makeVertex();

		double areaA = areaOfTriangle(V2,V3,P);
		double areaB = areaOfTriangle(V1,V3,P);
	
		
		double a = areaA / totalArea;
		double b = areaB / totalArea;
		double c = 1 - a - b;
		
		return makeVertex(a,b,c);
	}

	void computeBounds(BoundingBox& box,const Vertex& comparator )
	{
		box.minX = min(box.minX,comparator.x);
		box.maxX = max(box.maxX,comparator.x);
		box.minY = min(box.minY,comparator.y);
		box.maxY = max(box.maxY ,comparator.y);
		box.minZ = min(box.minZ,comparator.z);
		box.maxZ = max(box.maxZ ,comparator.z);


	}
	void interpolateColor(Color& color,const std::vector<Color> &colors ,const Vertex&  baryCoords)
	{
		color.r = (unsigned char) (colors[0].r * baryCoords.x  + colors[1].r * baryCoords.y + colors[2].r * baryCoords.z);
		color.g = (unsigned char)(colors[0].g * baryCoords.x  + colors[1].g * baryCoords.y + colors[2].g * baryCoords.z);
		color.b = (unsigned char)(colors[0].b * baryCoords.x  + colors[1].b * baryCoords.y + colors[2].b * baryCoords.z);
	}
	void computeRandomColor(Color& color)
	{
		color.r = psudeoRandomGenerator(256);
		color.g = psudeoRandomGenerator(256);
		color.b = psudeoRandomGenerator(256);
	}
	bool depthTest(const Vertex&  pixel)
	{
		return 
		(
			0 <= pixel.x && pixel.x <= 1 && 
			0 <= pixel.y && pixel.y <= 1 &&
			0 <= pixel.z && pixel.z <= 1 &&
			pixel.x + pixel.y + pixel.z == 1
		);
	}


private:
	double min(const double first, const double second)
	{
		return first < second ? first : second;
	}
	double max(const double first, const double second)
	{
		return first > second ? first : second;
	}
	double areaOfTriangle(const Vertex& V1 , const Vertex& V2,const Vertex& V3 )
	{
		double e0 = V2.x * V3.y - V2.y* V3.x;
		double e1 = V3.x * V1.y - V3.y* V1.x;
		double e2 = V1.x * V2.y - V1.y* V2.x;
		return e0 + e1 + e2;
	}
	int psudeoRandomGenerator(int range)
	{
		seed = seed * 1103515245 + 12345;
		return seed % range;
	}


};
#endif