#include "TerrainObject.h"

TerrainObject::TerrainObject(std::string objectName, Vector2f ellipseCenterPosition) : StaticObject(objectName, ellipseCenterPosition)
{
	this->ellipseCenterPosition = ellipseCenterPosition;
}


