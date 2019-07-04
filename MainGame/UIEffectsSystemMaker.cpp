#include "UIEffectsSystemMaker.h"

float UIEffectsSystemMaker::defaultDuration = 10e5;

UIEffectsSystemMaker::UIEffectsSystemMaker()
{
}


UIEffectsSystemMaker::~UIEffectsSystemMaker()
{
}

void UIEffectsSystemMaker::init()
{
	//for (auto& item : names)	
		//item.second = false;	
}

void UIEffectsSystemMaker::resetEffects(std::vector<std::string> removeList)
{
	int cnt = -1;
	for (const auto& name : removeList)
	{
		if (names.count(name) == 0)
			continue;
		cnt++;
		auto sprite = dynamic_cast<Sprite*>(transparencyRemoval.at(name).body);
		if (sprite)
		{
			const Color spriteColor = sprite->getColor();
			sprite->setColor(Color(spriteColor.r, spriteColor.g, spriteColor.b, 255));
		}
		const auto text = dynamic_cast<Text*>(transparencyRemoval.at(name).body);
		if (text)
		{
			const Color textColor = text->getFillColor();
			text->setFillColor(Color(textColor.r, textColor.g, textColor.b, 255));
		}
		transparencyRemoval.erase(name);
		names.erase(name);
	}
}


void UIEffectsSystemMaker::addEffect(UIEffects effect, Drawable* elem, std::string name, float duration)
{
	switch (effect)
	{
		case UIEffects::transparencyRemoval:
			if (names.count(name) == 0)
			{
				names.insert(name);
				transparencyRemoval[name] = uiEffect(elem, duration);
				auto sprite = dynamic_cast<Sprite*>(transparencyRemoval.at(name).body);
				if (sprite)
					sprite->setColor(Color(sprite->getColor().r, sprite->getColor().g, sprite->getColor().b, 0));
				auto text = dynamic_cast<Text*>(transparencyRemoval.at(name).body);
				if (text)
					text->setFillColor(Color(text->getFillColor().r, text->getFillColor().g, text->getFillColor().b, 0));
			}
			break;
	}
}


void UIEffectsSystemMaker::interact(float elapsedTime)
{
	int cnt = -1;
	for (const auto& name : names)
	{
		cnt++;
		if (transparencyRemoval.at(name).timePassed >= transparencyRemoval.at(name).duration)
		{
			transparencyRemoval.erase(name);
			names.erase(name);				
			continue;
		}
		auto sprite = dynamic_cast<Sprite*>(transparencyRemoval.at(name).body);
		if (sprite)
		{
			const Color spriteColor = sprite->getColor();
			sprite->setColor(Color(spriteColor.r, spriteColor.g, spriteColor.b, 255 * transparencyRemoval.at(name).timePassed / transparencyRemoval.at(name).duration));			
			transparencyRemoval.at(name).timePassed += elapsedTime;
		}
		const auto text = dynamic_cast<Text*>(transparencyRemoval.at(name).body);
		if (text)
		{
			const Color textColor = text->getFillColor();
			text->setFillColor(Color(textColor.r, textColor.g, textColor.b, 255 * transparencyRemoval.at(name).timePassed / transparencyRemoval.at(name).duration));
			transparencyRemoval.at(name).timePassed += elapsedTime;
		}
	}
}