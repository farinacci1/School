#pragma once
#ifndef _BATCH_H_
#define _BATCH_H_

#include <string>
#include <vector>
#include <memory>
#include "../model/Shape.h"
#include "Entity.h"

enum Type { ASSET, CAMERA,LIGHT};
class Batch
{

public:
    Type type;
	std::shared_ptr<Shape>  shape;
    bool withShape = false;
	//std::vector<Entity*> copies;
    std::vector<Entity*> copies;
    void setShape(std::shared_ptr<Shape>& s){ shape = s;withShape = true;}
    void setType(Type t){type = t;}
    

    void insertEntity(Entity* e)
    {
        copies.emplace_back(e);
    }
    bool hasShape(){return withShape;}
};

#endif