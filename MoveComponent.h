#pragma once

#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	MoveComponent(GameActor* actor);

	void MoveOwner(Math::Vec2DFN direction, float deltaTime);

	float GetMoveSpeed() const { return m_MoveSpeed; }
	void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; }

private:
	float m_MoveSpeed = 1.0f;
};

