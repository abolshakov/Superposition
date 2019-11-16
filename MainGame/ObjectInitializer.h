#ifndef OBJECTINITIALIZER_H
#define OBJECTINITIALIZER_H

#include "StaticObject.h"
#include "SpriteStructures.h"
#include <unordered_map>

class ObjectInitializer
{
public:
	ObjectInitializer();
	~ObjectInitializer();
	static StaticObject* initializeStaticItem(
		Tag itemClass,
		Vector2f itemPosition,
		int itemType,
		std::string itemName, 
		int count, 
		Vector2f cameraPosition,
		float scaleFactor,
		std::unordered_map<std::string, BoardSprite>* spriteMap,
		std::vector<std::pair<Tag, int>> inventory = {});
	static int newNameId;
};

#endif

