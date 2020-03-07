#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <cstdlib>
#include "Image.h"
#include "Types.h"
#include "Rasterizer.h"
// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

int main(int argc, char **argv)
{
	if(argc < 4) {
		cout << "Usage: L02 filename width height" << endl;
		return 0;
	}
	// Output filename
	string filename(argv[1]);
	// Width of image
	int width = atoi(argv[2]);
	// Height of image
	int height = atoi(argv[3]);
	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);

	RasterizationEngine engine;
	Vertex v1 = engine.makeVertex( atoi(argv[4]),atoi(argv[5]) );
	Vertex v2 = engine.makeVertex( atoi(argv[6]),atoi(argv[7]) );
	Vertex v3 = engine.makeVertex( atoi(argv[8]),atoi(argv[9]) );
	cout << "vertices::(x,y)::(" << v1.x << ","<< v1.y << ")::("<<v2.x << ","<<v2.y << ")::("<<v3.x <<"," <<v3.y << ")"<<endl;
	BoundingBox objBounds;

	engine.computeBounds(objBounds,v1 );
	engine.computeBounds(objBounds,v2 );
	engine.computeBounds(objBounds,v3 );

	std::cout << "bounds::(minx,miny)::(maxX,maxY)::(" <<
	objBounds.minX << " , "<< objBounds.minY  << 
	")::("<< objBounds.maxX <<" , "<< objBounds.maxY<<")"<<std::endl;

	Color color1;
	Color color2;
	Color color3;

	engine.computeRandomColor(color1);
	engine.computeRandomColor(color2);
	engine.computeRandomColor(color3);

	// Draw a triangle
	vector<Color> colors = { color1,color2,color3 };
	for(double y = objBounds.minY; y < objBounds.maxY; ++y) {
		for(double x = objBounds.minX; x < objBounds.maxX; ++x) {

			Vertex baryCoords = engine.computeBarycentricCoordinate(v1,v2,v3,Vertex{x,y});
			if(engine.depthTest(baryCoords))
			{
				
				Color color;
				engine.interpolateColor(color,colors, baryCoords);
				image->setPixel((int)x,(int)y, color.r, color.g, color.b);
				cout << x <<"," << y << endl;
			}
		}
	}

	// Write image to file
	image->writeToFile(filename);
	return 0;
}
