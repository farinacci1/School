#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "entity/Camera.h"
#include "shader/GLSL.h"
#include "MatrixStack.h"
#include "shader/Program.h"
#include "model/Shape.h"
#include "model/Texture.h"
#include "entity/Entity.h"
#include "entity/Batch.h"
#include "entity/GameAsset.h"
#include "entity/Light.h"

#include <vector>
using namespace std;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from

shared_ptr<Camera> camera;
shared_ptr<Program> prog;

shared_ptr<Shape> bunny;
shared_ptr<Shape> teapot;
shared_ptr<Shape> sphere;
shared_ptr<Batch> b;
shared_ptr<Batch> t;
shared_ptr<Batch> l;
shared_ptr<Batch> f;
glm::vec3 lightPosCam;
double start_time,end_time,elapsed;
shared_ptr<Light>  light;
shared_ptr<Shape>  plane;
bool keyToggles[256] = {false}; // only for English keyboards!



double random_color( void )
{/**/
static unsigned long seed = 123;
seed = (1103515245L * seed + 12345L ) & 0x7fffffffL;
return 1.0 - ((double)seed/(double)0x3fffffff);
}
int rand_int(int modFactor)
{
	static unsigned long seed = 500;
	seed = (1103515245L * seed + 12345L ) & 0x7fffffffL;
	return seed % modFactor;
}

int random_select_arg(int numArgs)
{
	static unsigned long seed = 1324;
	seed = (1103515245L * seed + 12345L );
	return seed % numArgs;	
}
// This function is called when a GLFW error occurs
static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

// This function is called when a key is pressed
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

// This function is called when the mouse is clicked
static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	// Get the current mouse position.
	double xmouse, ymouse;
	glfwGetCursorPos(window, &xmouse, &ymouse);
	// Get current window size.
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if(action == GLFW_PRESS) {
		bool shift = (mods & GLFW_MOD_SHIFT) != 0;
		bool ctrl  = (mods & GLFW_MOD_CONTROL) != 0;
		bool alt   = (mods & GLFW_MOD_ALT) != 0;
		camera->mouseClicked((float)xmouse, (float)ymouse, shift, ctrl, alt);
	}
}

// This function is called when the mouse moves
static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
	//int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	//if(state == GLFW_PRESS) {
		camera->mouseMoved((float)xmouse, (float)ymouse);
	//}
}

static void char_callback(GLFWwindow *window, unsigned int key)
{
	camera->keyEvent(key,0);
}

// If the window is resized, capture the new size and reset the viewport
static void resize_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// This function is called once to initialize the scene and OpenGL
static void init()
{
	// Initialize time.
	glfwSetTime(0.0);
	
	// Set background color.
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	prog = make_shared<Program>();
	prog->setShaderNames(RESOURCE_DIR + "vert.glsl", RESOURCE_DIR + "frag.glsl");
	prog->setVerbose(true);
	prog->init();
	prog->addAttribute("aPos");
	prog->addAttribute("aNor");
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addUniform("T");
	prog->addUniform("ka");
	prog->addUniform("kd");
	prog->addUniform("ks");
	prog->addUniform("s");
	prog->addUniform("lightPosCam");
	prog->addUniform("lightPos1");
	prog->addUniform("Li1");
	prog->setVerbose(false);
	
	

	light = make_shared<Light>();
	light->setLightPos(glm::vec3(5.0f,5.0f,2.0f));
	light->setIntensity(0.8);
	camera = make_shared<Camera>();
	camera->setPosition(glm::vec3(0.0f,1.0f,5.0f));

	

	
	lightPosCam.x = 1.0f;
	lightPosCam.y = 1.0f;
	lightPosCam.z = 1.0f;
	plane = make_shared<Shape>();
	plane->loadMesh(RESOURCE_DIR + "floor.obj");
	plane->init();
	f =  make_shared<Batch>();
	f->setType(Type::ASSET);
	f->setShape(sphere);
	GameAsset *ef = new GameAsset();
	ef->setPosition(glm::vec3(0.0f,0.0f,0.0f));
	ef->setOriginal(glm::vec3(0.0f,0.0f,0.0f));	
	ef->setScale(glm::vec3(200.0f,0.0f,100.0f));
	ef->setKd(glm::vec3(0.f,1.f,0.f));
	ef->setKs(glm::vec3(.1f,.1f,.1f));
	ef->setKa(glm::vec3(.1f,.1f,.1f));
	ef->setShine(1.f);
	f->insertEntity(ef);

	sphere = make_shared<Shape>();
	sphere->loadMesh(RESOURCE_DIR + "sphere.obj");
	sphere->init();
	l =  make_shared<Batch>();
	l->setType(Type::ASSET);
	l->setShape(sphere);
	GameAsset *eli = new GameAsset();
	eli->setPosition(glm::vec3(5.0f,5.0f,2.0f));
	eli->setOriginal(glm::vec3(5.0f,5.0f,2.0f));	
	eli->setKd(glm::vec3(1.f,1.f,0.f));
	eli->setKs(glm::vec3(.2f,.2f,.2f));
	eli->setKa(glm::vec3(.2f,.2f,.2f));
	eli->setShine(1.f);
	l->insertEntity(eli);

	bunny = make_shared<Shape>();
	bunny->loadMesh(RESOURCE_DIR + "bunny.obj");
	bunny->init();
	b =  make_shared<Batch>();
	b->setType(Type::ASSET);
	b->setShape(bunny);
	for(int i = 0; i <30;i++)
	{
		GameAsset *e = new GameAsset();
		e->setKd(glm::vec3(random_color(),random_color(),random_color()));
		e->setKs(glm::vec3(random_color(),random_color(),random_color()));
		e->setKa(glm::vec3(.2f,.2f,.2f));
		int state = random_select_arg(2);

		glm::vec3 randPos = glm::vec3(rand_int(30),0.0f,rand_int(30)) * 0.5f;
		glm::vec3 randRot = glm::vec3(0.0f,rand_int(360.f),0.0f);
		if(state == 1) randPos = -randPos;
		e->setPosition(randPos);
		e->setOriginal(randPos);
		e->setRotation(randRot);
		e->setScale(0.5f);
		b->insertEntity(e);

	}
	 
	teapot = make_shared<Shape>();
	teapot->loadMesh(RESOURCE_DIR + "teapot.obj");
	teapot->init();	
	t =  make_shared<Batch>();
	t->setType(Type::ASSET);
	t->setShape(teapot);
		for(int i = 0; i <30;i++)
	{
		GameAsset *e = new GameAsset();
		e->setKd(glm::vec3(random_color(),random_color(),random_color()));
		e->setKs(glm::vec3(random_color(),random_color(),random_color()));
		e->setKa(glm::vec3(.2f,.2f,.2f));
		int state = random_select_arg(2);

		glm::vec3 randPos = glm::vec3(rand_int(30),0.0f,rand_int(30)) * 0.5f;
		glm::vec3 randRot = glm::vec3(0.0f,rand_int(360.f),0.0f);
		if(state == 1) randPos = -randPos;
		e->setPosition(randPos);
		e->setOriginal(randPos);
		e->setRotation(randRot);
		e->setScale(0.5f);
		t->insertEntity(e);

	}
	GLSL::checkError(GET_FILE_LINE);
}

