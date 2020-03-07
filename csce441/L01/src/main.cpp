#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Image.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;
struct boundingBox
{
	float minX;
	float maxX;
	float minY;
	float maxY;
} bounds;

struct Vertex
{
	int x;
	int y;
};
inline int max(int first, int second)
{
	return first > second ? first : second;
}
inline int min(int first, int second)
{
	return first < second ? first : second;
}

int main(int argc, char **argv)
{
	if(argc < 4) {
		cout << "Usage: L01 filename width height" << endl;
		return 0;
	}
	// Output filename
	string filename(argv[1]);
	// Width of image
	int width = atoi(argv[2]);
	// Height of image
	int height = atoi(argv[3]);

	Vertex v1{atoi(argv[4]),atoi(argv[5])};
	Vertex v2{atoi(argv[6]),atoi(argv[7])};
	Vertex v3{atoi(argv[8]),atoi(argv[9])};

	bounds.minX = min(v1.x, min(v2.x,v3.x));
	bounds.maxX = max(v1.x, max(v2.x,v3.x));
	bounds.minY = min(v1.y, min(v2.y,v3.y));
	bounds.maxY = max(v1.y, max(v2.y,v3.y));
	std::cout << "bounds::(minx,miny)::(maxX,maxY)::(" <<
	bounds.minX << " , "<< bounds.minY  << 
	")::("<< bounds.maxX <<" , "<< bounds.maxY<<")"<<std::endl;
	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);
	// Draw a rectangle
	
	for(int y = bounds.minY; y < bounds.maxY; ++y) {
		
		for(int x = bounds.minX; x < bounds.maxX; ++x) {
			//std::cout << "pixel::(x,y)::(" << x << " , " << y << ")"<< std::endl; 
			unsigned char r = 0;
			unsigned char g = 255;
			unsigned char b = 0;
			image->setPixel(x, y, r, g, b);
		}
	}
	image->setPixel(v1.x, v1.y, 0, 0, 255);
	image->setPixel(v2.x, v2.y, 0, 0, 255);
	image->setPixel(v3.x, v3.y, 0, 0, 255);
	// Write image to file
	image->writeToFile(filename);
	return 0;
}
