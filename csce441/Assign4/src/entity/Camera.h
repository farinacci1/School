#pragma  once
#ifndef __Camera__
#define __Camera__

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Entity.h"
class MatrixStack;

class Camera : public Entity 
{
public:	
	Camera();
	virtual ~Camera();
	void setAspect(float a) { aspect = a; };
	void setRotationFactor(float f) { rfactor = f; };
	void setTranslationFactor(float f) { tfactor = f; };
	void applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const;
	void applyViewMatrix(std::shared_ptr<MatrixStack> MV) const;
	glm::mat4 applyViewMatrix();
	void mouseClicked(float x, float y, bool shift, bool ctrl, bool alt) override;
	void mouseMoved(float x, float y) override;
	void keyEvent(unsigned int key,int action) override;
private:
	float aspect;
	float fovy;
	float znear;
	float zfar;
	int state;
	float rfactor;
	float tfactor;
	float sfactor;
};

#endif
