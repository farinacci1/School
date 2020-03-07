#ifndef _TYPE_FUNCTIONS_H_
#define _TYPE_FUNCTIONS_H_
#include "Types.h"
#include <time.h>
namespace Utils{
	static unsigned long seed = 543;

	float min(const float first, const float second)
	{
		return first < second ? first : second;
	}
	float max(const float first, const float second)
	{
		return first > second ? first : second;
	}
	int ceil(float num)
	{   
		int val = num;
	    if ((float)val != num)
	        return val+1;
	    return val;
	}
	int floor(float num)
	{
		int val = num;
	    if ((float)val != num)
	        return val - 1;
	    return val;		
	}
	
	//type functions
	Vertex makeVertex(float x = 0, float y = 0, float z = 0)
	{
		return Vertex{x,y,z};
	}
	Color pickColor(unsigned char r,unsigned char g,unsigned char  b)
	{
		return Color{r,g,b} ;

	}
	unsigned char psudeoRandomGenerator(int range)
	{
		seed = seed * 1103515245 + 12345;
		return seed % range;
	}

	Color computeRandomColor()
	{
		return Color{psudeoRandomGenerator(256),psudeoRandomGenerator(256),psudeoRandomGenerator(256)};
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
	void initSeed()
	{
		 srand (time(NULL));
		 seed = rand() % 5432 + 1;
	}
	float abs(float num) 
	{
		return num < 0 ? -num : num;
	}
	void printBounds(const BoundingBox& box)
	{
		std::cout<<"BoundingBox::(min,min,min),(max,max,max)::(" << box.minX << ","<< box.minY << "," << box.minZ <<
			"),("<< box.maxX << ","<< box.maxY << ","<<box.maxZ <<")" << std::endl;
	}
	void printVertex(const Vertex&  v)
	{
		std::cout <<"vertex(x,y,z)::(" << v.x << ","<< v.y <<","<< v.z << ")"<<std::endl;
	}

	
}
#endif