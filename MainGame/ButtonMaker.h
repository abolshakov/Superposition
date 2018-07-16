#ifndef BUTTONMAKER
#define BUTTONMAKER

#include <SFML/Graphics.hpp>

using namespace sf;

enum Tags { playTag = 0, continueTag = 1, newRunTag = 2, settingsTag = 3, exitTag = 4, buildStartTag = 5 };

class ButtonMaker
{
private:
	Sprite sprite;
	Texture texture;
	Tags tag;
public:
	ButtonMaker();	
	void initialize(Texture &texture, Vector2f position, Vector2f size, Tags tag);
	void initialize(Texture &texture, FloatRect rect, Tags tag);

	bool isSelected(Vector2f mousePos);
	void draw(RenderWindow &window);
};
#endif

