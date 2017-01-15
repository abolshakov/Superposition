#pragma once
#include "TerrainObject.h"

class Stone : public TerrainObject
{	
public:	
	Stone(std::string name, FloatRect boundingBox, IntRect textureBox);
	const std::string getSpriteName(long long elapsedTime) override;	
};


