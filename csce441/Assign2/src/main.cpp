#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "GLSL.h"

#include "MatrixStack.h"
#include "Window.h"
#include "Loader.h"
#include "Robot.h"
#include "ActionHandler.h"



std::string RESOURCE_DIR = "./"; // Where the resources are loaded from
GLuint progID;
Window window;
Robot robot;
Loader loader;


std::map<std::string,GLint> attrIDs;
std::map<std::string,GLint> unifIDs;
std::map<std::string,GLuint> bufIDs;
std::vector<float> posBuf;
std::vector<float> norBuf;
int indCount;
Robot ActonHandler::robot;

void Shader(std::string vShaderName,std::string fShaderName)
{
    // Create shader handles
    GLuint vShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shader sources
    const char *vShaderText = GLSL::textFileRead(vShaderName.c_str());
    const char *fShaderText = GLSL::textFileRead(fShaderName.c_str());
    glShaderSource(vShaderID, 1, &vShaderText, NULL);
    glShaderSource(fShaderID, 1, &fShaderText, NULL);
            
    // Compile vertex shader
    int rc;
    glCompileShader(vShaderID);
    glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &rc);
    if(!rc) 
	{
        GLSL::printShaderInfoLog(vShaderID);
        std::cout << "Error compiling vertex shader " << vShaderName << std::endl;
        return;
    }
            
    // Compile fragment shader
    glCompileShader(fShaderID);
    glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &rc);
    if(!rc) 
	{
        GLSL::printShaderInfoLog(fShaderID);
        std::cout << "Error compiling fragment shader " << fShaderName << std::endl;
        return;
    }
            
    // Create the program and link
    progID = glCreateProgram();
    
    glAttachShader(progID, vShaderID);
    glAttachShader(progID, fShaderID);
    glLinkProgram(progID);
    glGetProgramiv(progID, GL_LINK_STATUS, &rc);
    if(!rc) 
	{
        GLSL::printProgramInfoLog(progID);
        std::cout << "Error linking shaders " << vShaderName << " and " << fShaderName << std::endl;
        return;
    }
    GLSL::checkError(GET_FILE_LINE);
}

// This function is called once to initialize the scene and OpenGL
static void init()
{
	// Initialize time.
	glfwSetTime(0.0);
	// Set background color.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);
	std::string vShaderName = RESOURCE_DIR + "vert.glsl";
	std::string fShaderName = RESOURCE_DIR + "frag.glsl";
	Shader(vShaderName,fShaderName);
	attrIDs["aPos"] = glGetAttribLocation(progID, "aPos");
	attrIDs["aNor"] = glGetAttribLocation(progID, "aNor");
	unifIDs["P"] = glGetUniformLocation(progID, "P");
	unifIDs["MV"] = glGetUniformLocation(progID, "MV");
	loader.load(RESOURCE_DIR + "cube.obj",posBuf,norBuf,indCount);
    GLuint tmp[2];
    glGenBuffers(2, tmp);
    bufIDs["bPos"] = tmp[0];
    bufIDs["bNor"] = tmp[1];
    //pass model data to buffers
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bPos"]);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bNor"]);
    glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);                
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	robot.makeRobot();
    ActonHandler::setRobot(robot);
   
	
}
static void prepare(glm::mat4& P)
{
    glUseProgram(progID);
    glUniformMatrix4fv(unifIDs["P"], 1, GL_FALSE, value_ptr(P));
    glEnableVertexAttribArray(attrIDs["aPos"]);
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bPos"]);
    glVertexAttribPointer(attrIDs["aPos"], 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(attrIDs["aNor"]);
    glBindBuffer(GL_ARRAY_BUFFER, bufIDs["bNor"]);
    glVertexAttribPointer(attrIDs["aNor"], 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	
}
static void cleanUp()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(attrIDs["aNor"]);
    glDisableVertexAttribArray(attrIDs["aPos"]);
    glUseProgram(0);
}
static void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width, height;
	glfwGetFramebufferSize(window.getWindowID(), &width, &height);
    float aspect = width/(float)height;
    glm::mat4 P = glm::perspective((float)(45.0*M_PI/180.0), aspect, 0.01f, 100.0f);            
	MatrixStack MV;
   
    prepare(P);
    robot.torso->draw( MV,unifIDs,indCount);
    cleanUp();

   	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		std::cout << "Please specify the resource directory." << std::endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + std::string("/");
 	window = Window("Jorge Farinacci",640, 480);
	window.setVsync(1);
	window.setCallbacks();
	init();
	while(!window.shouldClose()) 
	{
		render();		
		window.updateScene();
		window.processEvents();
	}
	window.terminateProgram();

	return 0;
}