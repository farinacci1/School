#pragma once
#ifndef _LIGHT_H_
#define _LIGHT_H_
#include <glm/gtc/matrix_transform.hpp>


class Light : public Entity
{
        glm::vec3 color;//color of light (ignore for now our light will be white)
        float intensity;//intensity of light (inidicator of brightness)
    public:

        Light(): intensity(0.8)
        {
        }
        void setLightPos(glm::vec3 p)
        {
            Entity::setPosition(p);
            Entity::setOriginal(p);
        }
        void setIntensity(float i){intensity = i;}
        float getIntensity(){return intensity;}
        void mouseClicked(float x, float y, bool shift, bool ctrl, bool alt) override{}
	    void mouseMoved(float x, float y) override{}
	    void keyEvent(unsigned int key,int action) override{}
};

#endif