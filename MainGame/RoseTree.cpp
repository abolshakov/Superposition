#include "RoseTree.h"
#include "Helper.h"

RoseTree::RoseTree(std::string objectName, Vector2f centerPosition) : TerrainObject(objectName, centerPosition)
{
	strength = 0;
	intersectsRadius = 200;
}

Vector2i RoseTree::calculateTextureOffset()
{
	textureBox.width *= getScaleRatio().x;
	textureBox.height *= getScaleRatio().y;

	if (typeOfImage == "1")	
		return Vector2i(textureBox.width / 3, int(textureBox.height / 1.2));
	if (typeOfImage == "2")	
		return Vector2i(textureBox.width / 1.7, int(textureBox.height / 1.25));
	if (typeOfImage == "3")	
		return Vector2i(textureBox.width / 2.6, int(textureBox.height / 1.15));

	return Vector2i(textureBox.width / 2, int(textureBox.height / 1.25));
}

Vector2f RoseTree::getScaleRatio()
{
	if (typeOfImage == "1")
		return Vector2f(Helper::GetScreenSize().x / (originalTextureBox.width * 3), Helper::GetScreenSize().y / (originalTextureBox.height * 1.5));
	else
		if (typeOfImage == "2")
			return Vector2f(Helper::GetScreenSize().x / (originalTextureBox.width * 3), Helper::GetScreenSize().y / (originalTextureBox.height * 1.5));
		else
			if (typeOfImage == "3")
				return Vector2f(Helper::GetScreenSize().x / (originalTextureBox.width * 3), Helper::GetScreenSize().y / (originalTextureBox.height * 1.3));
	return Vector2f(0, 0);
}

void RoseTree::initPedestal()
{
	if (typeOfImage == "1")
	{
		/*focus1 = Vector2i(textureBox.left + int(textureBox.width / 3.8), int(position.y + textureBox.height / 23));
		focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 1.8), int(position.y + textureBox.height / 23));*/
		focus1 = Vector2i(position.x - textureBox.width / 12, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 12, position.y);

		ellipseSize = int((focus2.x - focus1.x) * 1.4);		
	}
	if (typeOfImage == "2")
	{
		/*focus1 = Vector2i(textureBox.left + int(textureBox.width / 2.3), int(position.y));
		focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 3.3), int(position.y));*/
		focus1 = Vector2i(position.x - textureBox.width / 8, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 8, position.y);
		ellipseSize = int((focus2.x - focus1.x) * 1.2);
	}
	if (typeOfImage == "3")
	{
		/*focus1 = Vector2i(textureBox.left + int(textureBox.width / 3.5), int(position.y + textureBox.height / 20));
		focus2 = Vector2i(textureBox.left + textureBox.width - int(textureBox.width / 2), int(position.y + textureBox.height / 20));*/
		focus1 = Vector2i(position.x - textureBox.width / 10, position.y);
		focus2 = Vector2i(position.x + textureBox.width / 10, position.y);
		ellipseSize = int((focus2.x - focus1.x) * 1.3);
	}
	//textureBoxOffset = Vector2i((focus1.x + focus2.x) / 2 - textureBox.left, focus1.y - textureBox.top);
}

std::string RoseTree::getSpriteName(long long elapsedTime)
{
	return "roseTree" + typeOfImage + ".png";
}