#ifndef MATERIAL_H_
#define MATERIAL_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
    public:
        glm::vec3 Ka;//ambient intensity
        glm::vec3 Kd;//diffuse intensity
        glm::vec3 Ks;//specular intensity
        float s; //shininess factor,
        Material( glm::vec3 Ia,glm::vec3 Id,glm::vec3 Is, float shineDamper )
        {
           
            Ka = Ia;
            Kd = Id;
            Ks = Is;
            s = shineDamper;

        }
};
#endif