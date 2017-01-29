#pragma once
#include "TerrainObject.h"

class Stone : public TerrainObject
{	
public:	
	Stone(std::string objectName, Vector2f centerPosition);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;	
	int strength;
};


