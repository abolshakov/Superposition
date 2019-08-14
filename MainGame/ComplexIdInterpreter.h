#ifndef COMPLEXIDINTERPRETER_H
#define COMPLEXIDINTERPRETER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "WreathDraft.h"

class ComplexIdInterpreter
{
private:
	std::unordered_map<ButtonTag, DraftInfo> memorizedSetups;
public:
	ComplexIdInterpreter();
	~ComplexIdInterpreter();
	void rememberSetup(DraftInfo info, ButtonTag id);
	void clearSetup(ButtonTag id);
	DraftInfo getCurrentSetup(ButtonTag id);	
};

#endif

