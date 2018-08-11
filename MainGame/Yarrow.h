#pragma once
#ifndef YARROW_H
#define YARROW_H

#include "PickedObject.h"

class Yarrow : public PickedObject
{
protected:

public:
	Yarrow(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	int strength;
	void setType(int typeOfObject) override;
};

#endif