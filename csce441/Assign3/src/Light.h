#ifndef LIGHT_H_
#define LIGHT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
    public:
        glm::vec3 pos;//position of light
        glm::vec3 color;//color of light (ignore for now our light will be white)
        float intensity;//intensity of light (inidicator of brightness)
        Light(glm::vec3 p, float i)
        {
            intensity = i;
            pos = p;

        }
};
#endif