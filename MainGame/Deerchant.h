#include <SFML/Graphics.hpp>
#include "Structs.h"
#include "DynamicObject.h"

using namespace sf;
using namespace std;

struct bodyElements
{
	Texture texture;
	Sprite sprite;
	string name;
};

class Deerchant : public DynamicObject
{
private:
	float lastTime, timeForNewImage;
	Clock clock;
	Vector2f screenSize;
	int bodyElementsNumber;
	int currentImg;
	int imgInRunAnimation;

public:
	string nameOfFile;
	Deerchant(Vector2f position, Vector2u size, string name, string type);
	void move(float time);
};

