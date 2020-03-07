#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <cstdlib>

#include "Image.h"
#include "Types.h"
#include "Type_functions.h"

#include "Loader.h"
#include "Renderer.h"

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
	string inputFile(argv[1]);
	string outpuFIle(argv[2]);
	// Width of image
	int width = atoi(argv[3]);
	// Height of image
	int height = atoi(argv[4]);
	//color mode
	int mode = atoi(argv[5]);
	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);


	Loader loader;
	vector<float> posBuf;
	loader.objLoader(image,inputFile,posBuf);
	Renderer renderer(image,posBuf,mode);




	// Write image to file
	image->writeToFile(outpuFIle);
	return 0;
}
