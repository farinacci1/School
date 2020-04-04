#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>
#include <vector>
#include <memory>
#include "../MatrixStack.h"
/*
* A entity is an object which has a position, an orientation, and a scale.
*
*/
# define PI 3.14159265358979323846  /* pi */
class Entity
{

public:
    std::string identifier = "";
    Entity() : 
        originalPos(glm::vec3(0.0f, 0.0f, 0.0f)),
        position(glm::vec3(0.0f, 0.0f, 0.0f)),
        rotation(glm::vec3(0.f,0.f,0.f)),
        scale(glm::vec3(1.f,1.f,1.f))
    {}
    virtual ~Entity() { };
	glm::vec3 getPosition() const{return position;}
    glm::vec3 getOriginal()const{return originalPos;}
	glm::vec3 getScale() const{return scale;}
    glm::vec3 getRotation() const{return rotation;}
    float getRotX(){return rotation.x;}
    float getRotY(){return rotation.y;}
    float getRotZ(){return rotation.z;}
    glm::vec2 getMousePrev() const {return mousePrev;}
	MatrixStack getTransform()
    {
        MatrixStack transform;
        transform.translate(getPosition());
        transform.rotate(glm::radians(getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
	    transform.rotate(glm::radians(getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
        transform.rotate(glm::radians(getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
        transform.scale(scale);
        return transform;
    }
    void updatePosX(float x){position.x += x;}
    void updatePosY(float y){position.y += y;}
    void updatePosZ(float z){position.z += z ;}
    void updateRotX(float angle,float speed){rotation.x += angle*speed;}
    void updateRotY(float angle,float speed) {rotation.y += angle*speed;}
    void updateRotZ(float angle,float speed){rotation.z += angle*speed;}    
    //float toRadians(float angle){return ((angle * PI)/180);}
    void setOriginal(const glm::vec3& org){originalPos = org;}
    void setPosition(const glm::vec3& pos){position = pos;}
	void setScale(const glm::vec3& sc){scale = sc;}
    void setScale(const float sc){scale = glm::vec3(sc,sc,sc);}
    void setRotation(const glm::vec3& rot){rotation = rot;}
    void setRotX(const float angle){rotation.x = angle;}
    void setRotY(const float angle){rotation.y = angle;}
    void setRotZ(const float angle){rotation.z = angle;}

    void setMousePrev(const glm::vec2& mp){mousePrev = mp;}

	virtual void mouseClicked(float x, float y, bool shift, bool ctrl, bool alt) = 0;
	virtual void mouseMoved(float x, float y) = 0;
	virtual void keyEvent(unsigned int key,int action) = 0;
    
private:
    glm::vec3 originalPos;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec2 mousePrev;

};

#endif