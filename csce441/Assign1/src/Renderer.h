#pragma once
#ifndef _Renderer_H_
#define _Renderer_H_

#include <iostream>
#include <vector>
#include "Rasterizer.h"
#include "Types.h"
#include "Type_functions.h"
#include "Image.h"

class Renderer
{

public:
	Renderer(std::shared_ptr<Image>& image, std::vector<float>& posBuf, int mode)
	{

		Raterizer rasterizer;
		std::vector<float> zBuffer;
		rasterizer.initZBuffer(zBuffer,image->getHeight()* image->getWidth());
		Utils::initSeed();

		Color rando = Utils::computeRandomColor();
		
		//for all triangles
		unsigned int iter = 0;
		while(iter < posBuf.size())
		{
			//create triangle
			Vertex v1 = Utils::makeVertex( posBuf[iter], posBuf[iter + 1], posBuf[iter + 2] );
			Vertex v2 = Utils::makeVertex( posBuf[iter + 3], posBuf[iter + 4], posBuf[iter + 5]);
			Vertex v3 = Utils::makeVertex( posBuf[iter + 6], posBuf[iter + +7], posBuf[iter + 8]);
			
			// compute  bounds of triangle
			BoundingBox triBounds;
			Utils::computeBounds(triBounds,v1 );
			Utils::computeBounds(triBounds,v2 );
			Utils::computeBounds(triBounds,v3 );
			
			//Utils::printBounds(triBounds);
			

			Color color1;
			Color color2;
			Color color3;
			//color by triangle
			
			color1 = rando;
			color2 = rando;
			color3 = rando;

			 if (mode == 0)
			 { // color each vertex
				 color1 = Utils::computeRandomColor();
				 color3 = Utils::computeRandomColor();
				 color3 = Utils::computeRandomColor();
			 }

			 else if(mode == 1)
			 {// unifrom obj color
			 	
			 	color1 = rando;
				color2 = rando;
				color3 = rando;
			 }




		
			std::vector<Color> colors = { color1,color2,color3 };
			//for all pixels in triangle bounding box
			for(int y = triBounds.minY; y < triBounds.maxY; ++y) 
			{
			 	for(int x = triBounds.minX; x < triBounds.maxX; ++x) {

			 		Vertex baryCoords = rasterizer.computeBarycentricCoordinate(v1,v2,v3,Vertex{(float)x,(float)y});
			 		
			 		if(rasterizer.isFrontFacing(baryCoords))
					{
						float z = rasterizer.interpolateZ(baryCoords,v1,  v2, v3);
						
						if(rasterizer.depthTest(zBuffer,image->getHeight(), image->getWidth(), x, y, z))
						{
							//Utils::printVertex(baryCoords);
			 				Color color = rasterizer.interpolateColor(colors, baryCoords);
							image->setPixel(x,y, color.r, color.g, color.b);
			 				
						}
			 		}
			 	}
			 }

				iter += 9;

		}
	}


};
#endif