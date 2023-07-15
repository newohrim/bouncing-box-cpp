#pragma once

#include "Controller.h"

class MoveComponent;

class PlayerController : public Controller
{
public:
	PlayerController(MoveComponent* moveComp);

	virtual void ProceedInput(InputState inputState, float deltaTime) override;

private:
	MoveComponent* m_Target;
};

