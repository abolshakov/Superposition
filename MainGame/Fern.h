#pragma once
#ifndef FERN_H
#define FERN_H

#include "PickedObject.h"

class Fern : public PickedObject
{
protected:

public:
	Fern(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	void prepareSpriteNames(long long elapsedTime, float scaleFactor) override;
	int strength;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	Vector2f getEnterPosition() const;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};

#endif