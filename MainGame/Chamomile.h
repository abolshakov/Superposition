#pragma once
#ifndef CHAMOMILE_H
#define CHAMOMILE_H

#include "PickedObject.h"

class Chamomile : public PickedObject
{
protected:

public:
	Chamomile(std::string objectName, Vector2f centerPosition, int typeOfObject);
	Vector2i calculateTextureOffset() override;
	void initPedestal() override;
	std::string getSpriteName(long long elapsedTime) override;
	int strength;
	void setType(int typeOfObject) override;	
};

#endif