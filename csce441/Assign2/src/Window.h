#ifndef WINDOW_H_
#define WINDOW_H_

#include <cassert>
#include <cstring>
#include <iostream>

#include <GLFW/glfw3.h>
#include "GLSL.h"
#include "MatrixStack.h"
#include "ActionHandler.h"
class Window
{
    private:
        GLFWwindow * window;
    public:
        Window(const char* name="YOUR NAME",int width=640, int height=480)
        {
            // Set error callback.
	        glfwSetErrorCallback(ActonHandler::error_callback);
	        // Initialize the library.
	        if(!glfwInit()) {
		        return;
	        }
	        // Create a windowed mode window and its OpenGL context.
	        window = glfwCreateWindow(width, height, name, NULL, NULL);
	        if(!window) {
		        glfwTerminate();
		        return;
	        }
            // Make the window's context current.
	        glfwMakeContextCurrent(window);
            // Initialize GLEW.
            glewExperimental = true;
            if(glewInit() != GLEW_OK) {
                std::cerr << "Failed to initialize GLEW" << std::endl;
                return;
            }
            glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
            std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
            std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
            GLSL::checkVersion();

        }
        void setVsync(bool Vsync)
        {
            glfwSwapInterval(Vsync);
        }
        void setCallbacks()
        {
            // Set keyboard callback.
	        glfwSetKeyCallback(window, ActonHandler::key_callback);
	        // Set the mouse call back.
	        glfwSetMouseButtonCallback(window, ActonHandler::mouse_callback);
	        // Set the window resize call back.
	        glfwSetFramebufferSizeCallback(window, ActonHandler::resize_callback);

        }
        void updateScene()
        {
            // Swap front and back buffers.
            glfwSwapBuffers(window);
        }
        void processEvents()
        {
            // Poll for and process events.
		    glfwPollEvents();
        }
        bool shouldClose()
        {
            return glfwWindowShouldClose(window);
        }
        void terminateProgram()
        {
            // Quit program.
	        glfwDestroyWindow(window);
	        glfwTerminate();
        }


        GLFWwindow* getWindowID()
        {
            return window;
        }

    
};

#endif