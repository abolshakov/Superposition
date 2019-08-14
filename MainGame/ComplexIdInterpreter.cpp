#include "ComplexIdInterpreter.h"



ComplexIdInterpreter::ComplexIdInterpreter()
{
}


ComplexIdInterpreter::~ComplexIdInterpreter()
{
}

void ComplexIdInterpreter::rememberSetup(DraftInfo info, ButtonTag id)
{
	memorizedSetups[id] = info;	
}

void ComplexIdInterpreter::clearSetup(ButtonTag id)
{
	return;
}


DraftInfo ComplexIdInterpreter::getCurrentSetup(ButtonTag id)
{
	return memorizedSetups.at(id);
}



