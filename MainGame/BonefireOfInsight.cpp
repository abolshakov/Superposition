#include "BonefireOfInsight.h"
#include "Helper.h"

BonefireOfInsight::BonefireOfInsight(std::string objectName, Vector2f centerPosition, int typeOfObject) : TerrainObject(objectName, centerPosition)
{
	varietyOfTypes = 1;
	this->typeOfObject = typeOfObject;
	radius = 100;
	/*light->_emissionSprite.setTexture(Helper::pointLightTexture);
	light->_emissionSprite.setScale(Vector2f (25, 25));
	light->_emissionSprite.setColor(Color(229.5, 178, 178, 255));
	light->_sourceRadius = 10;*/
	toSaveName = "bonefireOfInsight";
	setType(typeOfObject);
}

void BonefireOfInsight::setType(int typeOfObject)
{
	this->typeOfObject = typeOfObject;
	this->conditionalSizeUnits = Vector2i (250, 250);
}

Vector2i BonefireOfInsight::calculateTextureOffset()
{
	textureBox.width = int(float(textureBox.width) * getScaleRatio().x);
	textureBox.height = int(float(textureBox.height) *getScaleRatio().y);
	return Vector2i (textureBox.width / 2, int(textureBox.height / 1.2));
}

void BonefireOfInsight::initPedestal()
{
	if (typeOfObject == 1)
	{
		focus1 = Vector2f(position.x - textureBox.width / 3, position.y);
		focus2 = Vector2f(position.x + textureBox.width / 3, position.y);
		ellipseSize = float((focus2.x - focus1.x) * 1.2);
	}
}

Vector2f BonefireOfInsight::getBuildPosition(std::vector<WorldObject*> visibleItems, float scaleFactor, Vector2f cameraPosition)
{
	return { -1, -1 };
}

int BonefireOfInsight::getBuildType(Vector2f ounPos, Vector2f otherPos)
{
	return 1;
}

void BonefireOfInsight::prepareSpriteNames(long long elapsedTime)
{
	additionalSprites.clear();
	spriteChainElement bonfireBody;
	bonfireBody.path = "Game/worldSprites/terrainObjects/bonefireOfInsight/bonefireOfInsight" + std::to_string(typeOfObject) + ".png";
	bonfireBody.size = Vector2f(conditionalSizeUnits);
	bonfireBody.offset = Vector2f(textureBoxOffset);
	additionalSprites.push_back(bonfireBody);
	//light->_emissionSprite.setPosition(Vector2f (Helper::GetScreenSize().x / 2, Helper::GetScreenSize().y / 2));
}