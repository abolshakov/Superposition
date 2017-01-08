#include "TerrainObject.h"

class Stone : public TerrainObject
{
	Vector2f focus1, focus2;
public:
	Stone(Vector2f position, Vector2u size, string name, string type);
	int elipseSize;
};

