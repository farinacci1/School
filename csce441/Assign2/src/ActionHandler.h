#ifndef ACTION_HANDLER_H_
#define ACTION_HANDLER_H_
#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLSL.h"
#include "MatrixStack.h"
#include "Robot.h"
class ActonHandler
{
    
    static Robot robot;
    public:
       
        static void setRobot(Robot rob)
        {
            robot = rob;
        }
        // This function is called when a GLFW error occurs
        static void error_callback(int error, const char *description)
        {
            std::cerr << description << std::endl;
        }

        // This function is called when a key is pressed
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            if(action == GLFW_PRESS)
            {
                switch(key)
                {
                    case GLFW_KEY_ESCAPE:
                        glfwSetWindowShouldClose(window, GL_TRUE);
                        break;
                    case GLFW_KEY_PERIOD:
                        robot.pos_iter += 1;
                        if( robot.pos_iter >= robot.pos.size() )
                            robot.pos_iter = robot.pos.size();
                        if(robot.pos_iter <  robot.pos.size())
                            robot.pos[robot.pos_iter]->scale += glm::vec3(.2,.2,.2);
                        if(robot.pos_iter > 0 &&  robot.pos_iter < robot.pos.size())
                            robot.pos[robot.pos_iter - 1]->scale = robot.pos[robot.pos_iter - 1]->sf;
                        break;
                    case GLFW_KEY_COMMA:
                        robot.pos_iter -= 1;
                        if (robot.pos_iter <= - 1)robot.pos_iter = -1;

                        if( robot.pos_iter < robot.pos.size() - 1)
                        {
                            robot.pos[robot.pos_iter+1]->scale = robot.pos[robot.pos_iter+1]->sf;
                            
                        }
                        if(robot.pos_iter >= 0 && robot.pos_iter <= robot.pos.size() - 1)
                        {
                            robot.pos[robot.pos_iter]->scale += glm::vec3(.2,.2,.2);

                        }
                        break;
                    case GLFW_KEY_X:
                        switch(mods)
                        {
                            case GLFW_MOD_SHIFT:
                            case GLFW_MOD_CAPS_LOCK:
                            if(robot.pos_iter >= 0)
                                robot.pos[robot.pos_iter]->rot.x -= 5;
                            break;

                            default:
                            if(robot.pos_iter >= 0)
                                robot.pos[robot.pos_iter]->rot.x += 5;
                            break;

                        }

                        break;
                    case GLFW_KEY_Y:
                        switch(mods)
                        {
                            case GLFW_MOD_SHIFT:
                            case GLFW_MOD_CAPS_LOCK:
                            if(robot.pos_iter >= 0)
                                robot.pos[robot.pos_iter]->rot.y -= 5;
                            break;

                            default:
                            if(robot.pos_iter >= 0)
                                robot.pos[robot.pos_iter]->rot.y += 5;
                            break;

                        }
                        break;
                    case GLFW_KEY_Z:
                        switch(mods)
                        {
                            case GLFW_MOD_SHIFT:
                            case GLFW_MOD_CAPS_LOCK:
                            if(robot.pos_iter >= 0)
                                robot.pos[robot.pos_iter]->rot.z -= 5;
                            break;

                            default:
                            if(robot.pos_iter >= 0)
                                robot.pos[robot.pos_iter]->rot.z += 5;
                            break;

                        }
                        break;                    
                }
            }
        }

        // This function is called when the mouse is clicked
        static void mouse_callback(GLFWwindow *window, int button, int action, int mods)
        {
            // Do nothing
        }

        // If the window is resized, capture the new size and reset the viewport
        static void resize_callback(GLFWwindow *window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
};
#endif