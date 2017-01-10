#pragma once
#include "TerrainObject.h"

class Stone : public TerrainObject
{
	Vector2f focus1, focus2;
public:
	int ellipseSize;
	Stone(Vector2f position, Vector2u size, std::string name);
	std::string getSpriteName(long long elapsedTime) override;	
};


