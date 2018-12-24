#include "TerrainObject.h"
#include "Helper.h"

TerrainObject::TerrainObject(std::string objectName, Vector2f centerPosition) : StaticObject(objectName, centerPosition)
{	
	isTerrain = true;
}

TerrainObject::~TerrainObject()
{

}

bool TerrainObject::isIntersected(Vector2f curPosition, float radius, Vector2f newPosition) const
{
	if (this->isMultiellipse)
		return false;

	if (this->isDotsAdjusted)
	{
		auto d1 = this->getDot1();
		auto d2 = this->getDot2();
		auto const xDec = abs(d2.x - d1.x) * 0.1, yDec = abs(d2.y - d1.y) * 0.1;
		d1.x -= xDec;
		d1.y -= yDec;
		d2.x += xDec;
		d2.y += yDec;

		auto const triangle1 = Helper::triangleArea(d1.x, d1.y, curPosition.x, curPosition.y, d2.x, d2.y);
		auto const triangle2 = Helper::triangleArea(d1.x, d1.y, newPosition.x, newPosition.y, d2.x, d2.y);

		if (triangle1 >= 0 && triangle2 < 0 || triangle2 >= 0 && triangle1 < 0)
		{
			float toD1 = sqrt(pow(newPosition.x - d1.x, 2) + pow(newPosition.y - d1.y, 2));
			float toD2 = sqrt(pow(newPosition.x - d2.x, 2) + pow(newPosition.y - d2.y, 2));

			//if (toD1 + toD2 <= std::max(abs(d2.x - d1.x), abs(d1.y - d2.y)))
				//return true;

			/*if (((newPosition.x >= d1.x && newPosition.x <= d2.x) || (newPosition.x >= d2.x && newPosition.x <= d1.x)) ||
				((newPosition.y >= d1.y && newPosition.y <= d2.y) || (newPosition.y >= d2.y && newPosition.y <= d1.y)))
				return true;*/
		}

		//return false;

		return (triangle1 >= 0 && triangle2 < 0 || triangle2 >= 0 && triangle1 < 0) &&
			(((newPosition.x >= d1.x && newPosition.x <= d2.x) || (newPosition.x >= d2.x && newPosition.x <= d1.x))
				|| ((newPosition.y >= d1.y && newPosition.y <= d2.y) || (newPosition.y >= d2.y && newPosition.y <= d1.y)));
	}

	auto const f1 = this->getFocus1();
	auto const f2 = this->getFocus2();

	Vector2f const position = newPosition;
	return (sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)) <= this->getEllipseSize());
}

std::vector<int> TerrainObject::getMultiellipseIntersect(Vector2f position) const
{
	std::vector<int> ans;
	if (this->internalEllipses.empty())	
		return ans;

	for (int i = 0; i < this->internalEllipses.size(); i++)
	{
		auto const f1 = this->internalEllipses[i].second.first;
		auto const f2 = this->internalEllipses[i].second.second;

		if (sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)/* - dynamic.radius*/) <= this->internalEllipses[i].first.first)
			ans.push_back(i);
	}

	return ans;
}

Vector2f TerrainObject::newSlippingPositionForDotsAdjusted(Vector2f position, float radius, float speed, float elapsedTime)
{
	Vector2f const dynamicPos = position;
	Vector2f dot1 = Vector2f(this->getDot1()), dot2 = Vector2f(this->getDot2());

	float a = (dot2.x - dot1.x);
	float b = (dot2.y - dot1.y);
	float k = pow((speed * elapsedTime), 2) / sqrt(a * a + b * b);

	Vector2f newPos1 = Vector2f(dynamicPos.x + b, dynamicPos.y - a), newPos2 = Vector2f(dynamicPos.x - b, dynamicPos.y + a);

	float xOffset = 0, yOffset = 0;

	if (this->isIntersected(position, radius, newPos1))
	{
		xOffset = k * -b;
		yOffset = k * a;
	}
	if (this->isIntersected(position, radius, newPos2))
	{
		xOffset = k * b;
		yOffset = k * -a;
	}

	return Vector2f(xOffset, yOffset);
}