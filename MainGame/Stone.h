#pragma once
#include "TerrainObject.h"

class Stone : public TerrainObject
{	
public:	
	Stone(std::string name, FloatRect boundingBox, Vector2f textureOffset, Vector2f textureBoxSize);
	const std::string getSpriteName(long long elapsedTime) override;	
	int strength;
};


