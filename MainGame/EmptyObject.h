#ifndef EMPTYOBJECT_H
#define EMPTYOBJECT_H

#include <SFML/Graphics.hpp>
#include "WorldObject.h"

class EmptyObject : public WorldObject
{
public:
	EmptyObject(std::string objectName, Vector2f centerPosition);
	~EmptyObject();
	std::string getSpriteName(long long elapsedTime) override;
	int getSpriteNumber() override;
	Vector2f getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
	int getBuildType(Vector2f ounPos, Vector2f otherPos) override;
	void setTextureSize(Vector2f textureSize) override;
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
};

#endif

