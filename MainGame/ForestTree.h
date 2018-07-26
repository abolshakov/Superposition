#pragma once
#ifndef FORESTTREE_H
#define FORESTTREE_H

#include "TerrainObject.h"

class ForestTree : public TerrainObject
{
protected:
public:
	ForestTree(std::string objectName, Vector2f centerPosition, int typeOfObject);
Vector2i calculateTextureOffset() override;
void initPedestal() override;
std::string getSpriteName(long long elapsedTime) override;
int strength;
void setType(int typeOfObject) override;
};

#endif