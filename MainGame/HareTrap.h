#pragma once
#ifndef HARETRAP_H
#define HARETRAP_H

#include "PickedObject.h"

class HareTrap : public PickedObject
{
protected:
public:
	HareTrap(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	void prepareSpriteNames(long long elapsedTime, float scaleFactor) override;
	void setType(int typeOfObject) override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	Vector2f getEnterPosition() const;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
};

#endif