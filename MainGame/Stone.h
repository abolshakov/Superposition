#pragma once
#include "TerrainObject.h"

class Stone : public TerrainObject
{	
public:	
	Stone(std::string objectName, Vector2f centerPosition);
	Vector2f calculateTextureOffset() override;
	void initPedestal() override;
	const std::string getSpriteName(long long elapsedTime) override;	
	int strength;
};


