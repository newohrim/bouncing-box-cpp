#pragma once

#include "InputMapping.h"

class Controller
{
public:
	virtual void ProceedInput(InputState inputState, float deltaTime) = 0;
};

