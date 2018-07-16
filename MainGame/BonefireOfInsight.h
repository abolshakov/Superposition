#pragma once
#ifndef BONEFIREOFINSIGHT_H
#define BONEFIREOFINSIGHT_H

#include "TerrainObject.h"

class BonefireOfInsight : public TerrainObject
{
protected:
public:
	BonefireOfInsight(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	void setType(int typeOfObject);
};

#endif