#include "Spawn.h"
#include "Helper.h"

Spawn::Spawn(std::string objectName, Vector2f centerPosition) : TerrainObject(objectName, centerPosition)
{
}

Vector2i Spawn::calculateTextureOffset()
{
	conditionalSizeUnits = Vector2f(1000, 1000);
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;
	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.6));
}

void Spawn::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2i(textureBox.left + int(textureBox.width / 3.8), int(position.y));
		focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 3.8), int(position.y));
		ellipseSize = int((focus2.x - focus1.x) * 1.08);
	}
	if (typeOfObject == 2)
	{
		focus1 = Vector2i(textureBox.left + int(textureBox.width / 2.3), int(position.y));
		focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 3.3), int(position.y));
		ellipseSize = int((focus2.x - focus1.x) * 1.2);
	}
	if (typeOfObject == 3)
	{
		focus1 = Vector2i(textureBox.left + int(textureBox.width / 3.5), int(position.y + textureBox.height / 20));
		focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 2), int(position.y + textureBox.height / 100));
		ellipseSize = int((focus2.x - focus1.x) * 1.3);
	}
}

std::string Spawn::getSpriteName(long long elapsedTime)
{
	return "spawn" + std::to_string(typeOfObject) + ".png";
}