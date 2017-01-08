#include "StaticObject.h"

class TerrainObject : public StaticObject
{
public:
	TerrainObject(Vector2f position, Vector2u size, string name, string type);
	int elipseSize;
	Vector2f focus1, focus2;
	bool isTouch(Vector2f position);
};

