#ifndef ENTITIY_H_
#define ENTITIY_H_

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
struct Entity
{
    std::vector<std::shared_ptr<Entity>> children;
    glm::vec3 tParent = glm::vec3(0,0,0);
    glm::vec3 rot = glm::vec3(0,0,0);
    glm::vec3 tJoint = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);
    glm::vec3 sf = glm::vec3(1,1,1);
    bool isVisited = false;
    Entity( 
        glm::vec3 tP, 
        glm::vec3 r ,
        glm::vec3 tJ,
        glm::vec3 s)
    {
            
        tParent = tP;
        tJoint = tJ;
        rot = r;
        scale = s;
        sf = s;
    }
    


    void draw(MatrixStack& MV,std::map<std::string,GLint>& unifIDs,int indCount)
    {
       
        MV.pushMatrix();
            MV.translate(tParent);
            MV.rotate(glm::radians(rot.x), 1, 0, 0);
            MV.rotate(glm::radians(rot.y), 0, 1, 0);
            MV.rotate(glm::radians(rot.z), 0, 0, 1);
            MV.pushMatrix();
            {
                MV.translate(tJoint);
                MV.scale(scale);
                glUniformMatrix4fv(unifIDs["MV"], 1, GL_FALSE, value_ptr(MV.topMatrix()));
                glDrawArrays(GL_TRIANGLES, 0, indCount);

            }
            MV.popMatrix();
            for(unsigned int i = 0; i < this->children.size();i++)
            {
                children[i]->draw(MV,unifIDs,indCount);
            }
        MV.popMatrix();
        
        
    }
};
class Robot
{
    public:
        int pos_iter = -1;
        std::shared_ptr<Entity> torso;
        std::vector<std::shared_ptr<Entity>> pos;
        Robot(){}
        void makeRobot()
        {
            torso = std::make_shared<Entity>(       
                glm::vec3(0,3,-20),
                glm::vec3(0,0,0),
                glm::vec3(0,0,0),
                glm::vec3(3,4,3)
            );
            std::shared_ptr<Entity> head =  std::make_shared<Entity>(
                glm::vec3(0,2,0),
                glm::vec3(0,0,0),
                glm::vec3(0,1,0),
                glm::vec3(2,2,2)
            );
            std::shared_ptr<Entity> lLegT =  std::make_shared<Entity>(
                glm::vec3(0.75,-2,0),
                glm::vec3(0,0,0),
                glm::vec3(0,-2,0),
                glm::vec3(1.4,4,1.4)
            );
            std::shared_ptr<Entity> rLegT =  std::make_shared<Entity>(
                glm::vec3(-0.75,-2,0),
                glm::vec3(0,0,0),
                glm::vec3(0,-2,0),
                glm::vec3(1.4,4,1.4)
            );
            std::shared_ptr<Entity> lArmT =  std::make_shared<Entity>(
                glm::vec3(-1.5,1,0),
                glm::vec3(0,0,0),
                glm::vec3(-1.5,0,0),
                glm::vec3(4,1.3,1)
            );  
            std::shared_ptr<Entity> rArmT =  std::make_shared<Entity>(
                glm::vec3(1.5,1,0),
                glm::vec3(0,0,0),
                glm::vec3(1.5,0,0),
                glm::vec3(4,1.3,1)
            );              
            std::shared_ptr<Entity> rArmB =  std::make_shared<Entity>(
                glm::vec3(2.5,0,0),
                glm::vec3(0,0,0),
                glm::vec3(2.5,0,0),
                glm::vec3(3,1,1)
            );
            std::shared_ptr<Entity> lArmB =  std::make_shared<Entity>(
                glm::vec3(-2.5,0,0),
                glm::vec3(0,0,0),
                glm::vec3(-2.5,0,0),
                glm::vec3(3,1,1)
            );      
            std::shared_ptr<Entity> rLegB =  std::make_shared<Entity>(
                glm::vec3(0,-2.7,0),
                glm::vec3(0,0,0),
                glm::vec3(0,-2.7,0),
                glm::vec3(1.2,3,1.4)
            );
            std::shared_ptr<Entity> lLegB =  std::make_shared<Entity>(
                glm::vec3(0,-2.7,0),
                glm::vec3(0,0,0),
                glm::vec3(0,-2.7,0),
                glm::vec3(1.2,3,1.4)
            ); 
            torso->children.push_back(head);
            torso->children.push_back(rLegT);
            torso->children.push_back(lLegT);
            torso->children.push_back(lArmT);
            torso->children.push_back(rArmT);               
            rLegT->children.push_back(rLegB);
            lLegT->children.push_back(lLegB);   
            rArmT->children.push_back(rArmB);
            lArmT->children.push_back(lArmB);
            
            
            dfs(torso);
                
        }
        void dfs(std::shared_ptr<Entity> Entity)
        {
            Entity->isVisited = true;
            pos.push_back(Entity);
            for(unsigned int i = 0; i < Entity->children.size();i++)
            {
                if(!Entity->children[i]->isVisited)
                {
                    Entity->children[i]->isVisited = true;
                    dfs(Entity->children[i]);
                }
            }
        }
};
#endif