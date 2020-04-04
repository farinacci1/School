#pragma once
#ifndef _GAME_ASSET_H_
#define _GAME_ASSET_H_
#include <glm/gtc/matrix_transform.hpp>

#include "../model/Texture.h"

class GameAsset : public Entity
{
public:
    GameAsset() :shine(20){}
    ~GameAsset(){}
    void setDiffuseMap(Texture & d){diffuse = d; hasDiffuseMap = true;}
    void setNormalMap(Texture & n){normal = n; hasNormalMap = true;}
	void setSpecularMap(Texture & s){specular = s; hasSpecularMap = true;}
    void setOcclusionMap(Texture & a){ambient = a; hasAmbientMap = true;}
    void setKd(glm::vec3 col){kd = col; hasKd = true;}
    void setKa(glm::vec3 col){ka = col; hasKa = true;}
    void setKs(glm::vec3 col){ks = col; hasKs = true;}
    void setShine(float s){shine = s; hasShine = true;}


  
    Texture getDiffuseMap(){return diffuse;}
    Texture getNormalMap(){return normal;}
    Texture getSpecularMap(){return specular;}
    Texture getOcclusionMap(){return ambient;}
    

    glm::vec3 getKd(){return kd ;}
    glm::vec3 getKa(){return ka;}
    glm::vec3 getKs(){return ks;}
    float getShine(){return shine;}

	void mouseClicked(float x, float y, bool shift, bool ctrl, bool alt) override{};
	void mouseMoved(float x, float y) override{};
	void keyEvent(unsigned int key,int action) override{};


private:
    glm::vec3 kd; //diffuse constant in place of diffuse map
    glm::vec3 ka;// ambient constant in place of ambient map  
    glm::vec3 ks;// specular constant in place of specular map
    float shine;//Shine damp

    Texture ambient;
    Texture diffuse;
    Texture normal;
    Texture specular;
    bool hasAmbientMap = false;
    bool hasDiffuseMap = false;
    bool hasNormalMap = false;
    bool hasSpecularMap = false;
     
    
    bool hasKd = false;
    bool hasKa = false;
    bool hasKs = false;
    bool hasShine = false;
};

#endif