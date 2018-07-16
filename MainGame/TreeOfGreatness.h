#pragma once
#ifndef TREEOFGREATNESS_H
#define TREEOFGREATNESS_H

#include "TerrainObject.h"

class TreeOfGreatness : public TerrainObject
{
protected:
public:
TreeOfGreatness(std::string objectName, Vector2f centerPosition, int typeOfObject);
Vector2i calculateTextureOffset() override;
void initPedestal() override;
std::string getSpriteName(long long elapsedTime) override;
int strength;
void setType(int typeOfObject) override;
};

#endif