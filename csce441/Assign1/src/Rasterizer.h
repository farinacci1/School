#pragma once
#ifndef _Raterizer_H_
#define _Raterizer_H_
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "Types.h"
#include "Type_functions.h"



class Raterizer
{

public:
	Raterizer()
	{
	}


	Vertex computeBarycentricCoordinate(const Vertex& V1 , const Vertex& V2,const Vertex& V3 ,const Vertex& P )
	{
		/*
		equation implmentation modeled after solution by molbdnilo:
		https://stackoverflow.com/questions/25385361/point-within-a-triangle-barycentric-co-ordinates
		*/
		float totalArea, a, b, c;
		totalArea = (V2.y - V3.y)*(V1.x-V3.x) + (V3.x - V2.x)*(V1.y - V3.y);
		if (totalArea == 0) return Utils::makeVertex();
		

		a = (V2.y - V3.y)*(P.x - V3.x) + (V3.x - V2.x)*(P.y - V3.y);
		b = (V3.y - V1.y)*(P.x - V3.x) + (V1.x - V3.x)*(P.y - V3.y);
		a = a / totalArea;
		b = b / totalArea;
		c = 1 - a - b;
		
		return Utils::makeVertex(a,b,c);
	}


	Color interpolateColor(const std::vector<Color> &colors ,const Vertex&  baryCoords)
	{
		Color color;
		color.r = (unsigned char)(colors[0].r * baryCoords.x  + colors[1].r * baryCoords.y + colors[2].r * baryCoords.z);
		color.g = (unsigned char)(colors[0].g * baryCoords.x  + colors[1].g * baryCoords.y + colors[2].g * baryCoords.z);
		color.b = (unsigned char)(colors[0].b * baryCoords.x  + colors[1].b * baryCoords.y + colors[2].b * baryCoords.z);
		return color;
	}


	bool isFrontFacing(const Vertex&  pixel)
	{
		return 
		(
			0 <= pixel.x && pixel.x <= 1 && 
			0 <= pixel.y && pixel.y <= 1 &&
			0 <= pixel.z && pixel.z <= 1 &&
			pixel.x + pixel.y + pixel.z == 1
		);
	}
	void initZBuffer(std::vector<float>& zBuffer,int size)
	{
		float inf = std::numeric_limits<float>::infinity();
		for (int i = 0; i < size; i++)
		{
			zBuffer.push_back(-inf);
		}
	}
	float interpolateZ(Vertex& baryCoords, Vertex& v1, Vertex & v2, Vertex & v3)
	{
		return baryCoords.x * v1.z + baryCoords.y * v2.z + baryCoords.z * v3.z;

	}


	bool depthTest(std::vector<float>& zBuffer,int imageHeight,int imageWidth,  int col, int row, const float& z)
	{

		if (conditions( imageHeight, imageWidth,col, row) )
			return false;

		int index = row * imageWidth + col;
		 if (z >= zBuffer[index])
		{
			zBuffer[index] = z;
			return true;
		}
		
		return false;
	}

private:

	bool conditions(int imageHeight,int imageWidth,  int col, int row)
	{
		return (col < 0 || col > imageWidth || row < 0 || row > imageHeight);

	}


};
#endif