// This function is called every frame to draw the scene.
static void render()
{
	
	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	// Matrix stacks
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	
	
	prog->bind();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(camera->applyViewMatrix()));
	glUniform3fv(prog->getUniform("lightPosCam"), 1, glm::value_ptr(lightPosCam));
	glUniform3fv(prog->getUniform("lightPos1"), 1, glm::value_ptr(light->getPosition()));
	glUniform1f(prog->getUniform("Li1"),  light->getIntensity());
	//plane->draw(prog);


	glUniformMatrix4fv(prog->getUniform("T"), 1, GL_FALSE, glm::value_ptr(f->copies[0]->getTransform().topMatrix()));
	glUniform3fv(prog->getUniform("kd"), 1, glm::value_ptr( ((GameAsset*)(f->copies[0]))->getKd()) );
	glUniform3fv(prog->getUniform("ks"), 1, glm::value_ptr( ((GameAsset*)(f->copies[0]))->getKs()) );
	glUniform3fv(prog->getUniform("ka"), 1, glm::value_ptr( ((GameAsset*)(f->copies[0]))->getKa()) );
	glUniform1f(prog->getUniform("s"),((GameAsset*)(f->copies[0]))->getShine() );
	plane->draw(prog);

	glUniformMatrix4fv(prog->getUniform("T"), 1, GL_FALSE, glm::value_ptr(l->copies[0]->getTransform().topMatrix()));
	glUniform3fv(prog->getUniform("kd"), 1, glm::value_ptr( ((GameAsset*)(l->copies[0]))->getKd()) );
	glUniform3fv(prog->getUniform("ks"), 1, glm::value_ptr( ((GameAsset*)(l->copies[0]))->getKs()) );
	glUniform3fv(prog->getUniform("ka"), 1, glm::value_ptr( ((GameAsset*)(l->copies[0]))->getKa()) );
	glUniform1f(prog->getUniform("s"),((GameAsset*)(l->copies[0]))->getShine() );
	sphere->draw(prog);
	for(uint i =0; i <b->copies.size();i++)
	{
		glUniformMatrix4fv(prog->getUniform("T"), 1, GL_FALSE, glm::value_ptr(b->copies[i]->getTransform().topMatrix()));
		glUniform3fv(prog->getUniform("kd"), 1, glm::value_ptr( ((GameAsset*)(b->copies[i]))->getKd()) );
		glUniform3fv(prog->getUniform("ks"), 1, glm::value_ptr( ((GameAsset*)(b->copies[i]))->getKs()) );
		glUniform3fv(prog->getUniform("ka"), 1, glm::value_ptr( ((GameAsset*)(b->copies[i]))->getKa()) );
		glUniform1f(prog->getUniform("s"),((GameAsset*)(b->copies[i]))->getShine() );
		bunny->draw(prog);
	}
	for(uint i =0; i <t->copies.size();i++)
	{
		glUniformMatrix4fv(prog->getUniform("T"), 1, GL_FALSE, glm::value_ptr(t->copies[i]->getTransform().topMatrix()));
		glUniform3fv(prog->getUniform("kd"), 1, glm::value_ptr( ((GameAsset*)(t->copies[i]))->getKd()) );
		glUniform3fv(prog->getUniform("ks"), 1, glm::value_ptr( ((GameAsset*)(t->copies[i]))->getKs()) );
		glUniform3fv(prog->getUniform("ka"), 1, glm::value_ptr( ((GameAsset*)(t->copies[i]))->getKa()) );
		glUniform1f(prog->getUniform("s"), ((GameAsset*)(b->copies[i]))->getShine() );

		teapot->draw(prog);
	}
	prog->unbind();
	
	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");

	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "YOUR NAME", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	GLSL::checkVersion();
	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	// Set char callback.
	glfwSetCharCallback(window, char_callback);
	// Set cursor position callback.
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// Set mouse button callback.
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// Set the window resize call back.
	glfwSetFramebufferSizeCallback(window, resize_callback);
	// Initialize scene.
	init();
	start_time = glfwGetTime();
	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
		end_time = glfwGetTime();
		elapsed = end_time - start_time;
		start_time = end_time;
		// Render scene.
		render();
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
