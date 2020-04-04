#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"


Camera::Camera() :
	aspect(1.0f),
	fovy((float)(45.0*M_PI/180.0)),
	znear(0.1f),
	zfar(1000.0f),
	rfactor(0.01f),
	tfactor(0.05f)
{
 	Entity::setPosition(glm::vec3(0.0f,0.0f,5.f));
}

Camera::~Camera()
{
}

void Camera::mouseClicked(float x, float y, bool shift, bool ctrl, bool alt){}
void Camera::mouseMoved(float x, float y)
{
	glm::vec2 mouseCurr(x, y);
	glm::vec2 dv = mouseCurr - Entity::getMousePrev();
	Entity::updateRotX(dv.y,rfactor);
	Entity::updateRotY(dv.x,rfactor);
	Entity::setMousePrev(mouseCurr);
	if(Entity::getRotX() > 89.0f) Entity::setRotX(89.0f);
	if(Entity::getRotX() < -89.0f) Entity::setRotX(-89.0f);
}
void Camera::keyEvent(unsigned int key,int action)
{
	
	switch(key)
	{
		case 'w':
		case 'W':
			Entity::updatePosX(-tfactor);
			break;
		case 'a':
		case 'A':
			Entity::updatePosZ(tfactor);
			break;
		case 's':
		case 'S':
			Entity::updatePosX(tfactor);
			break;
		case 'd':
		case 'D':
			Entity::updatePosZ(-tfactor);
			break;
	}
}
void Camera::applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const
{
	// Modify provided MatrixStack
	P->multMatrix(glm::perspective(fovy, aspect, znear, zfar));
}
glm::mat4 Camera::applyViewMatrix()
{
	glm::vec3 front = glm::vec3(0.0f,0.0f,-1.0f);
	glm::vec3 Right;
	
	glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
	front.x = cos(glm::radians(Entity::getRotation().y)) * cos(glm::radians(Entity::getRotation().x));
    front.y = sin(glm::radians(Entity::getRotation().x));
    front.z = sin(glm::radians(Entity::getRotation().y)) * cos(glm::radians(Entity::getRotation().x));
	front = -glm::normalize(front);
	Right = glm::normalize(glm::cross(front, up));
   	up  = glm::normalize(glm::cross(Right, front));	
	return glm::lookAt(Entity::getPosition(),Entity::getPosition() + front,up);
}
void Camera::applyViewMatrix(std::shared_ptr<MatrixStack> MV) const
{
/*do nothing*/
}
