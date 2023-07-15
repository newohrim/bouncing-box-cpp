#include "PlayerController.h"

#include "MoveComponent.h"

PlayerController::PlayerController(MoveComponent* moveComp) 
	: m_Target(moveComp)
{
}

void PlayerController::ProceedInput(InputState inputState, float deltaTime)
{
	Math::Vec2DF moveDir{0.0f, 0.0f};
	if (IsKeyDown(inputState, InputState::KEY_RIGHT)) {
		moveDir = moveDir + Math::Vec2DF{1.0f, 0.0f};
	}
	if (IsKeyDown(inputState, InputState::KEY_LEFT)) {
		moveDir = moveDir + Math::Vec2DF{ -1.0f, 0.0f };
	}

	if (Math::LengthSq(moveDir) < Math::EPSILON) {
		return;
	}
	m_Target->MoveOwner(Math::Normalize(moveDir), deltaTime);
}
